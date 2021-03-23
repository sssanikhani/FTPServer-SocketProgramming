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

#define PORT 3000
#define MAX_CLIENTS 20
#define TRUE 1
#define FALSE -1
#define SPACE ' '
#define ZERO 48
#define Z 122
#define USER "user"
#define PASSWORD "pass"

using json = nlohmann::json;
namespace fs = std::filesystem;
using namespace std;

const string init_path = fs::absolute(fs::current_path()).string();

// bool login_user(vector<vector<string>> &clients, vector<vector<string>> enrolled_users, int client_id)
// {
//     for (int i = 0; i < enrolled_users.size(); i++)
//     {
//     }
//     return false;
// }

// int check_username(vector<vector<string>> enrolled_users, string username)
// {
//     int user_row;
//     for (int i = 0; i < enrolled_users.size(); i++)
//     {
//         cout << enrolled_users[i][0] << endl;
//         if (username == enrolled_users[i][0])
//             return i;
//     }
//     return -1;
// }

// bool check_password(vector<vector<string>> enrolled_users, string password, int user_row)
// {
//     if (enrolled_users[user_row][1] == password)
//         return true;
//     else
//         return false;
// }

// bool check_username_password(vector<vector<string>> &clients, vector<vector<string>> enrolled_users, int client_id)
// {
//     for (int i = 0; i < enrolled_users.size(); i++)
//     {
//         if (clients[client_id][1] == enrolled_users[i][0] && clients[client_id][2] == enrolled_users[i][1])
//             return true;
//     }
//     return false;
// }

// string get_command(string input)
// {
//     vector<string> input_parts = split(input);
//     return input_parts[0];
// }

// string get_value(string input)
// {
//     vector<string> input_parts = split(input);
//     return input_parts[1];
// }

// void handle_input(string input, vector<vector<string>> &clients, vector<vector<string>> enrolled_users, int client_id, int client_sd)
// {
//     string message;
//     int user_row = 0;
//     string command;
//     string value;
//     int pos = 0;

//     cout << input << endl;

//     //Enter fd
//     if (clients[client_id][0] == "")
//         clients[client_id][0] = to_string(client_sd);

//     command = get_command(input);
//     if (command == USER)
//     {
//         message = "331: User name okay, need password.\n";
//         value = get_value(input);
//         clients[client_id][1] = value;
//     }
//     else if (command == PASSWORD)
//     {
//         value = get_value(input);
//         clients[client_id][2] = value;
//         if (check_username_password(clients, enrolled_users, client_id))
//         {
//             message = "230: User logged in, proceed. Logged out if appropriate.\n";
//         }
//         else
//         {
//             message = "430: Invalid username or password.\n";
//             clients[client_id][1] = "";
//             clients[client_id][2] = "";
//         }
//     }
//     else
//     {
//         message = "501: Syntax error in parameters or arguments.\n";
//     }
//     send(client_sd, message.c_str(), message.length(), 0);
//     cout << clients[client_id][0] << "  " << clients[client_id][1] << "  " << clients[client_id][2] << endl;
// }

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
    command_server.sin_port = htons(PORT);
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
    data_server.sin_port = htons(8001);      // TODO read port from config.json
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

    while (TRUE)
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

            string message = "You are now connected \nEnter your Username:";
            if (send(command_sd, message.c_str(), message.length(), 0) != message.length())
            {
                perror("send");
            }

            cout << "Welcome message sent successfully" << endl;
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
                    if (response.size() > 1) {
                        Client &client = DataBase::ClientManager::get(sd);
                        int client_data_socket = client.get_data_socket();
                        string data_response = response[1];
                        send(client_data_socket, data_response.c_str(), data_response.length(), 0);
                    }
                }
            }
        }
    }
    return 0;
}