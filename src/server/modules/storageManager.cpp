#include "storageManager.h"

//==============================================================================================
//                                      USER CLASS
//==============================================================================================

User::User(std::string username) 
    : name(username)
{}

User::User(std::string username, int cli_sockfd, struct sockaddr_in cli_addr) 
    : name(username)
{
    this->clientData.insert({cli_sockfd, cli_addr});
}

std::map<int, struct sockaddr_in> User::getClientData() {
    return clientData;
}

void User::addFollower(std::string newFollower) {
    sessionLock.lock();
    followers.insert(newFollower);

    // updateFollowersFile();

    sessionLock.unlock();
}

void User::addNotification(std::string message, int id) {
    
    Notification notification = {
        .id = id,
        .timestamp = std::time(0),
        .length = message.length(),
        .pending = followers.empty() ? 0 : followers.size(),
    };
    strcpy(notification._string, (name + ": " + message).c_str());

    std::cout << "initial pending: " << notification.pending << std::endl;

    notificationLock.lock();
    notifications.push_back(notification);
    notificationLock.unlock();

    addFollowersPendingNotification(id);
}

void User::addFollowersPendingNotification(int id) {
    sessionLock.lock();

    for (auto follower : followers){
        StorageManager::getUser(follower)->startProduction();

        StorageManager::getUser(follower)->addPendingNotification(name, id);

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

    if(!clientData.empty()) {


        // get the first message to be sent
        auto pendingNotification = pendingNotifications.front();
        // remove it from the list
        pendingNotifications.pop();

        std::string userInfluencer = std::get<0>(pendingNotification);
        int id = std::get<1>(pendingNotification);

        Notification notification;
        
        notification = StorageManager::getUser(userInfluencer)->getNotificationById(id);
        
        produced--;
        if(produced == 0) {
            ready = false;
        }
        ul.unlock();

        return notification;
    }
}

Notification User::getNotificationById(int id) {

    notificationLock.lock();
    for (auto& notification : notifications) {
        if ((notification.id == id) && (notification.pending > 0)) {
            notification.pending--;
            std::cout << "pending " << notification.pending << std::endl;
            notificationLock.unlock();
            return notification;
        }
    }
    notificationLock.unlock();
    std::cout << "ERROR getting notification" << std::endl;
}

void User::removeClient(int sockfd) {
    clientData.erase(
        clientData.find(sockfd)
    );
}

void User::setClientData(int cli_sockfd, struct sockaddr_in cli_addr) {
    clientData.insert({cli_sockfd, cli_addr});
}

std::string User::getUsername() {
    return name;
}

std::set<std::string> User::getFollowers() {
    return followers;
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
std::mutex StorageManager::saveLock;

void StorageManager::addUser(std::string username) {
    if(users.find(username) == users.end()) {
        users.insert({username, new User(username)});
    }
}

void StorageManager::addUser(std::string username, int cli_sockfd, struct sockaddr_in& cli_addr) {
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

void StorageManager::sendNotification(std::string username, Packet notificationPacket) {
    auto clientData = users[username]->getClientData();
    for (auto data = clientData.begin(); data != clientData.end(); ++data) {
        // std::cout << data->first << ntohs(data->second.sin_port) << std::endl; 
        CommunicationManager::sendPacket(notificationPacket, data->first, data->second);
    }

}

void StorageManager::removeClient(std::string username, int cli_sockfd) {
    users[username]->removeClient(cli_sockfd);
}

void StorageManager::setClientData(std::string username, int cli_sockfd, struct sockaddr_in& cli_addr) {
    users[username]->setClientData(cli_sockfd, cli_addr);
}

void StorageManager::recoverState() {
    // ifstream followersFile("../../database.csv");
    std::ifstream followersFile("database.txt");
    std::string line;

    if(followersFile.is_open()) {
        while(getline(followersFile, line)) {

            std::string username = line.substr(0, line.find(','));

            std::string stringFollowers = line.substr(line.find('['), line.find(']'));

            // if there is at least one follower
            if(stringFollowers.length() > 2) {
                stringFollowers = stringFollowers.substr(1, stringFollowers.size()-2);

                std::istringstream streamFollowers(stringFollowers);
                std::string follower;
                while(getline(streamFollowers, follower, ',')) {
                    
                    addFollower(username, follower);
                }
            }
        }
    }
    else {
        std::cout << "ERRO LENDO ARQUIVO" << std::endl;
    }
    followersFile.close();
}

void StorageManager::saveState() {
    saveLock.lock();

    std::ifstream followersFile("database.txt");
    std::ofstream newFile("temporaryFile.txt");

    std::string line;
    if(followersFile.is_open() && newFile.is_open()) {
        for(auto const& user : users) {
            line = user.second->getUsername() + ",[";
            
            for (auto follower : user.second->getFollowers()) 
                line += follower + ",";   

            if(line[line.length() - 1] != '[')
                line = line.substr(0, line.size()-1);
            
            line += "]\n";

            newFile << line;
        }
        rename("temporaryFile.txt", "database.txt");
    }
    followersFile.close();
    newFile.close();

    saveLock.unlock();
}