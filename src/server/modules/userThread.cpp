#include "userThread.h"
#include <unistd.h>
#include <string.h>
#include <string>
#include "../../include/packet.h"
#include "sessionManager.h"
#include "communicationManager.h"
#include "storageManager.h"

void creatSocket(int& cli_sockfd, int port, struct sockaddr_in& thread_addr) {
	if ((cli_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		printf("ERROR opening socket");

	thread_addr.sin_family = AF_INET;
	thread_addr.sin_port = htons(port);
	thread_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(thread_addr.sin_zero), 8);

	if (bind(cli_sockfd, (struct sockaddr *)&thread_addr, sizeof(struct sockaddr)) < 0)
		printf("ERROR on binding");
}

void userThread::start(std::string username, int port, int sockfd, struct sockaddr_in cli_addr) {

	int seqn = 0;
	struct sockaddr_in thread_addr;
	int cli_sockfd;
	Packet receive_packet;

	std::string str_port = std::to_string(port) + '\0';

	Packet send_packet = {
		.type = DATA,
		.seqn = StorageManager::getSeqn(),
		.length = str_port.length(),
		.timestamp = std::time(0)
	};
	strcpy(send_packet._payload, str_port.c_str());

	// send new port to be used to the user
	CommunicationManager::sendPacket(send_packet, sockfd, cli_addr);

	socklen_t clilen = sizeof(struct sockaddr_in);

	// create the new socket in the new port
	creatSocket(cli_sockfd, port, thread_addr);

	// cria thread -> envia as notificacoes
	
	while (true)
	{


		// receive query and content
		// for example: SEND message
		receive_packet = CommunicationManager::recvPacket(cli_sockfd, cli_addr);

		int query = CommunicationManager::getQuery(receive_packet._payload);
		std::cout << query << std::endl;
		if(query == SEND) {
			//...
			std::cout << "send";
			StorageManager::addNotification(username, CommunicationManager::getContent(receive_packet._payload));
		}
		else if(query == FOLLOW) {
			std::cout << "follow";
			// add the follower to the user
			StorageManager::addFollower(username, CommunicationManager::getContent(receive_packet._payload));
		}
		else if(query == END) {
			break;
		}
	}

	SessionManager::logout(username);
	close(cli_sockfd);
	std::cout << "terminando a thread" << std::endl;
}