#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/packet.h"

#define SEND 0
#define FOLLOW 1

class CommunicationManager {
public:
    static int getQuery(std::string);
    static std::string getContent(std::string);
    static packet recvPacket(int sockfd, struct sockaddr_in*cli_addr);
    // static void sendPacket(int sockfd, struct sockaddr_in )
};