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
	packet recv_packet;
	int port = MAIN_PORT + 1;
	std::vector<std::thread> threads;
	struct sockaddr_in cli_addr;

	MainSocket mainSocket(MAIN_PORT);
	mainSocket.startSocket();

	SessionManager sessionManager;

	while (true)
	{
		//request communication by the client (automatic)
		recv_packet = CommunicationManager::recvPacket(mainSocket.getSocket(), &cli_addr);

		//request communication message content is the "username"
		std::string username = std::string(recv_packet._payload);

		
		if(SessionManager::login(username)) {
			//
			sessionManager.createUser(
				username,
				port,
				mainSocket.getSocket(),
				cli_addr
			);

			//
			port++;
		}
	}

	for (std::thread &t : threads)
		t.join();
		
	return 0;
}
