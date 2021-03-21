#include "CommandHandler.hpp"
#include "DataBase.hpp"
#include "utils.hpp"
#include <string>
#include <unordered_map>
using namespace std;

string CommandHandler::handle(string request, int sd)
{
    string command = split(request)[0];

    if (command == USER_COMMAND)
        user(request, sd);
    else if (command == PASS_COMMAND)
        pass(request, sd);
    else if (command == PWD_COMMAND)
        pwd(request, sd);
    else if (command == MKD_COMMAND)
        mkd(request, sd);
    else if (command == DELE_COMMAND)
        dele(request, sd);
    else if (command == LS_COMMAND)
        ls(request, sd);
    else if (command == CWD_COMMAND)
        cwd(request, sd);
    else if (command == RENAME_COMMAND)
        rename(request, sd);
    else if (command == RETR_COMMAND)
        retr(request, sd);
    else if (command == HELP_COMMAND)
        help(request, sd);
    else if (command == QUIT_COMMAND)
        quit(request, sd);
    else
        incorrect(request, sd);
}

string CommandHandler::user(string request, int sd)
{
    // user <username>
    // TODO: user command
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

string CommandHandler::incorrect(string request, int sd)
{
    // <!incorrect command>
    // TODO: incorrect command
}