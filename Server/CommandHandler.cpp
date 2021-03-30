#include "CommandHandler.hpp"
#include "DataBase.hpp"
#include "../Utils/utils.hpp"
#include "Responses.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;
using namespace std;

bool is_logged_in(int sd)
{
    if (!DataBase::ClientManager::exists(sd))
        return false;

    Client &client = DataBase::ClientManager::get(sd);
    if (!client.is_authenticated())
        return false;

    return true;
}

vector<string> CommandHandler::handle(string request, int command_sd)
{
    vector<string> request_parts = split(request);
    if (request_parts.size() < 1)
        return incorrect();

    string command = request_parts[0];

    if (command == USER_COMMAND)
        return user(request, command_sd);
    else if (command == PASS_COMMAND)
        return pass(request, command_sd);
    else if (command == PWD_COMMAND)
        return pwd(request, command_sd);
    else if (command == MKD_COMMAND)
        return mkd(request, command_sd);
    else if (command == DELE_COMMAND)
        return dele(request, command_sd);
    else if (command == LS_COMMAND)
        return ls(request, command_sd);
    else if (command == CWD_COMMAND)
        return cwd(request, command_sd);
    else if (command == RENAME_COMMAND)
        return rename(request, command_sd);
    else if (command == RETR_COMMAND)
        return retr(request, command_sd);
    else if (command == HELP_COMMAND)
        return help(request, command_sd);
    else if (command == QUIT_COMMAND)
        return quit(request, command_sd);
    else
        return incorrect();
}

vector<string> CommandHandler::user(string request, int command_sd)
{
    // user <username>
    vector<string> request_parts = split(request);
    if (request_parts.size() != 2)
        return incorrect();
    string username = request_parts[1];

    if (!DataBase::UserManager::exists(username))
        return vector<string>{Responses::INVALID_USER_PASS};

    if (!DataBase::ClientManager::exists(command_sd))
        return vector<string>{Responses::ERROR};

    Client &client = DataBase::ClientManager::get(command_sd);
    client.bind_to_user(username);

    return vector<string>{Responses::USER_OKAY};
}

vector<string> CommandHandler::pass(string request, int command_sd)
{
    // pass <password>
    vector<string> request_parts = split(request);
    if (request_parts.size() != 2)
        return incorrect();
    string password = request_parts[1];

    if (!DataBase::ClientManager::exists(command_sd))
        return vector<string>{Responses::BAD_SEQUENCE};

    Client &client = DataBase::ClientManager::get(command_sd);

    if (!client.is_bound_to_user())
        return vector<string>{Responses::BAD_SEQUENCE};

    string username = client.get_username();
    User &user = DataBase::UserManager::get(username);
    if (!user.is_valid_password(password))
        return vector<string>{Responses::INVALID_USER_PASS};

    client.authenticate();
    return vector<string>{Responses::CORRECT_PASS};
}

vector<string> CommandHandler::pwd(string request, int command_sd)
{
    // pwd
    vector<string> request_parts = split(request);
    if (request_parts.size() != 1)
        return incorrect();

    if (!is_logged_in(command_sd))
        return vector<string>{Responses::NEED_LOGIN};

    Client &client = DataBase::ClientManager::get(command_sd);
    return vector<string>{Responses::PWD_CODE + client.get_current_path()};
}

vector<string> CommandHandler::mkd(string request, int command_sd)
{
    // mkd <path>
    vector<string> request_parts = split(request);
    if (request_parts.size() != 2)
        return incorrect();

    if (!is_logged_in(command_sd))
        return vector<string>{Responses::NEED_LOGIN};

    fs::path requested_path = request_parts[1];

    Client &client = DataBase::ClientManager::get(command_sd);
    fs::path current_path = client.get_current_path();

    fs::path final_path = current_path / requested_path;

    error_code ec;
    if (fs::create_directory(final_path, ec))
        return vector<string>{Responses::MKD_CODE + fs::canonical(final_path).string() + " created."};
    return vector<string>{Responses::ERROR};
}

vector<string> CommandHandler::dele(string request, int command_sd)
{
    // dele -f <file_path>
    // dele -d <directory_path>
    vector<string> request_parts = split(request);
    if (request_parts.size() != 3)
        return incorrect();

    if (!is_logged_in(command_sd))
        return vector<string>{Responses::NEED_LOGIN};

    Client &client = DataBase::ClientManager::get(command_sd);
    fs::path current_path = client.get_current_path();

    string flag = request_parts[1];
    if (flag != "-f" && flag != "-d")
        return incorrect();

    fs::path requested_path = request_parts[2];
    fs::path final_path = current_path / requested_path;

    error_code exists_ec;
    if (!fs::exists(final_path, exists_ec))
        return vector<string>{Responses::ERROR};

    error_code ec;
    if (flag == "-f" && fs::is_directory(final_path, ec))
        return vector<string>{Responses::ERROR};

    cout << final_path << endl;
    if (flag == "-d" && !fs::is_directory(final_path, ec))
        return vector<string>{Responses::ERROR};

    fs::path relative_to_init_path = fs::canonical(fs::relative(final_path, init_path));
    cout << init_path << endl;
    string username = client.get_username();
    User &user = DataBase::UserManager::get(username);
    if (!user.is_administrator() && DataBase::PrivilegeFiles::exists(relative_to_init_path))
        return vector<string>{Responses::FILE_UNAVAILABLE};

    uintmax_t num;
    error_code remove_ec;
    if ((num = fs::remove_all(final_path, remove_ec)) > 0)
        return vector<string>{Responses::DELE_CODE + relative_to_init_path.string() + " deleted."};

    return vector<string>{Responses::ERROR};
}

