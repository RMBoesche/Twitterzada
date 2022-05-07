#include "serverReplica.h"

#define SERVER_PORT 4000
#define N_SERVERS_R 5
#define AUX_PORT 4006
#define TIMEOUT 3


void ServerReplica::start() {
    isLeader = 0;
    int replicaSock;
    struct sockaddr_in servReplica_addr, serv_addr, aux_servReplica_addr;
    Packet recv_packet;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(serv_addr.sin_zero), 8);

    Packet ping_packet = {
		.type = 0,
		.seqn = 0,
		.length = 0,
		.timestamp = 0,
        .payload = "ping"
	};

    Packet establish_packet = {
		.type = 0,
		.seqn = 0,
		.length = 0,
		.timestamp = 0,
        .payload = "replica"
	};

    CommunicationManager::createSocket(replicaSock, AUX_PORT, servReplica_addr);

    CommunicationManager::sendPacket(establish_packet, replicaSock, serv_addr)
    
    if (TIMEOUT()){
        isLeader = 1;
        new_port = SERVER_PORT;
        std::cout << new_port;
        servReplica_addr.sin_port(new_port);
        return;
    } //Pseudocodigo

    recv_packet = CommunicationManager::recv_packet(replicaSock, serv_addr);
    int new_port = atoi(recv_packet._payload);
    std::cout << new_port;
    servReplica_addr.sin_port(new_port);  // muda porta para o csocket 

    //BACKUP TOTAL
    //recv_backup

            //thread SEND PING
            //mutexLockRECV.lock();
            CommunicationManager::sendPacket(ping_packet, replicaSock, serv_addr);
            
            if (TIMEOUT()) { // if tempo de inicio + tempo de espera < tempo atual  
                
            if (!(payload != "on")){
                serverOn = 0;
                //mutexLockRECV.lock();
            }
                
            }
            //thread control_timeout --> quando der timeout, flag timeout = 1
            
            //sleep(TIMEOUT);
            recv_packet = CommunicationManager::recv_packet()
            //mutexLockRECV.lock();

        //3 THREADS
        //1 - SENDPING e inicia eleicao se necessario
        //2 - REPLICACAO e ELECTION


        //REPLICAO e ELECTION
        //recv_packet = 
        //if (payload == "backupOP")
        //
        //else if (payload == "election")

        // 1 - ping
        // 2 - backup
        // 3 - election

    while(!isLeader) {

        while(serverOn){
            //ping_packet.timestamp = 
            


            //recv_packet = CommunicationManager::recv_packet(replicaSock, aux_servReplica_addr)
            //std::string = recv_packet.payload;
            //if (payload == "election")
            //else if (payload == "backupOP") //operacao de backup 


            //se não for lider, fica preso no loop de replicas:
            //fica travada n receive esperando atualizaçoes
            //OPERACAO BACKUP --> mandado a cada operacao SEND e FOLLOW
            //2 thread, uma para ouvir o servidor principal
        }
        isLeader = election//verifica se lider
    }
}