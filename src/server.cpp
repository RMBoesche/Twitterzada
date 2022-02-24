#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#define PORT 4000

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


int main(int argc, char *argv[])
{
	int sockfd, n;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	char buf[256];
		
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) 
		printf("ERROR opening socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);    
	 
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0) 
		printf("ERROR on binding");
	
	clilen = sizeof(struct sockaddr_in);
	
	int b = 100;
	while (b--) {
		/* receive from socket */
		n = recvfrom(sockfd, buf, 256, 0, (struct sockaddr *) &cli_addr, &clilen);

		if (n < 0) 
			printf("ERROR on recvfrom");
		printf("Received a datagram: %s\n", buf);
		
		/* send to socket */
		n = sendto(sockfd, "Got your message\n", 17, 0,(struct sockaddr *) &cli_addr, sizeof(struct sockaddr));
		if (n  < 0) 
			printf("ERROR on sendto");
	}
	
	close(sockfd);
	return 0;
}
