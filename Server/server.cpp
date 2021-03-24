#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>    //close
#include <arpa/inet.h> //close
#include <sys/time.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "../json/single_include/nlohmann/json.hpp"
#include "../Utils/utils.hpp"
#include "CommandHandler.hpp"
#include "DataBase.hpp"
#include "Client.hpp"
#include "User.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;
using namespace std;

#define PORT 3000 // TODO: read from config.json
#define MAX_CLIENTS 20

const string init_path = fs::absolute(fs::current_path()).string();

void add_users_to_database(json config)
{
    string username, password, size, admin;
    int i = 0;
    for (int i = 0; i < config["users"].size(); i++)
    {
        json user = config["users"][i];
        username = user["user"];
        password = user["password"];
        size = user["size"];
        admin = user["admin"];
        bool is_admin = (admin == "true") ? true : false;
        User new_user(username, password, stoi(size), is_admin);
        DataBase::UserManager::add(new_user);
    }
}

int main()
{

    ifstream config_file("Server/config.json", ifstream::binary);
    string content((std::istreambuf_iterator<char>(config_file)), (std::istreambuf_iterator<char>()));

    json config;
    stringstream(content) >> config;
    add_users_to_database(config);

    struct sockaddr_in command_server;
    struct sockaddr_in data_server;

    int command_srvlen = sizeof(command_server);
    int data_srvlen = sizeof(data_server);
    int opt = 1;

    fd_set readfds;
    int max_sd, sd, activity, valread;
    int command_sd, data_sd;

    char buffer[1025];
    char client_message[1025];

    vector<Client> clients;

    int sockfd_comm = socket(AF_INET, SOCK_STREAM, 0);
    int sockfd_data = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_comm < 0 || sockfd_data < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    command_server.sin_family = AF_INET;
    command_server.sin_addr.s_addr = INADDR_ANY;
    command_server.sin_port = htons(PORT); // TODO: read port from config.json
    if (setsockopt(sockfd_comm, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    if (bind(sockfd_comm, (struct sockaddr *)&command_server, sizeof(command_server)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    listen(sockfd_comm, MAX_CLIENTS);

    opt = 1;
    data_server.sin_family = AF_INET;
    data_server.sin_addr.s_addr = INADDR_ANY;
    data_server.sin_port = htons(8001); // TODO read port from config.json
    if (setsockopt(sockfd_data, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    if (bind(sockfd_data, (struct sockaddr *)&data_server, data_srvlen) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    listen(sockfd_data, MAX_CLIENTS);

    cout << "Server Started Running on port " << PORT << endl;

    while (1)
    {
        FD_ZERO(&readfds);

        FD_SET(sockfd_comm, &readfds);
        max_sd = sockfd_comm;

        clients = DataBase::ClientManager::get_all();
        for (int i = 0; i < clients.size(); i++)
        {
            sd = clients[i].get_command_socket();

            if (sd > 0)
                FD_SET(sd, &readfds);

            if (sd > max_sd)
                max_sd = sd;
        }
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            perror("select");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(sockfd_comm, &readfds))
        {
            if ((command_sd = accept(sockfd_comm, (struct sockaddr *)&command_server, (socklen_t *)&command_srvlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            if ((data_sd = accept(sockfd_data, (struct sockaddr *)&data_server, (socklen_t *)&data_srvlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            printf("New connection , socket fd is %d , ip is : %s , port : %d  \n", command_sd, inet_ntoa(command_server.sin_addr), ntohs(command_server.sin_port));

            Client new_client(command_sd, data_sd, init_path);
            DataBase::ClientManager::add(new_client);
        }

        clients = DataBase::ClientManager::get_all();
        for (int i = 0; i < clients.size(); i++)
        {
            sd = clients[i].get_command_socket();
            if (FD_ISSET(sd, &readfds))
            {
                int read_size = read(sd, client_message, sizeof(client_message));
                if (read_size <= 0)
                {
                    //Somebody disconnected , get his details and print
                    cout << "Host disconnected with command socket " << sd << endl;

                    Client &client = DataBase::ClientManager::get(sd);
                    int client_command_socket = client.get_command_socket();
                    int client_data_socket = client.get_data_socket();
                    close(client_command_socket);
                    close(client_data_socket);
                    DataBase::ClientManager::remove(client_command_socket);
                }
                else
                {
                    client_message[read_size] = '\0';
                    cout << client_message << endl;
                    vector<string> response = CommandHandler::handle(client_message, sd);
                    string command_response = response[0];
                    send(sd, command_response.c_str(), command_response.length(), 0);
                    if (response.size() > 1)
                    {
                        Client &client = DataBase::ClientManager::get(sd);
                        int client_data_socket = client.get_data_socket();
                        string data_response = response[1];
                        if (data_response == "")
                            data_response = " ";
                        send(client_data_socket, data_response.c_str(), data_response.length(), 0);
                    }
                }
            }
        }
    }
    return 0;
}