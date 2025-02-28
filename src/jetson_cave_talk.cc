#include "jetson_cave_talk.h"

#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stddef.h>

#include <string>
#include <iostream>

bool port_open = false;
int serial_port;

namespace cave_talk
{

CaveTalk_Error_t init(){

    CaveTalk_Error_t error = CAVE_TALK_ERROR_NONE;

    serial_port = open(PORT, O_RDWR | O_NONBLOCK);
	
	if((serial_port < 0) && (error == CAVE_TALK_ERROR_NONE)){
		std::cout << "Couldn't open port" << std::endl;
		error = CAVE_TALK_ERROR_SOCKET_CLOSED;
	}
	
	struct termios tty;
	
	if((tcgetattr(serial_port, &tty) != 0) && (error == CAVE_TALK_ERROR_NONE)){
		std::cout << "error from tcgetattr " << tcgetattr(serial_port, &tty) << std::endl;
			
		error = CAVE_TALK_ERROR_SOCKET_CLOSED;
	}

    if(error == CAVE_TALK_ERROR_NONE){

        tty.c_cflag &= ~PARENB;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CSIZE;
        tty.c_cflag |= CS8;
        tty.c_cflag &= ~CRTSCTS;
        tty.c_cflag |= CREAD | CLOCAL;
        
        tty.c_lflag &= ~ICANON;
        tty.c_lflag &= ~ECHO;
        tty.c_lflag &= ~ECHOE;
        tty.c_lflag &= ~ECHONL;
        tty.c_lflag &= ~ISIG;
        tty.c_lflag &= ~(IXON | IXOFF | IXANY);
        tty.c_lflag &= ~(IGNBRK | BRKINT| PARMRK| ISTRIP| INLCR| IGNCR| ICRNL);
        
        tty.c_oflag &= ~OPOST;
        tty.c_oflag &= ~ONLCR;
        
        tty.c_cc[VTIME] = 10;
        tty.c_cc[VMIN] = 0;

    }
	
	
	
	if( (cfsetispeed(&tty, BAUD_RATE) != 0) || (cfsetospeed(&tty, BAUD_RATE) != 0) && (error == CAVE_TALK_ERROR_NONE)){
        std::cout << "Error setting baud rate" << std::endl;

        error = CAVE_TALK_ERROR_SOCKET_CLOSED;
    }
	
	if((tcsetattr(serial_port, TCSANOW, &tty) != 0) && (error == CAVE_TALK_ERROR_NONE)) {
		std::cout << "Error from tcsetattr " << tcsetattr(serial_port, TCSANOW, &tty) << std::endl;
		
		error = CAVE_TALK_ERROR_SOCKET_CLOSED;
	}

    if(error != CAVE_TALK_ERROR_NONE){
        close(serial_port);
        port_open = false;
    }else{
        port_open = true;
    }
	
    if(error == CAVE_TALK_ERROR_NONE){
        std::cout << "Port initialized with Baud: " << std::endl;
        std::cout << BAUD_RATE << std::endl;

    }
    return error;
}


CaveTalk_Error_t deinit(){

    if(port_open){
        close(serial_port);
    }

    port_open = false;

    return CAVE_TALK_ERROR_SOCKET_CLOSED;

}


CaveTalk_Error_t flush(){

    CaveTalk_Error_t error = CAVE_TALK_ERROR_NONE;

    if(port_open){
        size_t bytes_available = 0;
        char throwaway = '!';
        size_t bytes_received = 0;

        available(&bytes_available);


        while(bytes_available > 0){
            receive(&throwaway, 1, &bytes_received);
            available(&bytes_available);
            //std::cout << (bytes_available) << std::endl;
        }

    }else{
        error = CAVE_TALK_ERROR_SOCKET_CLOSED;
    }

    return error;

}


CaveTalk_Error_t send(const void *const data, const size_t size){

    CaveTalk_Error_t error = CAVE_TALK_ERROR_NONE;
    
    if(port_open){
        size_t bytes_written = write(serial_port, data, size);

        if(bytes_written < 0){
            error = CAVE_TALK_ERROR_SOCKET_CLOSED;
        }else if(bytes_written < size){
            error = CAVE_TALK_ERROR_SIZE;
        }

    }else{
        error = CAVE_TALK_ERROR_SOCKET_CLOSED;
    }

    return error;

}


CaveTalk_Error_t receive(void *const data, const size_t size, size_t *const bytes_received){

    CaveTalk_Error_t error = CAVE_TALK_ERROR_NONE;

    if(port_open){

        *bytes_received = read(serial_port, data, size);

        if(*bytes_received != size) error = CAVE_TALK_ERROR_SIZE;

    }else{
        error = CAVE_TALK_ERROR_SOCKET_CLOSED;
    }

    return error;

}


CaveTalk_Error_t available(size_t *const bytes){

    CaveTalk_Error_t error = CAVE_TALK_ERROR_NONE;

    if(!port_open){
        error = CAVE_TALK_ERROR_SOCKET_CLOSED;
    }

    if(ioctl(serial_port, FIONREAD, bytes) == -1){

        error = CAVE_TALK_ERROR_SOCKET_CLOSED;

    }

    return error;
        
    
}

}//namespace cave_talk