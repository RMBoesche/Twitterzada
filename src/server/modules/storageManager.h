#ifndef storageManager
#define storageManager

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <string.h>
#include <mutex>

#include "../../include/packet.h"
#include "../../include/notification.h"

class StorageManager {
    static std::map<std::string, std::set<std::string>> userFollowers;
    static std::map<std::string, std::set<Notification>> userNotifications;
    static std::map<std::string, std::set< std::tuple< std::string, int >>> userPendingNotifications;

    static std::mutex sessionLock;

    static int seqn;
    static int id;


public:
    static int getSeqn();
    static void incrementSeqn();
    static void addFollower(std::string username, std::string follower);
    static void addNotification(std::string username, std::string message);
    static void addUserPendingNotification(std::string username, int id);
};

#endif