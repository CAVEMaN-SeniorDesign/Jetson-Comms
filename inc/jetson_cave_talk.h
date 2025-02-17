#ifndef JETSON_CAVE_TALK_H
#define JETSON_CAVE_TALK_H

#include <string>

#include "cave_talk_link.h"

#define PORT_OPEN 0
#define PORT "/dev/ttyUSB0"
#define BAUD_RATE B115200



CaveTalk_Error_t init(std::string port);

typedef struct
{
    CaveTalk_Error_t (*send)(const void *const data, const size_t size);
    CaveTalk_Error_t (*receive)(void *const data, const size_t size, size_t *const bytes_received);
    CaveTalk_Error_t (*available)(size_t *const bytes);
} CaveTalk_LinkHandle_t;


#endif