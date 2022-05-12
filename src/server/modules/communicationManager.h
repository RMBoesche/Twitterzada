#ifndef communication_def
#define communication_def

#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/packet.h"

#define SEND 0
#define FOLLOW 1
#define END 2

class CommunicationManager {
public:
    static void createSocket(int& cli_sockfd, int port, struct sockaddr_in& thread_addr);
    static int getQuery(std::string);
    static std::string getContent(std::string);
    static Packet recvPacket(int sockfd, struct sockaddr_in& cli_addr);
    static void sendPacket(Packet& send_packet, int sockfd, struct sockaddr_in& );
    static void sendMulticast(Packet&, int);
};

#endif