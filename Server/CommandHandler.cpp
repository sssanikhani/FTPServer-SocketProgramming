#include "CommandHandler.hpp"
#include "DataBase.hpp"
#include "../Utils/utils.hpp"
#include "Responses.hpp"
#include <string>
#include <unordered_map>
using namespace std;

bool is_logged_in(int sd)
{
    if (!DataBase::ClientManager::exists(sd))
        return false;
    
    Client& client = DataBase::ClientManager::get(sd);
    if (!client.is_authenticated())
        return false;
    
    return true;
}


string CommandHandler::handle(string request, int command_sd, int data_sd)
{
    if (split(request).size() < 1)
        return incorrect();

    string command = split(request)[0];

    if (command == USER_COMMAND)
        return user(request, command_sd, data_sd);
    else if (command == PASS_COMMAND)
        return pass(request, command_sd, data_sd);
    else if (command == PWD_COMMAND)
        return pwd(request, command_sd, data_sd);
    else if (command == MKD_COMMAND)
        return mkd(request, command_sd, data_sd);
    else if (command == DELE_COMMAND)
        return dele(request, command_sd, data_sd);
    else if (command == LS_COMMAND)
        return ls(request, command_sd, data_sd);
    else if (command == CWD_COMMAND)
        return cwd(request, command_sd, data_sd);
    else if (command == RENAME_COMMAND)
        return rename(request, command_sd, data_sd);
    else if (command == RETR_COMMAND)
        return retr(request, command_sd, data_sd);
    else if (command == HELP_COMMAND)
        return help(request, command_sd, data_sd);
    else if (command == QUIT_COMMAND)
        return quit(request, command_sd, data_sd);
    else
        return incorrect();
}

string CommandHandler::user(string request, int command_sd, int data_sd)
{
    // user <username>
    vector<string> request_parts = split(request);
    if (request_parts.size() < 2) return incorrect();
    string username = request_parts[1];

    if (!DataBase::UserManager::exists(username))
        return Responses::INVALID_USER_PASS;

    if (!DataBase::ClientManager::exists(command_sd)) {
        Client c(command_sd, data_sd);
        DataBase::ClientManager::add(c);
    }

    Client& client = DataBase::ClientManager::get(command_sd);
    client.bind_to_user(username);

    return Responses::USER_OKAY;
}

string CommandHandler::pass(string request, int command_sd, int data_sd)
{
    // pass <password>
    vector<string> request_parts = split(request);
    if (request_parts.size() < 2) return incorrect();
    string password = request_parts[1];

    if (!DataBase::ClientManager::exists(command_sd))
        return Responses::BAD_SEQUENCE;

    Client& client = DataBase::ClientManager::get(command_sd);

    if (!client.is_bound_to_user())
        return Responses::BAD_SEQUENCE;

    string username = client.get_username();
    User& user = DataBase::UserManager::get(username);
    if (!user.is_valid_password(password))
        return Responses::INVALID_USER_PASS;
    
    client.authenticate();
    return Responses::CORRECT_PASS;
}

string CommandHandler::pwd(string request, int command_sd, int data_sd)
{
    // pwd
    if (!is_logged_in(command_sd))
        return Responses::NEED_LOGIN;

    // TODO: pwd command
}

string CommandHandler::mkd(string request, int command_sd, int data_sd)
{
    // mkd <path>
    if (!is_logged_in(command_sd))
        return Responses::NEED_LOGIN;
    // TODO: mkd command
}

string CommandHandler::dele(string request, int command_sd, int data_sd)
{
    // dele -f <file_path>
    // dele -d <directory_path>
    if (!is_logged_in(command_sd))
        return Responses::NEED_LOGIN;
    // TODO: dele command
}

string CommandHandler::ls(string request, int command_sd, int data_sd)
{
    // ls
    if (!is_logged_in(command_sd))
        return Responses::NEED_LOGIN;
    // TODO: ls command
}

string CommandHandler::cwd(string request, int command_sd, int data_sd)
{
    // cwd <path>
    // cwd ..
    // cwd
    if (!is_logged_in(command_sd))
        return Responses::NEED_LOGIN;
    // TODO: cwd command
}

string CommandHandler::rename(string request, int command_sd, int data_sd)
{
    // rename <from> <to>
    if (!is_logged_in(command_sd))
        return Responses::NEED_LOGIN;
    // TODO: rename command
}

string CommandHandler::retr(string request, int command_sd, int data_sd)
{
    // retr <name>
    if (!is_logged_in(command_sd))
        return Responses::NEED_LOGIN;
    // TODO: retr command
}

string CommandHandler::help(string request, int command_sd, int data_sd)
{
    // help
    // TODO: help command
}

string CommandHandler::quit(string request, int command_sd, int data_sd)
{
    // quit
    // TODO: quit command
}

string CommandHandler::incorrect()
{
    // <!incorrect command>
    // TODO: incorrect command
}