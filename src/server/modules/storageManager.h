#ifndef storageManager
#define storageManager

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <set>

#include "../../include/packet.h"

class StorageManager {
    static std::map<std::string, std::set<std::string>> userFollowers;
    static std::map<std::string, std::set<std::string>> userReceive;
    static std::map<std::string, std::set<std::string>> userToSend;

    static int seqn;

public:
    static int getSeqn();
    static void incrementSeqn();
    static void addFollower(std::string user, std::string follower);
};

#endif