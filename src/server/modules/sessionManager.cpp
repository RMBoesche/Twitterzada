#include "sessionManager.h"
#include "userThread.h"

std::map<std::string, int> SessionManager::activeUsers;
std::mutex SessionManager::loginMutex;

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
	loginMutex.lock();
	// if the user is not active
	if(activeUsers.find(username) == activeUsers.end()) {
		activeUsers.insert({username, 1});
		loginMutex.unlock();
		return true;
	}
	// if the user is already logged in
	else {
		if(activeUsers[username] < MAX_USERS) {
			// increment number of active users
			activeUsers[username]++;
			loginMutex.unlock();
			return true;
		}
		else {
			std::cout << "MAXIMUM ACTIVE USERS FOR THIS USERNAME" << std::endl;
			loginMutex.unlock();
			return false;
		}
	}
}


void SessionManager::logout(std::string username) {
	// logout user
	loginMutex.lock();
	activeUsers[username]--;
	loginMutex.unlock();
}

SessionManager::~SessionManager() {}
