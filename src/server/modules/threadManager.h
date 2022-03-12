#ifndef threadManager_def
#define threadManager_def 

#include "userThread.h"
#include "notificationThread.h"
#include "../../include/notification.h"
#include <thread>
#include <vector>

class ThreadManager {
    static std::vector<std::thread> threadVector;
    static std::map<std::string, struct sockaddr_in> userCli_addr;
    static std::mutex lock_cli_addr; // REMOVER só 1 vez alterado e dps só lido

public:
    static void createProducerThread(std::string username, int& cli_sockfd, struct sockaddr_in& cli_addr);
    static void createConsumerThread(std::string username, int& cli_sockfd, struct sockaddr_in& cli_addr);
    static struct sockaddr_in getCli_addr(std::string);
    static void setCli_addr(std::string, struct sockaddr_in);
};

#endif