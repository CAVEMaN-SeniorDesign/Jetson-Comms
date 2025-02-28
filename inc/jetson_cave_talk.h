#ifndef JETSON_CAVE_TALK_H
#define JETSON_CAVE_TALK_H

#include <string>
#include <stddef.h>

#include "cave_talk_link.h"

#define PORT "/dev/ttyUSB0"
#define BAUD_RATE B1000000

extern bool port_open;
extern int serial_port;
namespace cave_talk
{
CaveTalk_Error_t init();
CaveTalk_Error_t deinit();
CaveTalk_Error_t flush();
CaveTalk_Error_t send(const void *const data, const size_t size);
CaveTalk_Error_t receive(void *const data, const size_t size, size_t *const bytes_received);
CaveTalk_Error_t available(size_t *const bytes);
}
#endif  // JETSON_CAVE_TALK_H
