#include "notificationThread.h"

#include <unistd.h>

void NotificationThread::sendNotificationsToUser(std::string username, int cli_sockfd, struct sockaddr_in cli_addr) {
    std::cout << "initiate consumer" << std::endl;
    
    while(true) {
        // std::set< std::tuple<std::string, int> > userPendingNotifications = StorageManager::getUserPendingNotifications(username);
        auto userPendingNotificationsList = StorageManager::getUserPendingNotifications(username);
        
        for(auto PendingNotification : userPendingNotificationsList) {
            Notification notificationToSend = StorageManager::getNotificationById( 
                std::get<0>(PendingNotification),
                std::get<1>(PendingNotification)
            );
            std::cout << "ID: " << notificationToSend.id << std::endl;

            Packet notificationPacket = {
                .type = DATA,
                .seqn = 0,
                .length = sizeof(Packet),
                .timestamp = std::time(0)
            };

            struct sockaddr_in user_cli_addr = ThreadManager::getCli_addr(username);
            strcpy(notificationPacket._payload, notificationToSend._string);
            std::cout << "NOTIFICATION PAYLOAD " << notificationPacket._payload << std::endl;
            CommunicationManager::sendPacket(notificationPacket, cli_sockfd, user_cli_addr);
        }
    }

    std::cout << "finishing consumer" << std::endl;
    // wait(resource)
    // <map<string, set<tupla>>> userPN = storageManager::getUserPendingNotifications(username);
    // for(auto PendingNotifications : userPN ) {
    //         PendingNotifications == tupla == string, id
    //      Notification notificationToSend = storageManager::getNotification(username, id);
    //      CommunicationManager::sendPacket(notificationToSend, cli_sockfd, cli_addr);
    // }
}