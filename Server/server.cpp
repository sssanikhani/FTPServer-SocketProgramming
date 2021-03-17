#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>   //close  
#include <arpa/inet.h>    //close  
#include <sys/time.h>
#include <fstream>
#include "../json/single_include/nlohmann/json.hpp"
#include <sstream>


#define PORT 3000
#define MAX_CLIENTS 20
#define TRUE 1
#define FALSE -1
#define SPACE ' '
#define ZERO 48
#define Z 122
#define USER "user"
#define PASSWORD "pass"

using json = nlohmann::json;
using namespace std;

bool login_user(vector< vector<string> > &clients, vector< vector<string> > enrolled_users, int client_id) {
    for(int i = 0; i < enrolled_users.size(); i++) {

    }
}

int check_username(vector< vector<string> > enrolled_users, string username) {
    int user_row;
    for(int i = 0; i < enrolled_users.size(); i++) {
        cout << enrolled_users[i][0] << endl;
        if(username == enrolled_users[i][0])
            return i;
    }
    return -1;
}

bool check_password(vector< vector<string> > enrolled_users, string password, int user_row) {
    if(enrolled_users[user_row][1] == password)
        return true;
    else
        return false;
}

bool check_username_password(vector< vector<string> > &clients, vector< vector<string> > enrolled_users, int client_id) {
    for(int i = 0; i < enrolled_users.size(); i++) {
        if(clients[client_id][1] == enrolled_users[i][0] && clients[client_id][2] == enrolled_users[i][1])
            return true;
    }
    return false;
}

string get_command(string input, int &pos) {
    string command = "";
    while(input[pos] == SPACE)
        pos++;

    while(input[pos] != SPACE) {
        if(input[pos] < ZERO || input[pos] > Z) break;
        command += input[pos];
        pos++;
    }
    return command;
}

string get_value(string input, int &pos) {
    string value = "";
    while(input[pos] == SPACE)
        pos++;
    
    while(TRUE) {
        if(input[pos] < ZERO || input[pos] > Z) break;
        value += input[pos];
        pos++; 
    }
    return value;
}

void handle_input(string input, vector< vector<string> > &clients, vector< vector<string> > enrolled_users, int client_id, int client_sd) {
    char* message;
    int user_row = 0;
    string command;
    string value;
    int pos = 0;
    //Enter fd
    if(clients[client_id][0] == "") 
        clients[client_id][0] = to_string(client_sd);


    command = get_command(input, pos);
    cout << command << endl;
    if(command == USER) {
        message = "331: User name okay, need password.\n";
        value = get_value(input, pos);
        cout << value << endl;
        clients[client_id][1] = value;
        //send(client_sd, message, strlen(message), 0);
    }else if(command == PASSWORD) {
        value = get_value(input, pos);
        clients[client_id][2] = value;
        if(check_username_password(clients, enrolled_users, client_id)) {
            message = "230: User logged in, proceed. Logged out if appropriate.\n";
        }else {
            message = "430: Invalid username or password.\n";
            clients[client_id][1] = "";
            clients[client_id][2] = "";
        }
    }else {
        message = "501: Syntax error in parameters or arguments.\n";
    }
    send(client_sd, message, strlen(message), 0);
    cout << clients[client_id][0] << "  " << clients[client_id][1] << "  " << clients[client_id][2] << endl;
}

void clear_user(vector< vector<string> > &clients, int client_id) {
    clients[client_id][0] = ""; 
    clients[client_id][1] = ""; 
    clients[client_id][2] = ""; 
    clients[client_id][3] = ""; 
}

void collect_enrolled_users(json config, vector< vector<string> > &enrolled_users) {
    string username, password, size, admin;
    vector<string> user;
    int i = 0;
    for (int i = 0; i < config["users"].size(); i++) {
        username = config["users"][i]["user"];
        password = config["users"][i]["password"];
        size = config["users"][i]["size"];
        admin = config["users"][i]["admin"];
        user.push_back(username);
        user.push_back(password);
        user.push_back(size);
        user.push_back(admin);
        enrolled_users.push_back(user);
        user.clear();
        cout << enrolled_users[i][0] << endl;
        cout << enrolled_users[i][1] << endl;
        cout << enrolled_users[i][2] << endl;
        cout << enrolled_users[i][3] << endl;
        cout << "--------------" << endl;
    }
}

int main() {
    
    vector< vector<string> > clients(MAX_CLIENTS, vector<string>(5, ""));

    vector< vector<string> > enrolled_users(0, vector<string>(4, ""));

    ifstream config_file("config.json", ifstream::binary);

    string content( (std::istreambuf_iterator<char>(config_file) ), (std::istreambuf_iterator<char>()    ) );

    json config;
    stringstream(content) >> config;
    collect_enrolled_users(config, enrolled_users);
    
    struct sockaddr_in server;
    
    int srvlen = sizeof(server); 
    int opt = 1;

    fd_set readfds;
    int max_sd, sd, activity, valread;
    int new_socket;

    char buffer[1025];
    char client_message[1025];

    int client_socket[MAX_CLIENTS];
    for(int i = 0; i < MAX_CLIENTS; i++) {
        client_socket[i] = 0;
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if( sockfd < 0 ) {
        cout << "Unable to create Socket!";
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    }

    if( bind(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0 ) {
        cout << "Unable to Bind!!";
        return 1;
    }

    listen(sockfd, MAX_CLIENTS);

    while(TRUE) {   
        FD_ZERO(&readfds);   
     
        FD_SET(sockfd, &readfds);   
        max_sd = sockfd;   
             
        for ( int i = 0 ; i < MAX_CLIENTS ; i++) {   
            sd = client_socket[i];   
                 
            if(sd > 0)   
                FD_SET( sd , &readfds);   
                 
            if(sd > max_sd)   
                max_sd = sd;   
        }   
     
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR)) {   
            printf("select error");   
        }   
             
        if (FD_ISSET(sockfd, &readfds)) {   
            if ((new_socket = accept(sockfd, (struct sockaddr *)&server, (socklen_t*)&srvlen))<0) {   
                perror("accept");   
                exit(EXIT_FAILURE);   
            }   
             
            printf("New connection , socket fd is %d , ip is : %s , port : %d  \n" , new_socket , inet_ntoa(server.sin_addr) , ntohs (server.sin_port));   
            
            char* message = "You are now connected \nEnter your Username:\n";
            if( send(new_socket, message, strlen(message), 0) != strlen(message) ) {   
                perror("send");   
            }

            puts("Welcome message sent successfully");   
                 
            for (int i = 0; i < MAX_CLIENTS; i++) {   
                if( client_socket[i] == 0 ) {   
                    client_socket[i] = new_socket;   
                    printf("Adding to list of sockets as %d\n" , i);   
                    break;   
                }   
            }   
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {   
            sd = client_socket[i];   
            if(FD_ISSET(sd , &readfds)) {   
                int read_size = read(sd, client_message, 1025);
                if( read_size <= 0 ) {   
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&server, (socklen_t*)&srvlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(server.sin_addr) , ntohs(server.sin_port));   
                    clear_user(clients, i);
                    close(sd);   
                    client_socket[i] = 0;   
                } else {       
                    client_message[read_size] = '\0';
                    cout << client_message << endl;
                    handle_input(client_message, clients, enrolled_users, i, sd);
                    client_message[0] = '\0';
                }   
            }   
        }

    }
    return 0;
}