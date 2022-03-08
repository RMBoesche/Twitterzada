// user models
#include "./modules/interface.h"
#include "./modules/validation.h"
#include "./modules/communication.h"
#include "./modules/receiver.h"

// communication models
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// general modules
#include <string.h>
#include <iostream>
#include <string>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 256

int main(int argc, char* argv[]) {

    // int sockfd;
	// struct hostent *server;
    // char buffer[BUFFER_SIZE];

    struct sockaddr_in from;


    // validate the inputs
    Validation::validateUsername(std::string(argv[1]));
    Validation::validateHostname(std::string(argv[2]));
    Validation::validatePort(std::string(argv[3]));
    
    std::string username = argv[1];
    // server = gethostbyname(argv[2]);
    int port = atoi(argv[3]);


    //create the interface
    Interface interface;
    Communication communication(username, port, gethostbyname(argv[2]));
    
    ///////////////
    unsigned int length = sizeof(struct sockaddr_in);
    char buffer[256];

    int n = recvfrom(communication.getSocket(), buffer, 256, 0, (struct sockaddr *) &from, &length);
    if (n < 0)
        printf("ERROR recvfrom");
    
    std::cout << buffer << std::endl;

    int new_port = atoi(buffer);
    communication.setPort(new_port);

    communication.sendMessage(NULL);
    ////////////////

    // std::thread recv_thread( Receiver(), communication.getSocket() );
    // recv_thread.join();

    getchar();
}