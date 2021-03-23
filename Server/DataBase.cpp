#include "DataBase.hpp"
#include "User.hpp"
#include "Client.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <unistd.h>
using namespace std;

unordered_map<string, User> DataBase::UserManager::users;
unordered_map<int, Client> DataBase::ClientManager::clients;
unordered_map<string, bool> DataBase::PrivilegeFiles::files;

User &DataBase::UserManager::get(string username)
{
    return users[username];
}

User &DataBase::UserManager::add(User user)
{
    users[user.get_username()] = user;
    return users[user.get_username()];
}

bool DataBase::UserManager::exists(string username)
{
    return users.find(username) != users.end();
}

vector<User *> DataBase::UserManager::get_all()
{
    vector<User *> res;
    for (auto it = users.begin(); it != users.end(); it++)
    {
        res.push_back(&(it->second));
    }
    return res;
}

Client &DataBase::ClientManager::get(int sd)
{
    return clients[sd];
}

Client &DataBase::ClientManager::add(Client client)
{
    int sd = client.get_command_socket();
    clients[sd] = client;
    return clients[sd];
}

bool DataBase::ClientManager::exists(int sd)
{
    return clients.find(sd) != clients.end();
}

void DataBase::ClientManager::remove(int sd)
{
    clients.erase(sd);
}

void DataBase::PrivilegeFiles::change_name(string from, string to)
{
    if (!exists(from))
        return;
    
    files.erase(from);
    files[to] = true;
}