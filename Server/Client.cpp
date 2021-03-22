#include "Client.hpp"
using namespace std;

Client::Client(int command_sd, int data_sd)
{
    this->command_socket = command_sd;
    this->data_socket = data_sd;
    this->bound_to_user = false;
    this->authenticated = false;
    this->current_path = "./";
}

Client::Client(Client &c)
{
    this->command_socket = c.command_socket;
    this->data_socket = c.data_socket;
    this->username = c.username;
    this->bound_to_user = c.bound_to_user;
    this->authenticated = c.authenticated;
    this->current_path = c.current_path;
}

string Client::bind_to_user(string username)
{
    this->username = username;
    this->bound_to_user = true;
    return this->username;
}