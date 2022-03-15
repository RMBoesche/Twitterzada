#ifndef storageManager_def
#define storageManager_def

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <queue>
#include <string.h>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "communicationManager.h"

#include "../../include/packet.h"
#include "../../include/notification.h"

//==============================================================================================
//                                        USER CLASS
//==============================================================================================

class User {
    std::string name;
    std::map<int, struct sockaddr_in> clientData;

    std::set<std::string> followers;
    std::vector<Notification> notifications;
    std::queue< std::tuple< std::string, int >> pendingNotifications;

    std::mutex sessionLock;
    std::mutex notificationLock;

    std::mutex pendingNotificationLock;
    std::condition_variable cv;
    bool ready;
    int produced;

public:
    User(std::string username);
    User(std::string username, int cli_sockfd, struct sockaddr_in cli_addr);

    std::string getUsername();
    std::set<std::string> getFollowers();
    std::map<int, struct sockaddr_in> getClientData();

    void addFollower(std::string newFollower);
    void addNotification(std::string message, int id);
    void addFollowersPendingNotification(int id);
    void addPendingNotification(std::string username, int id);
    
    void setClientData(int cli_sockfd, struct sockaddr_in cli_addr);
    void removeClient(int cli_sockfd);

    Notification getUserPendingNotification();
    Notification getNotificationById(int id);

    void startProduction();
    void endProduction();

    void updateFollowersFile(std::string);
};

//==============================================================================================
//                                     STORAGE MANAGER CLASS
//==============================================================================================

class StorageManager {
    static std::map<std::string, User*> users;

    static std::mutex saveLock;
    static int seqn;
    static int id;

public:
    static User* getUser(std::string username);
    static void setClientData(std::string username, int cli_sockfd, struct sockaddr_in& cli_addr);
    static int getSeqn();
    static void incrementSeqn();

    static void addUser(std::string username);
    static void addUser(std::string username, int cli_sockfd, struct sockaddr_in& cli_addr);
    static void addFollower(std::string username, std::string follower);
    static void addNotification(std::string username, std::string message);
    static void addFollowersPendingNotification(std::string username, int id);

    static Notification getUserPendingNotification(std::string username);
    static Notification getNotificationById(std::string username, int id);

    static void sendNotification(std::string username, Packet notificationPacket);
    static void removeClient(std::string username, int cli_sockfd);
    
    static void recoverState();
    static void saveState();
};

#endif