#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client
{
public:
    Client() {}
    Client(int command_sd, int data_sd, std::string _current_path);
    Client(const Client &c);

    int get_command_socket() { return this->command_socket; }
    int get_data_socket() { return this->data_socket; }
    std::string get_username() { return this->username; }
    bool is_bound_to_user() { return this->bound_to_user; }
    std::string get_current_path() { return this->current_path; }
    bool is_authenticated() { return authenticated; }

    int set_command_socket(int sd) { return this->command_socket = sd; }
    int set_data_socket(int sd) { return this->data_socket = sd; }
    std::string bind_to_user(std::string username);
    std::string set_current_path(std::string path) { return this->current_path = path; }
    bool authenticate() { return this->authenticated = true; }
    bool logout();

private:
    int command_socket;
    int data_socket;
    std::string username;
    bool bound_to_user;
    std::string current_path;
    bool authenticated;
};

#endif