#include <iostream>

class userThread {
    void operator()(int port, int sockfd, struct sockaddr_in cli_addr);
};