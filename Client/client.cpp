#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3000
#define TRUE 1

#define TEST_RES1 "226: Successful Download."
#define TEST_RES2 "226: List transfer done."

using namespace std;

int main()
{
    struct sockaddr_in command_serv_addr;
    struct sockaddr_in data_serv_addr;

    char buffer[1024] = {0};
    int opt = 1;

    int command_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int data_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (command_sockfd < 0 || data_sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    command_serv_addr.sin_family = AF_INET;
    command_serv_addr.sin_port = htons(PORT); // TODO: get from config.json

    data_serv_addr.sin_family = AF_INET;
    data_serv_addr.sin_port = htons(8001); // TODO: get from config.json

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
    string server_message, server_data;
    char delim = '\n';
    while (TRUE)
    {
        valread = recv(command_sockfd, buffer, sizeof(buffer), 0);
        if (valread <= 0)
        {
            perror("recv");
            continue;
        }
        buffer[valread] = '\0';
        server_message = buffer;
        cout << server_message << endl;
        if (server_message == TEST_RES1 || server_message == TEST_RES2){
            valread = recv(data_sockfd, buffer, sizeof(buffer), 0);
            if (valread <= 0)
            {
                perror("recv");
                continue;
            }
            server_data = buffer;
            cout << server_data << endl;
        }
        char char_input[100] = "";
        cin.getline(char_input, sizeof(char_input), '\n');
        cout << "char_input:" << char_input << endl;
        if (send(command_sockfd, char_input, strlen(char_input), 0) <= 0)
        {
            perror("send");
        }
    }

    return 0;
}