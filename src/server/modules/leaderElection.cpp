#include <communicationManager.h>

#define MAIN_PORT 4000
#define N_SERVERS_R 5
#define LAST_AUX_PORT 4006

void LeaderElection::start() {
      printf("start init_election\n");
    // bloqueante até que tenhamos um eleito
    in_election = 1;
    resistence_msg *result = send_election();

    if (result == NULL) {
        // ninguém me respondeu, é porque eu sou o maior ID e devo ser o coordenador
        send_announcement();
        pthread_mutex_lock(&election_mutex);
        in_election = 0;
        is_coordinator = 1;
        coord_pid = getpid();
        pthread_mutex_unlock(&election_mutex);
    } else {
        // não sou o maior ID, devo espear até que elejam alguém 
        pthread_mutex_lock(&election_mutex);
        pthread_cond_wait(&election_cond, &election_mutex);
        pthread_mutex_unlock(&election_mutex);
    }
    printf("end init_election\n");
}

int LeaderElection::select() {

  for (int port = MAIN_PORT; port < LAST_AUX_PORT; port++) {
    bool isLeader = false
    while (!isLeader) {
      //Packet& packet, int sockfd, int min
      CommunicationManager::sendMulticast('', )
    }
  }
}

int LeaderElection::sendLeaderMessage() {
  CommunicationManager::sendMulticast()
}