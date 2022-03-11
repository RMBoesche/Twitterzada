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
#include "modules/communicationManager.h"
#include "modules/sessionManager.h"
#include "modules/mainSocket.h"
#include "modules/userThread.h"
#include "modules/storageManager.h"

#define MAIN_PORT 4000

int main(int argc, char *argv[])
{
	int port = MAIN_PORT + 1;
	int cli_sockfd;
	struct sockaddr_in cli_addr;
	std::vector<std::thread> threads;
	std::string str_port;

	MainSocket mainSocket(MAIN_PORT);
	mainSocket.startSocket();

	SessionManager sessionManager;

	Packet recv_packet;
	Packet send_packet;
	Packet login_packet = {
		.type = 0,
		.seqn = 0,
		.length = 0,
		.timestamp = 0
	};
 
	while (true)
	{
		//request communication by the client (automatic)
		recv_packet = CommunicationManager::recvPacket(mainSocket.getSocket(), cli_addr);

		//request communication message content is the "username"
		std::string username = std::string(recv_packet._payload);
		
		if(!SessionManager::login(username)) {
			strcpy(login_packet._payload, "failed");
			CommunicationManager::sendPacket(login_packet, mainSocket.getSocket(), cli_addr);
		}
		else {
			strcpy(login_packet._payload, "successfull");
			CommunicationManager::sendPacket(login_packet, mainSocket.getSocket(), cli_addr);
			
			//sending port to user
			str_port = std::to_string(port) + '\0';

			// give values to the packet struct
			send_packet.type = DATA;
			send_packet.seqn = StorageManager::getSeqn();
			send_packet.length = str_port.length();
			send_packet.timestamp = std::time(0);
			strcpy(send_packet._payload, str_port.c_str());

			// send new port to be used to the user
			CommunicationManager::sendPacket(send_packet, mainSocket.getSocket(), cli_addr);

			// socklen_t clilen = sizeof(struct sockaddr_in);
			// criar socket aqui
			CommunicationManager::createSocket(cli_sockfd, port, cli_addr);
			
			// std::cout << cli_sockfd << " : " << ntohs(cli_addr.sin_port) << std::endl;
			// passar socket aqui ---> producer thread
			sessionManager.createUser(
				username,
				// port, // socket
				cli_sockfd,
				cli_addr
			);
			
			// passar socket aqui ---> consumer thread
			// ...
			// thread do consumidor
			// ...

			port++;
		}
	}

	for (std::thread &t : threads)
		t.join();
		
	return 0;
}
