#include "sessionManager.h"
#include "userThread.h"
// VAI PRECISAR DE SINCRONIZAÇÃO ->
//                                  ALTERAÇÕES NA DB
//                                  ENVIO DE NOTIFICAÇÕES

std::map<std::string, int> SessionManager::activeUsers;

SessionManager::SessionManager() {}

void SessionManager::createUser(std::string username, int port, int sockfd, struct sockaddr_in cli_addr) {
	std::cout << cli_addr.sin_port << std::endl;

	threadVector.emplace_back(
		userThread::start,
		username,
		port,
		sockfd,
		cli_addr
	);
}

bool SessionManager::login(std::string username) {
	// if the user is not active
	if(activeUsers.find(username) == activeUsers.end()) {
		activeUsers.insert({username, 1});
		return true;
	}
	// if the user is already logged in
	else {
		if(activeUsers[username] < MAX_USERS) {
			// increment number of active users
			activeUsers[username]++;
			return true;
		}
		else {
			std::cout << "MAXIMUM ACTIVE USERS FOR THIS USERNAME" << std::endl;
			return false;
		}
	}
}


void SessionManager::logout(std::string username) {
	// logout user
	activeUsers[username]--;
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
