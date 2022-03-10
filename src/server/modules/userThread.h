#include <iostream>
#include "../../include/packet.h"

class userThread {
   public: 
        static void start(int port, int sockfd, struct sockaddr_in cli_addr); 
};