vector<string> CommandHandler::ls(string request, int command_sd)
{
    // ls
    vector<string> request_parts = split(request);
    if (request_parts.size() != 1)
        return incorrect();

    if (!is_logged_in(command_sd))
        return vector<string>{Responses::NEED_LOGIN};

    Client &client = DataBase::ClientManager::get(command_sd);
    fs::path current_path = client.get_current_path();

    vector<string> data_vec;
    for (const auto &entry : fs::directory_iterator(current_path))
        data_vec.push_back(fs::relative(entry.path(), current_path).string());

    string data = join(data_vec, ' ');
    data = "./ ../ " + data;
    return vector<string>{Responses::LIST_TRANSFER, data};
}

vector<string> CommandHandler::cwd(string request, int command_sd)
{
    // cwd <path>
    // cwd
    vector<string> request_parts = split(request);
    if (request_parts.size() != 1 && request_parts.size() != 2)
        return incorrect();

    if (!is_logged_in(command_sd))
        return vector<string>{Responses::NEED_LOGIN};

    Client &client = DataBase::ClientManager::get(command_sd);
    fs::path current_path = client.get_current_path();

    fs::path final_path;
    if (request_parts.size() == 1)
        final_path = init_path;
    else
    {
        fs::path requested_path = request_parts[1];
        final_path = fs::absolute(current_path / requested_path);
    }

    error_code ec;
    if (!fs::exists(final_path, ec) || !fs::is_directory(final_path, ec))
        return vector<string>{Responses::ERROR};

    client.set_current_path(fs::canonical(final_path).string());
    return vector<string>{Responses::SUCCESSFUL_CHANGE};
}

vector<string> CommandHandler::rename(string request, int command_sd)
{
    // rename <from> <to>
    vector<string> request_parts = split(request);
    if (request_parts.size() != 3)
        return incorrect();

    if (!is_logged_in(command_sd))
        return vector<string>{Responses::NEED_LOGIN};

    Client &client = DataBase::ClientManager::get(command_sd);
    fs::path current_path = client.get_current_path();

    fs::path from = request_parts[1];
    fs::path to = request_parts[2];

    fs::path abs_from = fs::absolute(current_path / from);
    fs::path abs_to = fs::absolute(current_path / to);

    error_code ec;
    if (!fs::exists(abs_from, ec))
        return vector<string>{Responses::ERROR};
    fs::path relative_to_init_path = fs::canonical(fs::relative(abs_from, init_path));

    string username = client.get_username();
    User &user = DataBase::UserManager::get(username);
    if (!user.is_administrator() && DataBase::PrivilegeFiles::exists(relative_to_init_path))
        return vector<string>{Responses::FILE_UNAVAILABLE};

    fs::rename(abs_from, abs_to, ec);

    if (ec)
        return vector<string>{Responses::ERROR};

    return vector<string>{Responses::SUCCESSFUL_CHANGE};
}

vector<string> CommandHandler::retr(string request, int command_sd)
{
    // retr <name>
    vector<string> request_parts = split(request);
    if (request_parts.size() != 2)
        return incorrect();

    if (!is_logged_in(command_sd))
        return vector<string>{Responses::NEED_LOGIN};

    Client &client = DataBase::ClientManager::get(command_sd);
    fs::path current_path = client.get_current_path();

    fs::path filename = request_parts[1];
    fs::path file_path = current_path / filename;

    error_code ec;
    if (!fs::exists(file_path, ec) || !fs::is_regular_file(file_path, ec))
        return vector<string>{Responses::ERROR};

    fs::path relative_to_init_path = fs::canonical(fs::relative(file_path, init_path));

    string username = client.get_username();
    User &user = DataBase::UserManager::get(username);
    if (!user.is_administrator() && DataBase::PrivilegeFiles::exists(relative_to_init_path))
        return vector<string>{Responses::FILE_UNAVAILABLE};
    
    uintmax_t file_size = fs::file_size(file_path, ec);
    int user_remaining_size = user.get_remaining_size();
    int new_remaining_size = user_remaining_size - file_size;
    if (new_remaining_size < 0)
        return vector<string>{Responses::NOT_ENOUGH_SIZE};
    cout << user_remaining_size << endl;
    cout << file_size << endl;
    user.set_remaining_size(new_remaining_size);
    string data = file_to_string(relative_to_init_path.string());

    return vector<string>{Responses::SUCCESSFUL_DOWNLOAD, data};
}

vector<string> CommandHandler::help(string request, int command_sd)
{
    // help
    return vector<string>{Responses::HELP};
}

vector<string> CommandHandler::quit(string request, int command_sd)
{
    // quit
    if (!DataBase::ClientManager::exists(command_sd))
        return vector<string>{Responses::ERROR};
    Client &client = DataBase::ClientManager::get(command_sd);
    client.logout();

    return vector<string>{Responses::SUCCESSFUL_QUIT};
}

vector<string> CommandHandler::incorrect()
{
    // <!incorrect command>
    return vector<string>{Responses::PARAMS_ERROR};
}