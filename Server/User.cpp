#include "User.hpp"
#include <iostream>
using namespace std;

User::User(string _username, string _password, int max_size, bool _is_admin)
{
    this->username = _username;
    this->password = _password;
    this->remaining_size = max_size;
    this->is_admin = _is_admin;
}
