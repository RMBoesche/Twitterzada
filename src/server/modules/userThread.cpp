#include "userThread.h"
#include <unistd.h>
#include <string.h>
#include "../../include/packet.h"

void userThread::operator()(int port, int sockfd, struct sockaddr_in cli_addr) {

    std::cout << 1 << std::endl;

	struct sockaddr_in thread_addr;
	int cli_sockfd;
	char buf[256];
    packet receive_packet;
    int exit = 0;

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

	while (!exit)
	{
		bzero(buf, 256);
		int x = recvfrom(cli_sockfd, &receive_packet, sizeof(receive_packet), 0, (struct sockaddr *)&cli_addr, &clilen);
		std::cout << receive_packet._payload << std::endl;
        if(strcmp("end", receive_packet._payload)) {
            exit = 1;
        }
        sleep(5);
	}

    std::cout << "terminando a thread" << std::endl;
}