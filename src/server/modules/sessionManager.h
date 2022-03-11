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
#include <map>
#include "../../include/packet.h"

#define MAX_USERS 2

class SessionManager {
    std::vector<std::thread> threadVector;
    static std::mutex loginMutex;
    static std::map<std::string, int> activeUsers;
public:
    SessionManager();
    static bool login(std::string username);
    static void logout(std::string username);
    void createUser(std::string username, int& cli_sockfd, struct sockaddr_in& cli_addr);
    ~SessionManager();
};