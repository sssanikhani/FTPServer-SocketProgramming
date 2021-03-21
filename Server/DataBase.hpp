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
        static User& get(std::string username);
        static User& add(User user);
        static std::vector<User*> get_all();
        static bool exists(std::string username);

    private:
        static std::unordered_map<std::string, User> users;
    };

    class ClientManager
    {
    public:
        static int bind_socket_to_user(std::string username, int sd);
        static int authenticate(string username, int sd);
        static bool is_authenticated(int sd);
        static bool is_bound(int sd);
        static std::string get_username(int sd);
        static void logout(int sd);

    private:
        static std::unordered_map<int, std::string> socket_username_map;
        static std::unordered_map<int, bool> socket_authentication;
    };
};

std::unordered_map<std::string, User> users;
std::unordered_map<int, std::string> socket_username_map;
std::unordered_map<int, bool> socket_authentication;

#endif