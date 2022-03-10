#include "sessionManager.h"
#include "userThread.h"
#include <unistd.h>

// VAI PRECISAR DE SINCRONIZAÇÃO ->
//                                  ALTERAÇÕES NA DB
//                                  ENVIO DE NOTIFICAÇÕES


SessionManager::SessionManager() {}

void SessionManager::createUser(int port, int socket, struct sockaddr_in cli_addr) {
	threadVector.emplace_back(
		userThread(), 
		// params
		port,
		socket,
		cli_addr
	);
}

SessionManager::~SessionManager() {}


// class sessionManager {
//     std::vector<std::thread> usersThread;
//     std::mutex userMutex;
// public:
//     sessionManager();
//     void createUser(int port, int socket, struct sockaddr_in cli_addr);
//     ~sessionManager();
// };
