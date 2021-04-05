#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "json.hpp"
#include "utils.hpp"

#define CONFIG_FILE_PATH "config.json"
#define BUFF_SIZE 65536

#define SUCCESSFUL_DOWNLOAD "226: Successful Download."
#define LIST_TRANSFER "226: List transfer done."

using json = nlohmann::json;
using namespace std;

int main()
{

    ifstream config_file(CONFIG_FILE_PATH, ifstream::binary);
    string content((std::istreambuf_iterator<char>(config_file)), (std::istreambuf_iterator<char>()));

    json config;
    stringstream(content) >> config;

    const int COMMAND_PORT = config["commandChannelPort"];
    const int DATA_PORT = config["dataChannelPort"];

    struct sockaddr_in command_serv_addr;
    struct sockaddr_in data_serv_addr;

    char buffer[BUFF_SIZE] = {0};
    int opt = 1;

    int command_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int data_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (command_sockfd < 0 || data_sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    command_serv_addr.sin_family = AF_INET;
    command_serv_addr.sin_port = htons(COMMAND_PORT);

    data_serv_addr.sin_family = AF_INET;
    data_serv_addr.sin_port = htons(DATA_PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &command_serv_addr.sin_addr) <= 0)
    {
        perror("invalid address");
        exit(EXIT_FAILURE);
    }

    if (inet_pton(AF_INET, "127.0.0.1", &data_serv_addr.sin_addr) <= 0)
    {
        perror("invalid address");
        exit(EXIT_FAILURE);
    }

    int command_connectfd = connect(command_sockfd, (struct sockaddr *)&command_serv_addr, sizeof(command_serv_addr));
    int data_connectfd = connect(data_sockfd, (struct sockaddr *)&data_serv_addr, sizeof(data_serv_addr));
    if (command_connectfd < 0 || data_connectfd < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    cout << "Connected Successfully!" << endl;

    if (setsockopt(command_sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    opt = 1;
    if (setsockopt(data_sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    int valread;
    string input, server_message, server_data;
    char delim = '\n';
    while (1)
    {
        getline(cin, input, '\n');
        if (input == "")
            input = " ";
        vector<string> input_parts = split(input);
        if (send(command_sockfd, input.c_str(), input.length(), 0) <= 0)
        {
            perror("send");
        }

        memset(buffer, 0, sizeof(buffer));
        valread = recv(command_sockfd, buffer, sizeof(buffer), 0);
        if (valread < 0)
            perror("recv");
        else
        {
            buffer[valread] = '\0';
            server_message = buffer;
            cout << server_message << endl;
        }
        memset(buffer, 0, sizeof(buffer));
        if (server_message == LIST_TRANSFER || server_message == SUCCESSFUL_DOWNLOAD)
        {
            valread = recv(data_sockfd, buffer, sizeof(buffer), 0);
            if (valread < 0)
                perror("recv");
            else
            {
                buffer[valread] = '\0';
                server_data = buffer;
                cout << server_data << endl;
            }
        }

        if (input_parts.size() > 0 && input_parts[0] == "quit")
            exit(0);
    }

    return 0;
}
