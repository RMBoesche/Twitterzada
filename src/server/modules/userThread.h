#include <iostream>
#include "../../include/packet.h"

class userThread {
   public: 
        static void start(std::string, int port, int sockfd, struct sockaddr_in cli_addr); 
};