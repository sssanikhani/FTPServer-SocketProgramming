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
    static const std::string HELP;
    static const std::string SUCCESSFUL_QUIT;
    static const std::string NEED_LOGIN;
    static const std::string PARAMS_ERROR;
    static const std::string ERROR;
    static const std::string NOT_ENOUGH_SIZE;
};

#endif