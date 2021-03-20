#ifndef USER_H
#define USER_H

#include <string>

class User
{
public:
    User(std::string username, std::string password, int max_size, bool is_admin);
    void clear();

private:
    std::string username;
    std::string password;
    int remaining_size;
    bool is_admin;
};

#endif