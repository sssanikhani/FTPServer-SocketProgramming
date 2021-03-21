#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <string>

#define USER_COMMAND "user"
#define PASS_COMMAND "pass"
#define PWD_COMMAND "pwd"
#define MKD_COMMAND "mkd"
#define DELE_COMMAND "dele"
#define LS_COMMAND "ls"
#define CWD_COMMAND "cwd"
#define RENAME_COMMAND "rename"
#define RETR_COMMAND "retr"
#define HELP_COMMAND "help"
#define QUIT_COMMAND "quit"

class CommandHandler
{
public:
    static std::string handle(std::string request, int sd);
private:
    static std::string user(std::string request, int sd);
    static std::string pass(std::string request, int sd);
    static std::string pwd(std::string request, int sd);
    static std::string mkd(std::string request, int sd);
    static std::string dele(std::string request, int sd);
    static std::string ls(std::string request, int sd);
    static std::string cwd(std::string request, int sd);
    static std::string rename(std::string request, int sd);
    static std::string retr(std::string request, int sd);
    static std::string help(std::string request, int sd);
    static std::string quit(std::string request, int sd);
    static std::string incorrect();


};

#endif