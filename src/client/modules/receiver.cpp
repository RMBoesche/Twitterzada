#include "receiver.h"

void Receiver::start(Communication& communication, int& exit) {
    Packet recv_packet;

    while(!exit) {
        recv_packet = communication.recvPacket();
        std::cout << "\n\nNOTIFICATION: " << recv_packet._payload << std::endl;
    }
}