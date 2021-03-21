#include "DataBase.hpp"
#include "User.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <unistd.h>
using namespace std;

User &DataBase::UserManager::get(string username)
{
    if (exists(username))
        return users[username];
    return;
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

int DataBase::ClientManager::bind_socket_to_user(string username, int sd)
{
    socket_username_map[sd] = username;
    socket_authentication[sd] = false;
    return sd;
}

int DataBase::ClientManager::authenticate(string username, int sd)
{
    bind_socket_to_user(username, sd);
    socket_authentication[sd] = true;
    return sd;
}

bool DataBase::ClientManager::is_authenticated(int sd)
{
    if (socket_authentication.find(sd) == socket_authentication.end())
        return false;

    return socket_authentication[sd];
}

bool DataBase::ClientManager::is_bound(int sd)
{
    if (socket_username_map.find(sd) == socket_username_map.end())
        return false;
    return true;
}

string DataBase::ClientManager::get_username(int sd)
{
    if (!is_bound(sd))
        return NULL;
    return socket_username_map[sd];
}

void DataBase::ClientManager::logout(int sd)
{
    if (socket_authentication.find(sd) != socket_authentication.end())
        socket_authentication.erase(sd);

    if (socket_username_map.find(sd) != socket_username_map.end())
        socket_username_map.erase(sd);
}
