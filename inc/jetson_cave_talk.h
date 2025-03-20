#ifndef JETSON_CAVE_TALK_H
#define JETSON_CAVE_TALK_H

#include "cave_talk_link.h"

#include <string>
#include <stddef.h>
#include <stdio.h>
#include <iostream>
#include <cstring>

#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>



extern bool port_open;
extern int serial_port;
extern std::string target_port;
extern speed_t baud_rate;

namespace cave_talk
{
    inline void setBaud(speed_t new_baud) {
        baud_rate = new_baud;
    }

    inline void setTargetPort(const std::string& new_port) {
        target_port = new_port;
    }
    CaveTalk_Error_t init();
    CaveTalk_Error_t deinit();
    CaveTalk_Error_t flush();
    CaveTalk_Error_t send(const void *const data, const size_t size);
    CaveTalk_Error_t receive(void *const data, const size_t size, size_t *const bytes_received);
    CaveTalk_Error_t available(size_t *const bytes);
}

#endif  // JETSON_CAVE_TALK_H