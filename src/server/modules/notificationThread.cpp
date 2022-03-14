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
        
        struct sockaddr_in user_cli_addr = ThreadManager::getCli_addr(username);
        std::cout << "NOTIFICATION PAYLOAD " << notificationPacket._payload << std::endl;
        CommunicationManager::sendPacket(notificationPacket, cli_sockfd, user_cli_addr);
    }

    std::cout << "finishing consumer" << std::endl;
}