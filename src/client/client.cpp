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

int main(int argc, char *argv[])
{
    // validate the inputs
    Validation::validateUsername(std::string(argv[1]));
    Validation::validateHostname(std::string(argv[2]));
    Validation::validatePort(std::string(argv[3]));

    std::string username = argv[1];
    const char* server = argv[2];
    int port = atoi(argv[3]);

    // create the interface
    Interface interface;
    Communication communication(username, port, gethostbyname(server));

    Packet loggin_packet = communication.recvPacket();

    if(strcmp(loggin_packet._payload, "failed") == 0) {

        std::cout << "FAILED! Max users logged in." << std::endl;
        return 0;
    }
    else {

        std::cout << "Login Successful!" << std::endl;
        communication.recvPort();
        
        communication.sendMessage(std::string("dumb"), DATA);

        std::thread listenerThread(Receiver::start, std::ref(communication));
        
        while (true)
        {
            interface.setInput();

            // get operation
            int query = interface.getQuery();

            if (query == SEND) {
                communication.sendMessage(std::string("SEND "+interface.getMessage()), DATA);
            } 
            else if (query == FOLLOW) { 
                communication.sendMessage(std::string("FOLLOW "+interface.getUsername()), DATA);
            } 
            else if (query == END) {
                break;
            }
        }
    }
}