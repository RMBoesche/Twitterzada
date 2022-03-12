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

#include "../../include/packet.h"
#include "../../include/notification.h"

class StorageManager {
    static std::map<std::string, std::set<std::string>> userFollowers;
    static std::map<std::string, std::vector<Notification>> userNotifications;
    static std::map<std::string, std::queue< std::tuple< std::string, int >>> userPendingNotifications;

    static std::mutex pendingNotificationLock;
    static std::mutex sessionLock;
    static std::mutex notificationLock;
    static std::condition_variable cv;
    static bool ready;

    static int seqn;
    static int id;

public:
    static int getSeqn();
    static void incrementSeqn();
    static void addFollower(std::string username, std::string follower);
    static void addNotification(std::string username, std::string message);
    static void addFollowersPendingNotification(std::string username, int id);
    static Notification getUserPendingNotification(std::string username);
    static Notification getNotificationById(std::string username, int id);
    static void decrementNotificationPending(std::string userInfluencer, Notification notification);
};

#endif