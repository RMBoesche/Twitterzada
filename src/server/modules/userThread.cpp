#include "userThread.h"
#include <unistd.h>
#include <string.h>
#include "../../include/packet.h"

void userThread::start(std::string username, int port, int sockfd, struct sockaddr_in cli_addr) {
	std::cout << "aah";

	struct sockaddr_in thread_addr;
	int cli_sockfd;
	char buf[256];
	packet receive_packet;
	int exit = 0;

	// send new port to be used to the user
	std::string str_port = std::to_string(port) + '\n';
	int n = sendto(sockfd, str_port.c_str(), 5, 0, (struct sockaddr *)&cli_addr, sizeof(struct sockaddr));

	socklen_t clilen = sizeof(struct sockaddr_in);

	// create the new socket in the new port
	if ((cli_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		printf("ERROR opening socket");

	thread_addr.sin_family = AF_INET;
	thread_addr.sin_port = htons(port);
	thread_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(thread_addr.sin_zero), 8);

	if (bind(cli_sockfd, (struct sockaddr *)&thread_addr, sizeof(struct sockaddr)) < 0)
		printf("ERROR on binding");

	while (!exit)
	{
		bzero(buf, 256);
		int x = recvfrom(cli_sockfd, &receive_packet, sizeof(receive_packet), 0, (struct sockaddr *)&cli_addr, &clilen);
		std::cout << receive_packet._payload << std::endl;
		if(strcmp("end", receive_packet._payload) == 0) {
			exit = 1;
		}
		sleep(5);
	}

	close(cli_sockfd);
	std::cout << "terminando a thread" << std::endl;
}