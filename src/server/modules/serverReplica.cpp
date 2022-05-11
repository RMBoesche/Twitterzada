#include "serverReplica.h"
#include "threadManager.h"

#define COORDINATION std::string("coordination")
#define ELECTION std::string("election")
#define REPLICATION std::string("replication")

#define REPLICA_PORT 4000   // port of the replica
#define NUM_REPLICAS 5

int serverReplica::IDleader;
int serverReplica::participant;
int serverReplica::myreplicationport;
Packet serverReplica::recv_packet;
struct sockaddr_in serverReplica::rep_addr;
struct sockaddr_in serverReplica::repNeighbour_addr;
bool serverReplica::isLeader;
std::mutex serverReplica::listenLock;
MainSocket serverReplica::serverSocket = MainSocket(REPLICA_PORT);

void serverReplica::start() {
    // start the server
    serverReplica::participant=false;
	std::string str_port;
    
    myreplicationport = REPLICA_PORT;

    serverReplica::serverSocket = MainSocket(4000);
    bool foundPort = serverReplica::serverSocket.startSocket();
    
    while (!foundPort) {
        serverReplica::myreplicationport++;
        // serverReplica::serverSocket.~MainSocket();
        serverReplica::serverSocket = MainSocket(serverReplica::myreplicationport);
        foundPort = serverReplica::serverSocket.startSocket();
    }

    while(serverReplica::serverSocket.getSocket() == 0) {
        serverReplica::serverSocket.startSocket();
    }

    rep_addr = serverReplica::serverSocket.getCli_addr();

    std::cout << "SOCKET: " << serverReplica::serverSocket.getSocket() << std::endl;
    //initialize the server
    printf("Server started on port %d\n", myreplicationport);

    //faz primeira verificação se é o lider
    if(serverReplica::myreplicationport == REPLICA_PORT)
        serverReplica::isLeader = true;
    else 
        serverReplica::isLeader = false; 
    
    if(!serverReplica::isLeader) {
        ThreadManager::startListen();
    }
    // = serverReplica::myreplicationport!=REPLICA_PORT;
    // liderON = !testSocket.startSocket();
    // if (serverReplica::myreplicationport==REPLICA_PORT) {
    //     liderON = true;
    // }
    
   //loop identifica que o servidor caiu

    bool liderOFF; 
    MainSocket testServer(4000);
    while(!serverReplica::isLeader) {
        sleep(3);
        liderOFF = testServer.startSocket();
        if ((liderOFF) && (!serverReplica::participant)) {
            std::cout << "Iniciou eleicao" << std::endl;
            std::string startElectionMessage = "election " + std::to_string(serverReplica::myreplicationport);
            electionMessage(startElectionMessage);
            
            serverReplica::participant=true;
        }
    }
    serverReplica::serverSocket.~MainSocket();
    
    printf("saiu loop\n");
	
}

