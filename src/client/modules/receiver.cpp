#include "receiver.h"

void Receiver::start(Communication& communication) {
    Packet recv_packet;


    while(true) {
        recv_packet = communication.recvPacket();
        std::cout << "PAYLOAD" << recv_packet._payload << std::endl;
    }
}