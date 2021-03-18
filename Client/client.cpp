#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3000
#define TRUE 1

using namespace std;

int main()
{
    struct sockaddr_in serv_addr;

    char buffer[1024] = {0};
    int opt = 1;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        cout << "Unable to create Socket!\n";
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    int connectfd = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (connectfd < 0)
    {
        cout << "Unable to Connect!\n";
        return 1;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    int valread;
    string input;
    char delim = '\n';
    while (TRUE)
    {
        valread = recv(sockfd, buffer, 1024, 0);
        if (valread <= 0)
        {
            perror("recv");
        }
        buffer[valread] = '\0';
        cout << buffer;
        buffer[0] = '\0';
        char char_input[100] = "";
        cin.getline(char_input, sizeof(char_input), '\n');
        cout << "char_input:" << char_input << endl;
        if (send(sockfd, char_input, strlen(char_input), 0) <= 0)
        {
            perror("send");
        }
    }

    return 0;
}