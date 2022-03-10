#include "communicationManager.h"
#include "../../include/packet.h"


int CommunicationManager::getQuery(std::string received_string) {
    //get the first word
    std::string query = received_string.substr(0, received_string.find(' '));

    std::cout << query << std::endl;

    // determine the command
    if (query == std::string("SEND")) {
        return SEND;
    }
    else if (query == std::string("FOLLOW")) {
        return FOLLOW;
    }
    else {
        // indicating error
        return -1;
    }
}

std::string CommunicationManager::getContent(std::string received_string) {
     return received_string.substr(received_string.find(' ') + 1);
}

packet CommunicationManager::recvPacket(int sockfd, struct sockaddr_in* cli_addr) {
    
    socklen_t clilen = sizeof(struct sockaddr_in);
    packet recv_packet;

    int n = recvfrom(
        sockfd,
        &recv_packet,
        sizeof(packet),
        0,
        (struct sockaddr *) cli_addr,
        &clilen
    );
    if (n < 0)
		printf("ERROR on recvfrom");
    return recv_packet;
}

