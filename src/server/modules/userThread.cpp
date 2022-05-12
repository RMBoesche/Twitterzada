#include "userThread.h"

void UserThread::start(std::string username, int cli_sockfd, struct sockaddr_in cli_addr) {
	
	StorageManager::addUser(username);

	int seqn = 0;
	Packet receive_packet;

	// receive query and content
	// for example: SEND message
	receive_packet = CommunicationManager::recvPacket(cli_sockfd, cli_addr);
	
	// .....................................
	// REMOVER tem que na vdd aceitar 2 valores, 1 pra cada sessão, caso 2 usuarios entrem na mesma conta
	
	StorageManager::setClientData(username, cli_sockfd, cli_addr);

	//PRONTO PRA SER CONSUMIDO - FUNCIONOU!!!!


	while (true)
	{
		receive_packet = CommunicationManager::recvPacket(cli_sockfd, cli_addr);

		int query = CommunicationManager::getQuery(receive_packet._payload);
		// std::cout << query << std::endl;
		if(query == SEND) {
			//...
			std::cout << username << " sent: " << CommunicationManager::getContent(receive_packet._payload) << std::endl;
			StorageManager::addNotification(username, CommunicationManager::getContent(receive_packet._payload));
			
			std::string aux = receive_packet._payload;
			std::string payload = "replication " + username + " " + aux;
			strcpy(receive_packet._payload, payload.c_str());

			CommunicationManager::sendMulticast(receive_packet, cli_sockfd);
		}
		else if(query == FOLLOW) {
			std::cout << username << " followed " << CommunicationManager::getContent(receive_packet._payload) << std::endl;
			// add the follower to the user
			StorageManager::addFollower(CommunicationManager::getContent(receive_packet._payload), username);
			StorageManager::saveState();

			std::string aux = receive_packet._payload;
			std::string payload = "replication " + username + " " + aux;
			strcpy(receive_packet._payload, payload.c_str());
			
			CommunicationManager::sendMulticast(receive_packet, cli_sockfd);
		}
		else if(query == END) {
			SessionManager::logout(username);
			close(cli_sockfd);
			StorageManager::removeClient(username, cli_sockfd);
			break;
		}
	}
}
