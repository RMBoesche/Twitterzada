#include "sessionManager.h"
#include "userThread.h"

std::map<std::string, int> SessionManager::activeUsers;
std::mutex SessionManager::loginMutex;

SessionManager::SessionManager() {}

void SessionManager::createUser(std::string username, int& cli_sockfd, struct sockaddr_in& cli_addr) {
	std::cout << cli_addr.sin_port << std::endl;

	threadVector.emplace_back(
		UserThread::start,
		username,
		cli_sockfd,
		cli_addr 
	);
}

bool SessionManager::login(std::string username) {
	loginMutex.lock();
	bool flag = false;
	// if the user is not active
	if(activeUsers.find(username) == activeUsers.end()) {
		activeUsers.insert({username, 1});
		flag = true;
	}
	// if the user is already logged in
	else if (activeUsers[username] < MAX_USERS) {
		// increment number of active users
		activeUsers[username]++;
		flag = true;
	}
	loginMutex.unlock();
	if (flag) {
		
	}
	return flag;
}


void SessionManager::logout(std::string username) {
	// logout user
	loginMutex.lock();
	activeUsers[username]--;
	loginMutex.unlock();
}

SessionManager::~SessionManager() {}
