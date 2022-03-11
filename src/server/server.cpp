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

#define MAIN_PORT 4000

int main(int argc, char *argv[])
{
	int port = MAIN_PORT + 1;
	struct sockaddr_in cli_addr;
	std::vector<std::thread> threads;

	MainSocket mainSocket(MAIN_PORT);
	mainSocket.startSocket();

	SessionManager sessionManager;

	Packet recv_packet;
	Packet login_packet = {
		.type = 0,
		.seqn = 666,
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
			
			sessionManager.createUser(
				username,
				port,
				mainSocket.getSocket(),
				cli_addr
			);			
			port++;
		}
	}

	for (std::thread &t : threads)
		t.join();
		
	return 0;
}
