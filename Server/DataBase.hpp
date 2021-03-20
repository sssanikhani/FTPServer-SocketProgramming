#ifndef DATABASE_H
#define DATABASE_H

#include <unordered_map>
#include <string>
#include <vector>
#include "User.hpp"

class DataBase
{
public:
    class UserManager
    {
    public:
        static User get(std::string username);
        static User put(User user);
        static User remove(std::string username);
        static std::vector<User> get_all();

    private:
        static std::unordered_map<std::string, User> users;
    };
};

std::unordered_map<std::string, User> users;

#endif