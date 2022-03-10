#include "storageManager.h"

std::map<std::string, std::set<std::string>> StorageManager::userFollowers;
std::map<std::string, std::set<std::string>> StorageManager::userReceive;
std::map<std::string, std::set<std::string>> StorageManager::userToSend;
int StorageManager::seqn = 0;

int StorageManager::getSeqn() {
    return seqn;
}

void StorageManager::incrementSeqn() {
    seqn++;
}

void StorageManager::addFollower(std::string user, std::string follower) {
    userFollowers[user].insert(follower);
    // for(auto itr = userFollowers[user].begin(); itr!= userFollowers[user].end(); itr++)
    //     std::cout << *itr << std::endl;
}

