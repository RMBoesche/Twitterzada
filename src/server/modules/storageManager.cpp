#include "storageManager.h"

std::map<std::string, std::set<std::string>> StorageManager::userFollowers;
std::map<std::string, std::set<Notification>> StorageManager::userNotifications;
std::map<std::string, std::tuple<std::string, int>> StorageManager::userPendingNotifications;
int StorageManager::seqn = 0;
int StorageManager::id = 0;
std::mutex StorageManager::sessionLock;

void StorageManager::addFollower(std::string username, std::string follower) {
    sessionLock.lock();
    userFollowers[username].insert(follower);
    sessionLock.unlock();
    // for(auto itr = userFollowers[username].begin(); itr!= userFollowers[user].end(); itr++)
    //     std::cout << *itr << std::endl;
}

void StorageManager::addNotification(std::string username, std::string message) {
    sessionLock.lock();

    id++;

    Notification notification = {
        .id = id,
        .timestamp = std::time(0),
        .length = message.length(),
        .pending = userFollowers[username].empty() ? userFollowers[username].size() : 0,
    };

    strcpy(notification._string, message.c_str());
    
    userNotifications[username].insert(notification);
    
    sessionLock.unlock();
    // for(auto i : userNotifications[username]) {
    //     std::cout << i.id << std::endl; 
    // }
}

inline bool operator<(const Notification& lhs, const Notification& rhs)
{
  return lhs.id < rhs.id;
}

int StorageManager::getSeqn() {
    return seqn;
}

void StorageManager::incrementSeqn() {
    seqn++;
}
