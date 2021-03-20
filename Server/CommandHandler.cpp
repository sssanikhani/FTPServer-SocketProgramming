#include "CommandHandler.hpp"
#include "DataBase.hpp"
#include "utils.hpp"
#include <string>
#include <unordered_map>
using namespace std;

string CommandHandler::handle(string request, int sd)
{
    
    unordered_map<string, int> commands_map({
        {USER_COMMAND, 0},
        {PASS_COMMAND, 1},
        {PWD_COMMAND, 2},
        {MKD_COMMAND, 3},
        {DELE_COMMAND, 4},
        {LS_COMMAND, 5},
        {CWD_COMMAND, 6},
        {RENAME_COMMAND, 7},
        {RETR_COMMAND, 8},
        {HELP_COMMAND, 9},
        {QUIT_COMMAND, 10}
    });

    string command = split(request)[0];
    if (commands_map.find(command) == commands_map.end()) {
        // Incorrect Command
        return;
    }

    switch (commands_map[command])
    {
    case commands_map[USER_COMMAND]:
        break;
    case commands_map[PASS_COMMAND]:
        break;
    case commands_map[PWD_COMMAND]:
        break;
    case commands_map[MKD_COMMAND]:
        break;
    case commands_map[DELE_COMMAND]:
        break;
    case commands_map[LS_COMMAND]:
        break;
    case commands_map[CWD_COMMAND]:
        break;
    case commands_map[RENAME_COMMAND]:
        break;
    case commands_map[RETR_COMMAND]:
        break;
    case commands_map[HELP_COMMAND]:
        break;
    case commands_map[QUIT_COMMAND]:
        break;
    }
}