#include "notificationThread.h"

#include <unistd.h>

void NotificationThread::sendNotificationsToUser(std::string username, int cli_sockfd, struct sockaddr_in cli_addr) {
    std::cout << "starting consumer" << std::endl;

    struct sockaddr_in user_cli_addr = ThreadManager::getCli_addr(username);

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
        CommunicationManager::sendPacket(notificationPacket, cli_sockfd, user_cli_addr);
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