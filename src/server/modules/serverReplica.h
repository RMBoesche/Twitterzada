#ifndef serverReplica_def
#define serverReplica_def
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>

#include "mainSocket.h"
#include "sessionManager.h"
#include "communicationManager.h"
#include "threadManager.h"

class serverReplica {
    static int IDleader;
    static int participant;
    static int myreplicationport;
    static Packet recv_packet;
    static struct sockaddr_in rep_addr;
    static struct sockaddr_in repNeighbour_addr;
    static bool isLeader;
    static std::mutex listenLock;
    
    static MainSocket serverSocket;
public:
    static void start();
    static void listen();
    static void electionMessage(std::string);
    static int getNeighbor();
};

#endif  


