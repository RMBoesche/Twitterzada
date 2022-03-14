#include "storageManager.h"

//==============================================================================================
//                                      USER CLASS
//==============================================================================================

User::User(std::string username) 
    : name(username)
{}

void User::addFollower(std::string newFollower) {
    sessionLock.lock();
    followers.insert(newFollower);
    sessionLock.unlock();
}

void User::addNotification(std::string message, int id) {
    
    Notification notification = {
        .id = id,
        .timestamp = std::time(0),
        .length = message.length(),
        .pending = followers.empty() ? 0 : followers.size(),
    };

    strcpy(notification._string, message.c_str());

    notificationLock.lock();
    notifications.push_back(notification);
    notificationLock.unlock();

    addFollowersPendingNotification(id);
}

void User::addFollowersPendingNotification(int id) {
    sessionLock.lock();
    for (auto follower : followers){
        StorageManager::getUser(follower)->startProduction();

        std::cout << "pre pending notification" << std::endl;

        StorageManager::getUser(follower)->addPendingNotification(name, id);

        std::cout << "pos pending notification" << std::endl;

        StorageManager::getUser(follower)->endProduction();
    }
    sessionLock.unlock();
}

void User::startProduction() {
    pendingNotificationLock.lock();
}

void User::endProduction() {
    produced++;
    ready = true;
    pendingNotificationLock.unlock();
    cv.notify_all();
}

void User::addPendingNotification(std::string username, int id) {
    pendingNotifications.push(std::make_tuple(username, id));
}

Notification User::getUserPendingNotification() {
    
    std::unique_lock<std::mutex> ul(pendingNotificationLock);
    while(produced == 0) {
        cv.wait(ul, [&]() {
            return ready;
        });
    }

    // if(!(pendingNotifications.empty())) {

        // get the first message to be sent
        auto pendingNotification = pendingNotifications.front();
        // remove it from the list
        pendingNotifications.pop();

        std::string userInfluencer = std::get<0>(pendingNotification);
        int id = std::get<1>(pendingNotification);

        Notification notification;
        std::cout << "pre get notification" << std::endl;
        std::cout << userInfluencer << " " << id << std::endl;
        
        notification = StorageManager::getUser(userInfluencer)->getNotificationById(id);
        
        std::cout << "pos get notification" << std::endl;
        
        produced--;
        if(produced == 0) {
            ready = false;
        }
        ul.unlock();

        return notification;
    // }
    std::cout << "NÃO TEM NOTIFICACAO" << std::endl;
    ul.unlock();
}

Notification User::getNotificationById(int id) {

    notificationLock.lock();
    for (auto& notification : notifications) {
        if ((notification.id == id) && (notification.pending > 0)) {
            notification.pending--;
            notificationLock.unlock();
            return notification;
        }
    }
    notificationLock.unlock();
    std::cout << "ERROR getting notification" << std::endl;
}

inline bool operator==(const Notification& lhs, const Notification& rhs) {
    return lhs.id == rhs.id;
}

inline bool operator<(const std::tuple<std::string, int>& lhs, const std::tuple<std::string, int>& rhs) {
    return std::get<1>(lhs) < std::get<1>(rhs);
}

inline bool operator<(const Notification& lhs, const Notification& rhs) {
  return lhs.id < rhs.id;
}

//==============================================================================================
//                                     STORAGE MANAGER CLASS
//==============================================================================================


int StorageManager::seqn = 0;
int StorageManager::id = 0;
std::map<std::string, User*> StorageManager::users;

void StorageManager::addUser(std::string username) {
    if(users.find(username) == users.end()) {
        users.insert({username, new User(username)});
    }
}

void StorageManager::addFollower(std::string username, std::string follower) {
    if(users.find(username) == users.end())
        addUser(username);
    users[username]->addFollower(follower);
}

void StorageManager::addNotification(std::string username, std::string message) {
    id++;
    users[username]->addNotification(message, id);
}

Notification StorageManager::getUserPendingNotification(std::string username) {
    return users[username]->getUserPendingNotification();

}

User* StorageManager::getUser(std::string username) {
    return users[username];
}

int StorageManager::getSeqn() {
    return seqn;
}

void StorageManager::incrementSeqn() {
    seqn++;
}


