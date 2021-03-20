#include "User.hpp"
#include <iostream>
using namespace std;

User::User(string username, string password, int max_size, bool is_admin)
{
    this->username = username;
    this->password = password;
    this->remaining_size = max_size;
    this->is_admin = is_admin;
}
