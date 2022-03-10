#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include "../../include/packet.h"

class SessionManager {
    std::vector<std::thread> threadVector;
    std::mutex userMutex;
public:
    SessionManager();
    void createUser(int port, int socket, struct sockaddr_in cli_addr);
    ~SessionManager();
};