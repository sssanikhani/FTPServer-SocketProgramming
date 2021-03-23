#ifndef DATABASE_H
#define DATABASE_H

#include <unordered_map>
#include <string>
#include <vector>
#include "User.hpp"
#include "Client.hpp"

class DataBase
{
public:
    class UserManager
    {
    public:
        static User &get(std::string username);
        static User &add(User user);
        static std::vector<User *> get_all();
        static bool exists(std::string username);

    private:
        static std::unordered_map<std::string, User> users;
    };

    class ClientManager
    {
    public:
        static Client &get(int sd);
        static Client &add(Client client);
        static bool exists(int sd);
        static void remove(int sd);

    private:
        static std::unordered_map<int, Client> clients;
    };

    class PrivilegeFiles
    {
    public:
        static bool exists(std::string file_name) { return files.find(file_name) != files.end(); }
        static void add(std::string file_name) { files[file_name] = true; }
        static void change_name(std::string from, std::string to);
    private:
        static std::unordered_map<std::string, bool> files;
    };
};

#endif