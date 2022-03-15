#include "notificationThread.h"

#include <unistd.h>

void NotificationThread::sendNotificationsToUser(std::string username) {

    while(true) {            
        Notification notificationToSend = StorageManager::getUserPendingNotification(username);

        Packet notificationPacket = {
            .type = DATA,
            .seqn = 0,
            .length = sizeof(Packet),
            .timestamp = std::time(0)
        };
        strcpy(notificationPacket._payload, notificationToSend._string);
        
        StorageManager::sendNotification(username, notificationPacket);
    }
}