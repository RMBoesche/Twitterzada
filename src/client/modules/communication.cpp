#include "communication.h"
#include "interface.h"

Communication::Communication(std::string &name, int port, struct hostent *server)
    : username(name), port(port), server(server)
{
    establishConnection();
}

int Communication::establishConnection()
{
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        std::cout << "ERROR opening socket" << std::endl;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
    bzero(&(serv_addr.sin_zero), 8);

    // clean buffer
    bzero(buffer, 256);

    std::string initializer = username;
    strcpy(buffer, initializer.c_str());

    // initialize structure for first communication
    packet establish_packet = {
        .type = ESTABLISHMENT,
        .seqn = seqn,
        .length = strlen(buffer),
        .timestamp = std::time(0)
    };
    strcpy(establish_packet._payload, buffer);

    // sends an initial message
    int n = sendto(
        sockfd,
        (packet *)&establish_packet,
        sizeof(establish_packet),
        0,
        (const struct sockaddr *)&serv_addr,
        sizeof(struct sockaddr_in));
    if (n < 0)
        std::cout << "ERROR sendto" << std::endl;

    return 1;
}

int Communication::sendMessage(std::string cli_message, int type)
{
    // update sequence number
    seqn++;

    bzero(buffer, 256);

    strcpy(buffer, cli_message.c_str());

    packet communication_packet = {
        .type = type,
        .seqn = seqn,
        .length = strlen(buffer),
        .timestamp = std::time(0)
    };

    strcpy(communication_packet._payload, buffer);
    
    int n = sendto(
        sockfd,
        &communication_packet,
        sizeof(communication_packet),
        0,
        (const struct sockaddr *)&serv_addr,
        sizeof(struct sockaddr_in)
    );

    if (n < 0)
    {
        std::cout << "ERROR sendto aqui" << std::endl;
        return 0;
    }

    return 1;
}

int Communication::getSocket()
{
    return sockfd;
}

void Communication::setPort(int port)
{
    serv_addr.sin_port = htons(port);
}

packet Communication::recvPacket(int sockfd, struct sockaddr_in* cli_addr) {
    
    socklen_t clilen = sizeof(struct sockaddr_in);
    packet recv_packet;

    int n = recvfrom(
        sockfd,
        &recv_packet,
        sizeof(packet),
        0,
        (struct sockaddr *) cli_addr,
        &clilen
    );
    if (n < 0)
		printf("ERROR on recvfrom");
    return recv_packet;
}

void Communication::recvPort()
{
    packet recv_packet = recvPacket(sockfd, &serv_addr);

    int new_port = atoi(recv_packet._payload);
    std::cout << new_port;
    setPort(new_port);
}

Communication::~Communication()
{
    sendMessage(std::string("END"), ESTABLISHMENT);

    close(sockfd);
}