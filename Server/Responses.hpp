#ifndef RESPONSES_H
#define RESPONSES_H

#include <string>

class Responses
{
public:
    static const std::string USER_OKAY;
    static const std::string CORRECT_PASS;
    static const std::string BAD_SEQUENCE;
    static const std::string INVALID_USER_PASS;
    static const std::string FILE_UNAVAILABLE;
    static const std::string PWD_CODE;
    static const std::string MKD_CODE;
    static const std::string DELE_CODE;
    static const std::string LIST_TRANSFER;
    static const std::string SUCCESSFUL_CHANGE;
    static const std::string SUCCESSFUL_DOWNLOAD;
    static const std::string SUCCESSFUL_QUIT;
    static const std::string NEED_LOGIN;
    static const std::string PARAMS_ERROR;
    static const std::string ERROR;
    static const std::string NOT_ENOUGH_SIZE;
};

const std::string Responses::USER_OKAY = "331: User name okay, need password.";
const std::string Responses::CORRECT_PASS = "230: User logged in, proceed. Logged out if appropriate.";
const std::string Responses::BAD_SEQUENCE = "503: Bad sequence of commands.";
const std::string Responses::INVALID_USER_PASS = "430: Invalid username or password";
const std::string Responses::FILE_UNAVAILABLE = "550: File unavailable.";
const std::string Responses::PWD_CODE = "257: ";
const std::string Responses::MKD_CODE = "257: ";
const std::string Responses::DELE_CODE = "250: ";
const std::string Responses::LIST_TRANSFER = "226: List transfer done.";
const std::string Responses::SUCCESSFUL_CHANGE = "250: Successful change.";
const std::string Responses::SUCCESSFUL_DOWNLOAD = "226: Successful Download.";
const std::string Responses::SUCCESSFUL_QUIT = "221: Successful Quit.";
const std::string Responses::NEED_LOGIN = "332: Need account for login.";
const std::string Responses::PARAMS_ERROR = "501: Syntax error in parameters or arguments.";
const std::string Responses::ERROR = "500: Error";
const std::string Responses::NOT_ENOUGH_SIZE = "425: Can't open data connection.";

#endif