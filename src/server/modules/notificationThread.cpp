#include "notificationThread.h"

#include <unistd.h>

void NotificationThread::sendNotificationsToUser(std::string username, int cli_sockfd, struct sockaddr_in cli_addr) {
    std::cout << "starting consumer" << std::endl;

    while(true) {            
        Notification notificationToSend = StorageManager::getUserPendingNotification(username);

        Packet notificationPacket = {
            .type = DATA,
            .seqn = 0,
            .length = sizeof(Packet),
            .timestamp = std::time(0)
        };
        strcpy(notificationPacket._payload, notificationToSend._string);
        
        std::cout << "NOTIFICATION PAYLOAD " << notificationPacket._payload << std::endl;
        
        StorageManager::sendNotification(username, notificationPacket);
    }

    std::cout << "finishing consumer" << std::endl;
}