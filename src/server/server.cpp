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


typedef struct __packet{
	uint16_t type; //Tipo do pacote (p.ex. DATA | CMD)
	uint16_t seqn; //Número de sequência
	uint16_t length; //Comprimento do payload
	uint16_t timestamp; // Timestamp do dado
	const char* _payload; //Dados da mensagem
} packet;

typedef struct __notification{
	uint32_t id; //Identificador da notificação (sugere-se um identificador único)
	uint32_t timestamp; //Timestamp da notificação
	uint16_t length; //Tamanho da mensagem
	uint16_t pending; //Quantidade de leitores pendentes
	const char* _string; //Mensagem
} notification;

void new_socket(int port) {
	std::cout << 1 << std::endl;

	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	int cli_sockfd;
	char buf[256];

	std::cout << 2 << std::endl;

	std::string str_port = std::to_string(4000);
	int n = sendto(cli_sockfd, str_port.c_str(), 26, 0,(struct sockaddr *) &cli_addr, sizeof(struct sockaddr));

	std::cout << 3 << std::endl;

	socklen_t clilen = sizeof(struct sockaddr_in);

	if ((cli_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR opening socket");

	std::cout << 4 << std::endl;
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);   

	std::cout << 5 << std::endl;

	if (bind(cli_sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR on binding");
	
	std::cout << 6 << std::endl;

	int x = recvfrom(cli_sockfd, buf, 256, 0, (struct sockaddr *) &cli_addr, &clilen);
	
	std::cout << buf << std::endl;
}



int main(int argc, char *argv[])
{
	int port = 4000;
	int sockfd, n;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr1, cli_addr2;
	char buf[256];
	
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR opening socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);    
	 
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR on binding");
	
	clilen = sizeof(struct sockaddr_in);
	
	while(1) {

		port++;
		n = recvfrom(sockfd, buf, 256, 0, (struct sockaddr *) &cli_addr1, &clilen);
		if (n < 0) 
			printf("ERROR on recvfrom");
		printf("Received a datagram: %s\n", buf);

		// create thread with socket
		std::thread th1(new_socket, port);
		th1.join();
	}


	// /* receive from socket */
	// n = recvfrom(sockfd, buf, 256, 0, (struct sockaddr *) &cli_addr2, &clilen);

	// // std::cout << inet_ntoa( cli_addr1.sin_addr ); // 127.0.0.1 in all
	// // std::cout << inet_ntoa( cli_addr2.sin_addr );
	// // std::cout << inet_ntoa( serv_addr.sin_addr ); // 0.0.0.0

	// if (n < 0) 
	// 	printf("ERROR on recvfrom");
	// printf("Received a datagram: %s\n", buf);
	
	// /* send to socket */
	// n = sendto(sockfd, "Got your message client 1\n", 26, 0,(struct sockaddr *) &cli_addr1, sizeof(struct sockaddr));
	// n = sendto(sockfd, "Got your message client 2\n", 26, 0,(struct sockaddr *) &cli_addr2, sizeof(struct sockaddr));
	// if (n  < 0) 
	// 	printf("ERROR on sendto");

	
	close(sockfd);
	return 0;
}
