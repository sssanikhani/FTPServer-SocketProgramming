#ifndef USER_H
#define USER_H

#include <string>

class User
{
public:
    User(std::string _username, std::string _password, int max_size, bool _is_admin);
    User();
    User(User& u);
    std::string get_username() { return this->username; }
    int get_remaining_size() { return this->remaining_size; }
    bool is_administrator() { return this->is_admin; }
    void set_username(std::string _username) { this->username = _username; }
    void set_password(std::string _password) { this->password = _password; }
    bool is_valid_password(std::string _password) { return this->password == _password; }

private:
    std::string username;
    std::string password;
    int remaining_size;
    bool is_admin;
};

#endif