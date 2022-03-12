#ifndef userThread_def
#define userThread_def

#include <iostream>
#include "../../include/packet.h"
#include <unistd.h>
#include <string.h>
#include <string>
#include "../../include/packet.h"
#include "threadManager.h"
#include "sessionManager.h"
#include "communicationManager.h"
#include "storageManager.h"

class UserThread {
   public: 
        static void start(std::string, int sockfd, struct sockaddr_in cli_addr);
};

#endif