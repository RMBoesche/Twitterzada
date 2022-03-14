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

// #include "user.h"

#include "../../include/packet.h"
#include "../../include/notification.h"

class User {
    std::set<std::string> followers;
    std::vector<Notification> notifications;
    std::queue< std::tuple< std::string, int >> pendingNotifications;
    std::mutex sessionLock;
    std::mutex notificationLock;
    std::mutex pendingNotificationLock;
    std::condition_variable cv;
    bool ready;
    int produced;
    std::string name;

public:
    User(std::string username);
    void addFollower(std::string newFollower);
    void addNotification(std::string message, int id);
    void addFollowersPendingNotification(int id); //*
    void addPendingNotification(std::string username, int id); //*
    Notification getUserPendingNotification();
    Notification getNotificationById(int id);
    void startProduction();
    void endProduction();
};

class StorageManager {

    static std::map<std::string, User*> users;

    static int seqn;
    static int id;

public:
    static int getSeqn();
    static void addUser(std::string username);
    static void incrementSeqn();
    static void addFollower(std::string username, std::string follower);
    static void addNotification(std::string username, std::string message);
    static void addFollowersPendingNotification(std::string username, int id);
    static Notification getUserPendingNotification(std::string username);
    static Notification getNotificationById(std::string username, int id);
    static User* getUser(std::string username);
};


#endif