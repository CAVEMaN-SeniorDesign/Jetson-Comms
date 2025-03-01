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


// #define PORT "/dev/ttyUSB1"
// #define BAUD_RATE B1000000

#define CBAUDEX 0010000
#define    BOTHER 0010000
#define    B57600 0010001
#define   B115200 0010002
#define   B230400 0010003
#define   B460800 0010004
#define   B500000 0010005
#define   B576000 0010006
#define   B921600 0010007
#define  B1000000 0010010
#define  B1152000 0010011
#define  B1500000 0010012
#define  B2000000 0010013
#define  B2500000 0010014
#define  B3000000 0010015
#define  B3500000 0010016
#define  B4000000 0010017

// #define PORT "/dev/ttyUSB1"
// #define BAUD_RATE B1000000

extern bool port_open;
extern int serial_port;
char target_port[20] = "/dev/ttyUSB1";
speed_t baud_rate = B1000000;

namespace cave_talk
{
void setBaud(speed_t new_baud);
void setTargetPort(const char* new_port);
CaveTalk_Error_t init();
CaveTalk_Error_t deinit();
CaveTalk_Error_t flush();
CaveTalk_Error_t send(const void *const data, const size_t size);
CaveTalk_Error_t receive(void *const data, const size_t size, size_t *const bytes_received);
CaveTalk_Error_t available(size_t *const bytes);
}
#endif  // JETSON_CAVE_TALK_H
