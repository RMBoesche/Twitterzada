#ifndef mainSocket_def
#define mainSocket_def

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "../../include/packet.h"

class MainSocket {
    int port;
	int sockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	Packet m_packet;
public:
    MainSocket(int port);
    bool startSocket();
    struct sockaddr_in getCli_addr();
    int getSocket();
    Packet recvPacket();
    void setPort(int);
    ~MainSocket();
};

#endif