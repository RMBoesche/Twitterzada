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

#include "../include/packet.h"
#include "modules/sessionManager.h"
#include "modules/mainSocket.h"

#define MAIN_PORT 4000

void new_socket(int port, int sockfd, struct sockaddr_in cli_addr)
{
	std::cout << 1 << std::endl;

	struct sockaddr_in thread_addr;
	int cli_sockfd;
	char buf[256];

	std::cout << 2 << std::endl;

	std::string str_port = std::to_string(port) + '\n';
	int n = sendto(sockfd, str_port.c_str(), 5, 0, (struct sockaddr *)&cli_addr, sizeof(struct sockaddr));

	std::cout << 3 << std::endl;

	socklen_t clilen = sizeof(struct sockaddr_in);

	if ((cli_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		printf("ERROR opening socket");

	std::cout << 4 << std::endl;

	thread_addr.sin_family = AF_INET;
	thread_addr.sin_port = htons(port);
	thread_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(thread_addr.sin_zero), 8);

	std::cout << 5 << std::endl;

	if (bind(cli_sockfd, (struct sockaddr *)&thread_addr, sizeof(struct sockaddr)) < 0)
		printf("ERROR on binding");

	std::cout << 6 << std::endl;

	int x = 10;
	while (x--)
	{
		bzero(buf, 256);
		int x = recvfrom(cli_sockfd, buf, 256, 0, (struct sockaddr *)&cli_addr, &clilen);
		std::cout << buf << std::endl;
		sleep(5);
	}
}

int main(int argc, char *argv[])
{
	packet recv_packet;
	int port = MAIN_PORT + 1;
	std::vector<std::thread> threads;

	MainSocket mainSocket(MAIN_PORT);
	mainSocket.startSocket();

	SessionManager sessionManager;

	while (1)
	{
		recv_packet = mainSocket.recvPacket();

		std::cout << "inicio" << std::endl;
		std::cout << recv_packet.length << std::endl;
		std::cout << recv_packet.seqn << std::endl;
		std::cout << recv_packet.timestamp << std::endl;
		std::cout << recv_packet._payload << std::endl;

		sessionManager.createUser(
			port,
			mainSocket.getSocket(),
			mainSocket.getCli_addr()
		);

		port++;
	}

	for (std::thread &t : threads)
		t.join();
		
	return 0;
}
