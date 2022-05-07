#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "../include/packet.h"
#include "modules/communicationManager.h"
#include "modules/sessionManager.h"
#include "modules/mainSocket.h"
#include "modules/userThread.h"
#include "modules/storageManager.h"
#include "modules/threadManager.h"

#define MAIN_PORT 4000
#define N_SERVERS_R 5
#define AUX_PORT 4006