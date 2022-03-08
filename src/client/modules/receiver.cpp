#include "receiver.h"


void Receiver::operator()(int sockfd) {
    unsigned int length = sizeof(struct sockaddr_in);

    char buffer[256];

    int n = recvfrom(sockfd, buffer, 256, 0, (struct sockaddr *) &from, &length);
    if (n < 0)
        printf("ERROR recvfrom");
    
    std::cout << buffer << std::endl;

}