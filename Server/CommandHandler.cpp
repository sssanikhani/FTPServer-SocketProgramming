#include "CommandHandler.hpp"
#include "DataBase.hpp"
#include "utils.hpp"
#include "Responses.hpp"
#include <string>
#include <unordered_map>
using namespace std;

string CommandHandler::handle(string request, int sd)
{
    if (split(request).size() < 1)
        return incorrect();

    string command = split(request)[0];

    if (command == USER_COMMAND)
        return user(request, sd);
    else if (command == PASS_COMMAND)
        return pass(request, sd);
    else if (command == PWD_COMMAND)
        return pwd(request, sd);
    else if (command == MKD_COMMAND)
        return mkd(request, sd);
    else if (command == DELE_COMMAND)
        return dele(request, sd);
    else if (command == LS_COMMAND)
        return ls(request, sd);
    else if (command == CWD_COMMAND)
        return cwd(request, sd);
    else if (command == RENAME_COMMAND)
        return rename(request, sd);
    else if (command == RETR_COMMAND)
        return retr(request, sd);
    else if (command == HELP_COMMAND)
        return help(request, sd);
    else if (command == QUIT_COMMAND)
        return quit(request, sd);
    else
        return incorrect();
}

string CommandHandler::user(string request, int sd)
{
    // user <username>
    if (split(request).size() < 2) return incorrect();
    string username = split(request)[1];

    if (!DataBase::UserManager::exists(username))
        return Responses::INVALID_USER_PASS;

    DataBase::ClientManager::bind_socket_to_user(username, sd);
    return Responses::USER_OKAY;

}

string CommandHandler::pass(string request, int sd)
{
    // pass <password>
    // TODO: pass command
}

string CommandHandler::pwd(string request, int sd)
{
    // pwd
    // TODO: pwd command
}

string CommandHandler::mkd(string request, int sd)
{
    // mkd <path>
    // TODO: mkd command
}

string CommandHandler::dele(string request, int sd)
{
    // dele -f <file_path>
    // dele -d <directory_path>
    // TODO: dele command
}

string CommandHandler::ls(string request, int sd)
{
    // ls
    // TODO: ls command
}

string CommandHandler::cwd(string request, int sd)
{
    // cwd <path>
    // cwd ..
    // cwd
    // TODO: cwd command
}

string CommandHandler::rename(string request, int sd)
{
    // rename <from> <to>
    // TODO: rename command
}

string CommandHandler::retr(string request, int sd)
{
    // retr <name>
    // TODO: retr command
}

string CommandHandler::help(string request, int sd)
{
    // help
    // TODO: help command
}

string CommandHandler::quit(string request, int sd)
{
    // quit
    // TODO: quit command
}

string CommandHandler::incorrect()
{
    // <!incorrect command>
    // TODO: incorrect command
}