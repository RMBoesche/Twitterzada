#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
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
#include "../../include/packet.h"

class SessionManager {
    std::vector<std::thread> threadVector;
    std::mutex userMutex;
public:
    SessionManager();
    void createUser(int port, int sockfd, struct sockaddr_in cli_addr);
    ~SessionManager();
};