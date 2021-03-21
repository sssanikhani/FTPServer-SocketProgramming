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
    {
        // user <username>
        // TODO: USER_COMMAND
    }
    else if (command == PASS_COMMAND)
    {
        // pass <password>
        // TODO: PASS_COMMAND
    }
    else if (command == PWD_COMMAND)
    {
        // pwd
        // TODO: PWD_COMMAND
    }
    else if (command == MKD_COMMAND)
    {
        // mkd <path>
        // TODO: MKD_COMMAND
    }
    else if (command == DELE_COMMAND)
    {
        // dele -f <file_path>
        // dele -d <directory_path>
        // TODO: DELE_COMMAND
    }
    else if (command == LS_COMMAND)
    {
        // ls
        // TODO: LS_COMMAND
    }
    else if (command == CWD_COMMAND)
    {
        // cwd <path>
        // TODO: CWD_COMMAND
    }
    else if (command == RENAME_COMMAND)
    {
        // rename <from> <to>
        // TODO: RENAME_COMMAND
    }
    else if (command == RETR_COMMAND)
    {
        // retr <name>
        // TODO: RETR_COMMAND
    }
    else if (command == HELP_COMMAND)
    {
        // help
        // TODO: HELP_COMMAND
    }
    else if (command == QUIT_COMMAND)
    {
        // quit 
        // TODO: QUIT_COMMAND
    }
    else
    {
        // Incorrect Command
        // TODO: Incorrect Command
    }
}