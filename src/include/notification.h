#include <netinet/in.h>

#ifndef NOTIFICATION
#define NOTIFICATION

typedef struct __notification
{
    uint32_t id;         // Identificador da notificação (sugere-se um identificador único)
    uint32_t timestamp;  // Timestamp da notificação
    uint16_t length;     // Tamanho da mensagem
    uint16_t pending;    // Quantidade de leitores pendentes
    char _string[128]; // Mensagem
} Notification;

#endif