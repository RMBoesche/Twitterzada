#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "../../include/packet.h"

#define SEND 0
#define FOLLOW 1
#define END 2

class CommunicationManager {
public:
    static int getQuery(std::string);
    static std::string getContent(std::string);
    static packet recvPacket(int sockfd, struct sockaddr_in*cli_addr);
    static void sendPacket(packet& send_packet, int sockfd, struct sockaddr_in& );
};