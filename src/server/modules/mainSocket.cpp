#include "mainSocket.h"


MainSocket::MainSocket(int port)
    : port(port)
{
    clilen = sizeof(struct sockaddr_in);
}

void MainSocket::startSocket() {

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR opening socket");

    serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);   
	 
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR on binding");
   
}

Packet MainSocket::recvPacket() {
    int n = recvfrom(
        sockfd,
        &m_packet,
        sizeof(struct __packet),
        0,
        (struct sockaddr *) &cli_addr,
        &clilen
    );
    if (n < 0)
		printf("ERROR on recvfrom");
    return m_packet;
}

struct sockaddr_in MainSocket::getCli_addr() {
    return cli_addr;
}

int MainSocket::getSocket() {
    return sockfd;
}

MainSocket::~MainSocket() {
    close(sockfd);
}
