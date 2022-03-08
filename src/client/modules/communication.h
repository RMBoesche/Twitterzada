#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 256

class Communication {
    std::string username;
    int port;
    int sockfd;
    struct sockaddr_in serv_addr;
	struct hostent *server;
    char buffer[BUFFER_SIZE];

public:
    Communication(std::string& username, int port, struct hostent *server);
    int establishConnection();
    int getSocket();
    void setPort(int port);
    int sendMessage(std::string cli_message);
    ~Communication();
};