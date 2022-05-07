#include "server.hpp"

int main(int argc, char *argv[])
{
	int port = AUX_PORT + 1;
	int portServer = MAIN_PORT + 1;
	int cli_sockfd;
	struct sockaddr_in cli_addr, new_cli_addr;
	std::string str_port;
	
	// read data from files
	StorageManager::recoverState();

	ServerReplica::start();

	//Is the leader
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
	Packet server_packet = {
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
		std::string payload = std::string(recv_packet._payload);

		if ((payload == "multicast") || (payload == "ping")) {
			//Servers backups communication
			strcpy(server_packet.payload, "on");
			CommunicationManager::sendMulticast(server_packet, mainSocket.getSocket());
		}
		else if (payload == "replica") {
				//sending port to server
				str_portServer = std::to_string(portServer) + '\0';

				// give values to the packet struct
				send_packet.type = DATA;
				send_packet.seqn = 0;
				send_packet.length = str_port.length();
				send_packet.timestamp = std::time(0);
				strcpy(send_packet._payload, str_port.c_str());

				CommunicationManager::sendPacket(send_packet, mainSocket.getSocket(), cli_addr);

				portServer++;
		}
		else {
			//Client communication
			std::string username = payload;
		
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

				CommunicationManager::createSocket(cli_sockfd, port, new_cli_addr);

				ThreadManager::createProducerThread(username, cli_sockfd, new_cli_addr);

				ThreadManager::createConsumerThread(username);

				port++;
			}
		}
	}
	return 0;
}
