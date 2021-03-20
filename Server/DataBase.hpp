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
        static User add(User user);
        static User remove(std::string username);
        static std::vector<User> get_all();

    private:
        static std::unordered_map<std::string, User> users;
    };

    class ClientManager
    {
    public:
        static int bind_socket_to_user(string username, int sd);
        static int authenticate(string username, int sd);
        static bool is_authenticated(int sd);
        static User get_user(int sd);
        static int logout();

    private:
        static std::unordered_map<int, User> socket_user_map;
        static std::unordered_map<int, bool> socket_authentication;
    };
};

std::unordered_map<std::string, User> users;
std::unordered_map<int, User> socket_user_map;
std::unordered_map<int, bool> socket_authentication;

#endif