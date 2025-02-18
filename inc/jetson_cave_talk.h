#ifndef JETSON_CAVE_TALK_H
#define JETSON_CAVE_TALK_H

#include <string>

#include "cave_talk_link.h"

#define PORT "/dev/ttyUSB0"
#define BAUD_RATE B115200

bool port_open = false;
int serial_port;


CaveTalk_Error_t init(std::string port);
CaveTalk_Error_t deinit();
CaveTalk_Error_t flush();


#endif