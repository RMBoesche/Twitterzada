#include "threadManager.h"

std::vector<std::thread> ThreadManager::threadVector;
std::map<std::string, struct sockaddr_in> ThreadManager::userCli_addr;
std::mutex ThreadManager::lock_cli_addr;

void ThreadManager::createProducerThread(std::string username, int& cli_sockfd, struct sockaddr_in& cli_addr) {
	// std::cout << cli_addr.sin_port << std::endl;
	threadVector.emplace_back(
		UserThread::start,
		username,
		cli_sockfd,
		cli_addr
	);
}

void ThreadManager::createConsumerThread(std::string username) {
	// std::cout << cli_addr.sin_port << std::endl;
	threadVector.emplace_back(
		NotificationThread::sendNotificationsToUser,
		username
	);
}


struct sockaddr_in ThreadManager::getCli_addr(std::string username) {
	return userCli_addr[username];
}

void ThreadManager::setCli_addr(std::string username, struct sockaddr_in cli_addr) {
	userCli_addr[username] = cli_addr;
}