#ifndef userThread_def
#define userThread_def

#include <iostream>
#include "../../include/packet.h"

class UserThread {
   public: 
        static void start(std::string, int sockfd, struct sockaddr_in cli_addr);
        // static void createSocket(int& cli_sockfd, int port, struct sockaddr_in& thread_addr); 
};

#endif