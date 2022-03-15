#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "../../include/packet.h"
#include "communication.h"

class Receiver {
    struct sockaddr_in from;
public:
    static void start(Communication& communication, int& exit);
};
