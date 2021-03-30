#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <string>
#include <vector>

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

extern const std::string INIT_PATH;
extern const std::string LOG_FILE_PATH;

class CommandHandler
{
public:
    static std::vector<std::string> handle(std::string request, int command_sd);

private:
    static std::vector<std::string> user(std::string request, int command_sd);
    static std::vector<std::string> pass(std::string request, int command_sd);
    static std::vector<std::string> pwd(std::string request, int command_sd);
    static std::vector<std::string> mkd(std::string request, int command_sd);
    static std::vector<std::string> dele(std::string request, int command_sd);
    static std::vector<std::string> ls(std::string request, int command_sd);
    static std::vector<std::string> cwd(std::string request, int command_sd);
    static std::vector<std::string> rename(std::string request, int command_sd);
    static std::vector<std::string> retr(std::string request, int command_sd);
    static std::vector<std::string> help(std::string request, int command_sd);
    static std::vector<std::string> quit(std::string request, int command_sd);
    static std::vector<std::string> incorrect();
};

#endif