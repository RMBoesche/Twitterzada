#include "sessionManager.h"
#include "userThread.h"
// VAI PRECISAR DE SINCRONIZAÇÃO ->
//                                  ALTERAÇÕES NA DB
//                                  ENVIO DE NOTIFICAÇÕES


SessionManager::SessionManager() {}

void SessionManager::createUser(int port, int sockfd, struct sockaddr_in cli_addr) {
	std::cout << cli_addr.sin_port << std::endl;

	userThread userThr;

	threadVector.emplace_back(
		userThread::start,
		port,
		sockfd,
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
