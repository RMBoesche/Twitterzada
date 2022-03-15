#ifndef notificationThread_def
#define notificationThread_def

#include "storageManager.h"
#include "communicationManager.h"
#include "threadManager.h"
#include "../../include/notification.h"

class NotificationThread {
public:
    static void sendNotificationsToUser(std::string username);
};

#endif