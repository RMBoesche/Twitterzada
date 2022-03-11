#ifndef communication_package
#define communication_package

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "../../include/packet.h"

#define BUFFER_SIZE 256

class Communication
{
    std::string username;
    int port;
    int sockfd;
    struct sockaddr_in serv_addr, from;
    struct hostent *server;
    Packet u_packet;
    char buffer[BUFFER_SIZE];
    int seqn = 0;

public:
    Communication(std::string &username, int port, struct hostent *server);
    void createSocket();
    int establishConnection();
    int getSocket();
    void setPort(int port);
    int sendMessage(std::string cli_message, int type);
    Packet recvPacket();
    void recvPort();
    ~Communication();
};

#endif