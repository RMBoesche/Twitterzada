#include "communicationManager.h"
#include "../../include/packet.h"
#include <arpa/inet.h>



void CommunicationManager::createSocket(int& cli_sockfd, int port, struct sockaddr_in& thread_addr) {
	if ((cli_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		printf("ERROR opening socket");

	thread_addr.sin_family = AF_INET;
	thread_addr.sin_port = htons(port);
	thread_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(thread_addr.sin_zero), 8);

	if (bind(cli_sockfd, (struct sockaddr *)&thread_addr, sizeof(struct sockaddr)) < 0)
		printf("ERROR on binding");
}

int CommunicationManager::getQuery(std::string received_string) {
    //get the first word
    std::string query = received_string.substr(0, received_string.find(' '));

    // determine the command
    if (query == std::string("SEND")) {
        return SEND;
    }
    else if (query == std::string("FOLLOW")) {
        return FOLLOW;
    }
    else if (query == std::string("END")) {
        return END;
    }
    else {
        // indicating error
        return -1;
    }
}

std::string CommunicationManager::getContent(std::string received_string) {
     return received_string.substr(received_string.find(' ') + 1);
}

Packet CommunicationManager::recvPacket(int sockfd, struct sockaddr_in& cli_addr) {
    
    socklen_t clilen = sizeof(struct sockaddr_in);
    Packet packet;

    std::cout << std::flush;
    int n = recvfrom(
        sockfd,
        &packet,
        sizeof(packet),
        0,
        (struct sockaddr *)&cli_addr,
        &clilen
    );

    if (n < 0)
		printf("ERROR on recvfrom");

    return packet;
}

void CommunicationManager::sendPacket(Packet& packet, int sockfd, struct sockaddr_in& cli_addr) {
    
    int n = sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&cli_addr, sizeof(struct sockaddr));
    if(n<0) {
        std::cout << "ERROR sendto" << std::endl;
    }
}

void CommunicationManager::sendMulticast(Packet& packet, int sockfd, int min) {
    
    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);

    for (int port = min+1 ; port <= MAIN_PORT + N_SERVERS_R; port++) {
	    serv_addr.sin_port = htons(port);
        sendPacket(packet, sockfd, serv_addr)
    }

}