// thread
void serverReplica::listen() {
    
    // listen to messages -> might be replication or election messages
    // according to that, choose the correct response
    

    // id da mensagem maior -> repassa
    // id da mensagem eh menor 
    //                  --> nao participante -> passa
    //                  --> participante -> nao repassa
    // acaba quando algum processo recebe seu proprio id na mensagem
    //                  --> dai tem que mandar mensagem indicando que ele eh o novo lider
    //                  --> quando receber a mensagem de novo, vai parar de enviar

    while(!serverReplica::isLeader) {
        std::cout<<"recebido1"<<std::endl;
        recv_packet = CommunicationManager::recvPacket(serverReplica::serverSocket.getSocket(), serverReplica::repNeighbour_addr);
        std::cout<<"\nrecebido2"<<std::endl;
        
        listenLock.lock();

        std::string str_recv=serverReplica::recv_packet._payload;
        printf("Received: %s\n", str_recv.c_str());

        //get the first word
        std::string query = str_recv.substr(0, str_recv.find(' '));
        // get the rest of the content
        // it is the id for the election
        // and the operation result for the replication
        std::string content = str_recv.substr(str_recv.find(' ')+1);
       
        if(query == COORDINATION) {
            serverReplica::IDleader = std::stoi(content);
            serverReplica::participant=false;

            if( serverReplica::IDleader != serverReplica::myreplicationport) {
                //repassa
                std::cout << "IDleader " << IDleader << " " << "My port " << serverReplica::myreplicationport << std::endl;
                electionMessage(query+" "+content);
            }
            else {
                std::cout << "Virou lider: " << serverReplica::myreplicationport << std::endl;
                serverReplica::isLeader=true;

            }
        }
        else if(query== ELECTION) {
            
            if( std::stoi(content) > serverReplica::myreplicationport) {
                // repassa
                serverReplica::participant=true;
                std::cout << "continuando eleicao" << std::endl;
                electionMessage(query+" "+content);
            }
            else if( std::stoi(content) <  serverReplica::myreplicationport) {
                if(serverReplica::participant) {
                    // nao-repassa                    
                }
                else {
                    // repassa
                    std::cout << "Líder temporario: " << serverReplica::myreplicationport << std::endl;
                    serverReplica::participant=true;
                    std::string newMessage = "election "+ std::to_string(serverReplica::myreplicationport);
                    electionMessage(newMessage);
                }
            }
            else { 
                // if it is equal (recv_id == id)
                // comeca o processo de informacao do novo lider
                // manda msg informando o novo lider
                std::cout << "Vai virar lider: " << serverReplica::myreplicationport << std::endl;
                serverReplica::serverSocket.setPort(4000);

                std::cout << "Coordination message: coordination + " <<  serverReplica::myreplicationport << std::endl;
                std::string coordMessage = "coordination " + std::to_string(serverReplica::myreplicationport);
                electionMessage(coordMessage);
            }
        }
        else if(query == REPLICATION) {
            //...
        }

        listenLock.unlock();
        
    }
}

void serverReplica::electionMessage(std::string message) { 
    // election protocol

    // manda pro proximo do anel
    // manda msg
    //create message packet to be sent
    Packet send_packet = {
		.type = 0,
		.seqn = 0,
		.length = 0,
		.timestamp = 0
	};
    strcpy(send_packet._payload, message.c_str());
    
    struct sockaddr_in send_addr;
    send_addr.sin_family = AF_INET;	
	send_addr.sin_addr.s_addr = INADDR_ANY;

    // get the neighbor to create the ring
    int Neighbor = getNeighbor();

    if (Neighbor == serverReplica::myreplicationport) {
        std::cout << "entrou aqui" << std::endl;
        serverReplica::isLeader = true;  
        return;
    }    
    send_addr.sin_port = htons(Neighbor);
    
    std::cout << "mandando mensagem" << std::endl;
    std::cout << "getSocket: " << serverReplica::serverSocket.getSocket() << std::endl;
    std::cout << "send_port: " << ntohs(send_addr.sin_port) << std::endl; 
    CommunicationManager::sendPacket(send_packet, serverReplica::serverSocket.getSocket(), send_addr);
    std::cout << "mensagem mandada" << std::endl;
    
}

int serverReplica::getNeighbor() {

    int port = serverReplica::myreplicationport;
    printf("entrou no get com port: %d\n", port);
    
    //next port
    // port++;
    // if (port >= 4006){
    //     port = 4001;
    // }

     MainSocket testSocket(port);
     bool foundPort = true;
    // bool foundPort = testSocket.startSocket();
    while (foundPort) {
        port++;
        if (port >= REPLICA_PORT+NUM_REPLICAS+1){
            port = REPLICA_PORT+1;
        }
        if (port == serverReplica::myreplicationport) {
            printf("AQUIIIII Neighbor: %d\n", port);
            return port;
        }

        // port=(port-REPLICA_PORT+1)%5 + REPLICA_PORT;
        // if(port==4000){
        //     port=4001;
        // }


        // // if there is only the current server available
        // if(port == serverReplica::myreplicationport) {
        //      printf("Neighbor: %d\n", port);
        //     return port;
        // }

        testSocket.~MainSocket();
        testSocket = MainSocket(port);
        printf("testou port: %d\n", port);
        foundPort = testSocket.startSocket();
    }
    printf("Neighbor: %d\n", port);
    return port;
    
}
