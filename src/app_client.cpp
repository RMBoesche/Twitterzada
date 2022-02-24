#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

typedef struct __packet{
	uint16_t type; //Tipo do pacote (p.ex. DATA | CMD)
	uint16_t seqn; //Número de sequência
	uint16_t length; //Comprimento do payload
	uint16_t timestamp; // Timestamp do dado
	const char* _payload; //Dados da mensagem
 } packet;


int main(int argc, char *argv[])
{
    int sockfd, n;
	unsigned int length;
	struct sockaddr_in serv_addr, from;
	struct hostent *server;
	int PORT, timestamp;
	std::string profileName;
	std::string all_data, command;

	char buffer[256];
	if (argc < 2) {
		fprintf(stderr, "usage %s hostname\n", argv[0]);
		exit(0);
	}
	
	// get profile name
	profileName = argv[1];

	// get hostname
	server = gethostbyname(argv[2]);
	if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }	

	// get PORT
	PORT = atoi(argv[3]);

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		printf("ERROR opening socket");
	
	serv_addr.sin_family = AF_INET;     
	serv_addr.sin_port = htons(PORT);    
	serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
	bzero(&(serv_addr.sin_zero), 8);

	printf("Enter the message: ");
	bzero(buffer, 256);

	// get the 'SEND' or 'FOLLOW'
	std::cin >> command;
	
	if( command == std::string("SEND") ) {
		std::cout << "SEND";

		// get the message to be sent
		std::getline(std::cin, all_data);
		strcpy(buffer, all_data.c_str());
	}
	else if( command == std::string("FOLLOW") ) {
		std::cout << "FOLLOW";
		// get the user to be followed
		std::getline(std::cin, all_data);	

	}
	else {
		std::cout << "PLEASE ENTER ONE OF THE FOLLOWING COMMANDS:\n\tSEND\n\tFOLLOW" << std::endl;
	}

	std::cout << all_data << std::endl;
	std::string message = all_data.substr( all_data.find(' ') );
	// fgets(buffer, 256, stdin);

	std::cout << buffer << std::endl;
	timestamp = (int)time(NULL);
	std::cout << timestamp << std::endl;

	n = sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in));
	if (n < 0) 
		printf("ERROR sendto");
	
	length = sizeof(struct sockaddr_in);
	n = recvfrom(sockfd, buffer, 256, 0, (struct sockaddr *) &from, &length);
	if (n < 0)
		printf("ERROR recvfrom");

	printf("Got an ack: %s\n", buffer);
	
	close(sockfd);
	return 0;
}