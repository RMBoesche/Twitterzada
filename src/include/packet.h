#include <netinet/in.h>
#include <ctime>

#ifndef PACKET
#define PACKET

#define ESTABLISHMENT 0
#define DATA 1
#define CMD 2

typedef struct __packet
{
    uint16_t type;        // Tipo do pacote (p.ex. DATA | CMD)
    uint16_t seqn;        // Número de sequência
    uint16_t length;      // Comprimento do payload
    uint16_t timestamp;   // Timestamp do dado
    char _payload[256]; // Dados da mensagem
} packet;

#endif