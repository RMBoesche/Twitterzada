#include "userThread.h"


void UserThread::start(std::string username, int cli_sockfd, struct sockaddr_in cli_addr) {
	std::cout << "starting producer" << std::endl;
	
	StorageManager::addUser(username);

	int seqn = 0;
	Packet receive_packet;

	// receive query and content
	// for example: SEND message
	receive_packet = CommunicationManager::recvPacket(cli_sockfd, cli_addr);
	
	// .....................................
	// REMOVER tem que na vdd aceitar 2 valores, 1 pra cada sessão, caso 2 usuarios entrem na mesma conta
	ThreadManager::setCli_addr(username, cli_addr);

	while (true)
	{
		int query = CommunicationManager::getQuery(receive_packet._payload);
		// std::cout << query << std::endl;
		if(query == SEND) {
			//...
			std::cout << "send";
			StorageManager::addNotification(username, CommunicationManager::getContent(receive_packet._payload));
		}
		else if(query == FOLLOW) {
			std::cout << "follow";
			// add the follower to the user
			StorageManager::addFollower(CommunicationManager::getContent(receive_packet._payload), username);
		}
		else if(query == END) {
			break;
		}

		receive_packet = CommunicationManager::recvPacket(cli_sockfd, cli_addr);
	}

	SessionManager::logout(username);
	close(cli_sockfd);
	std::cout << "finishing producer" << std::endl;
}