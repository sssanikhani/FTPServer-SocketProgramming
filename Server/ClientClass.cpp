#ifndef CLIENT_CLASS_H
#define CLIENT_CLASS_H

#include <iostream>
using namespace std;

class Client
{
public:
    Client(string username, string password, int max_size, bool is_admin);
    void clear();

private:
    string username;
    string password;
    int remaining_size;
    bool is_admin;
};


Client::Client(string username, string password, int max_size, bool is_admin) {
    this->username = username;
    this->password = password;
    this->remaining_size = max_size;
    this->is_admin = is_admin;
}











#endif