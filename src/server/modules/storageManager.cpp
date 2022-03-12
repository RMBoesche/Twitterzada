#include "storageManager.h"

std::map<std::string, std::set<std::string>> StorageManager::userFollowers;
std::map<std::string, std::vector<Notification>> StorageManager::userNotifications;
std::map<std::string, std::queue<std::tuple<std::string, int>>> StorageManager::userPendingNotifications;
int StorageManager::seqn = 0;
int StorageManager::id = 0;
std::mutex StorageManager::sessionLock;
std::mutex StorageManager::notificationLock;
std::mutex StorageManager::pendingNotificationLock;
std::condition_variable StorageManager::cv;
bool StorageManager::ready = false;


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
    
    notificationLock.lock();
    userNotifications[username].push_back(notification);
    notificationLock.unlock();

    addFollowersPendingNotification(username, id);

    sessionLock.unlock();
}

void StorageManager::addFollowersPendingNotification(std::string username,int id) {
    //PRODUCER
    for (auto follower : userFollowers[username]){
        // pendingNotificationLock.lock();
        std::unique_lock<std::mutex> ul(pendingNotificationLock);

        userPendingNotifications[follower].push(std::make_tuple(username, id));
        
        ready = true;
        ul.unlock();
        cv.notify_all();
    }
}

Notification StorageManager::getUserPendingNotification(std::string username) {

    std::unique_lock<std::mutex> ul(pendingNotificationLock);
    
    cv.wait(ul, []() {
        return ready;
    });

    auto userPendingNotification = userPendingNotifications[username].front();

    std::string userInfluencer = std::get<0>(userPendingNotification);
    int id = std::get<1>(userPendingNotification);

    Notification notification;
    notification = getNotificationById(userInfluencer, id);
    
    userPendingNotifications[username].pop();

    std::cout << "1" << std::endl;
    ready = false;
    ul.unlock();
    std::cout << "2" << std::endl;

    return notification;
}

Notification StorageManager::getNotificationById(std::string username, int id) {
    notificationLock.lock();
    for (auto& notification : userNotifications[username]) {
        if (notification.id == id) {
            notification.pending--;
            return notification;
        }
    }
    notificationLock.unlock();
    std::cout << "ERROR getting notification" << std::endl;
}

// void StorageManager::decrementNotificationPending(std::string username, Notification notification) {
//     Notification notificationCopy = notification; 
//     notification.pending--;

//     notificationLock.lock();
//     std::replace(userNotifications[username].begin(),
//         userNotifications[username].end(),
//         notificationCopy,
//         notification
//         );
//     notificationLock.unlock();
// }

inline bool operator==(const Notification& lhs, const Notification& rhs) {
    return lhs.id == rhs.id;
}

inline bool operator<(const std::tuple<std::string, int>& lhs, const std::tuple<std::string, int>& rhs) {
    return std::get<1>(lhs) < std::get<1>(rhs);
}

inline bool operator<(const Notification& lhs, const Notification& rhs) {
  return lhs.id < rhs.id;
}

int StorageManager::getSeqn() {
    return seqn;
}

void StorageManager::incrementSeqn() {
    seqn++;
}
