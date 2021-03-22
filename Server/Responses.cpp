#include "Responses.hpp"
#include <string>
using namespace std;

const string Responses::USER_OKAY = "331: User name okay, need password.";
const string Responses::CORRECT_PASS = "230: User logged in, proceed. Logged out if appropriate.";
const string Responses::BAD_SEQUENCE = "503: Bad sequence of commands.";
const string Responses::INVALID_USER_PASS = "430: Invalid username or password";
const string Responses::FILE_UNAVAILABLE = "550: File unavailable.";
const string Responses::PWD_CODE = "257: ";
const string Responses::MKD_CODE = "257: ";
const string Responses::DELE_CODE = "250: ";
const string Responses::LIST_TRANSFER = "226: List transfer done.";
const string Responses::SUCCESSFUL_CHANGE = "250: Successful change.";
const string Responses::SUCCESSFUL_DOWNLOAD = "226: Successful Download.";
const string Responses::SUCCESSFUL_QUIT = "221: Successful Quit.";
const string Responses::NEED_LOGIN = "332: Need account for login.";
const string Responses::PARAMS_ERROR = "501: Syntax error in parameters or arguments.";
const string Responses::ERROR = "500: Error";
const string Responses::NOT_ENOUGH_SIZE = "425: Can't open data connection.";    
const string Responses::HELP = 
    string("214\n")
    + "user [name], Its argument is used to specify the user's string. It's used for user authentication.\n"
    + "pass [password], Its argument is used to specify the user's password. It's used for user authentication.\n"
    + "pwd, It's used to specify current directory user is in.\n"
    + "mkd [dirname], It's used to make directory with provided name dirname.\n"
    + "dele [-f, -d] [path], It's used to delete a file or directory with specified path. [-d] specifies directory and [-f] specifies file.\n"
    + "ls, It's used to show list of files in current directory.\n"
    + "cwd [Optional: path], It's used to change current directory to provided path. If not provided path, current directory will become home.\n"
    + "rename [from] [to], Its arguments respectively are used to specify current file name and wanted file name. This command renames specified file.\n"
    + "retr [path], Its argument is used to specify wanted file. It's used to download specified file.\n"
    + "help, It's used to show this mannual.\n"
    + "quit, It's used to quit user from server and logout.";