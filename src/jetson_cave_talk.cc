#include "jetson_cave_talk.h"

#ifndef PORT
#define PORT "/dev/ttyUSB0"
#endif

#ifndef BAUD_RATE
#define BAUD_RATE B1000000
#endif

bool port_open = false;
int serial_port;

namespace cave_talk
{

CaveTalk_Error_t init(){
    std::cout << "Trying to open port: "<< PORT << std::endl;

    CaveTalk_Error_t error = CAVE_TALK_ERROR_NONE;

    serial_port = open(PORT, O_RDWR | O_NONBLOCK);
	
	if((serial_port < 0) && (error == CAVE_TALK_ERROR_NONE)){
		std::cout << "Couldn't open port" << std::endl;
		error = CAVE_TALK_ERROR_SOCKET_CLOSED;
	}
	
    // flushing buffers
    tcflush(serial_port, TCIFLUSH);
    tcflush(serial_port, TCIOFLUSH);
    usleep(1000000);  // 1 sec delay

	struct termios tty;
	
	if((tcgetattr(serial_port, &tty) != 0) && (error == CAVE_TALK_ERROR_NONE)){
		std::cout << "error from tcgetattr " << tcgetattr(serial_port, &tty) << std::endl;
			
		error = CAVE_TALK_ERROR_SOCKET_CLOSED;
	}

    if(error == CAVE_TALK_ERROR_NONE){
        tty.c_cflag &= ~PARENB; // Disables parity.
        tty.c_cflag &= ~CSTOPB; // Stop bit: CSTOPB = 2 stop bits, clearing is 1 stop bit. (Set two stop bits as a test)
        tty.c_cflag &= ~CSIZE; // Character size mask.  This clears the data size mask
        tty.c_cflag |= CS8; // Set the data bits = 8
        tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (Check if MCU has RTS and CTS pins)
        tty.c_cflag |= CREAD | CLOCAL; // Enable receiver, ignore modem control lines
        
        // Clearing these bits sets "Non-Cannonical mode", i.e. disables Cannonical mode that continues to read until a new-line character is read.
        tty.c_lflag &= ~ICANON; 
        tty.c_lflag &= ~ECHO; 
        tty.c_lflag &= ~ECHOE; 
        tty.c_lflag &= ~ECHONL; 
        tty.c_lflag &= ~ISIG;
        
        tty.c_lflag &= ~(IXON | IXOFF | IXANY); // Disables XON/XOFF flow control on both input & output
        tty.c_lflag &= ~(IGNBRK | BRKINT| PARMRK| ISTRIP| INLCR| IGNCR| ICRNL); // Disables special handling of received bytes
        tty.c_oflag &= ~OPOST; // Disables output processing
        tty.c_oflag &= ~ONLCR; // Disable mapping NL char to CR-NL
        tty.c_cc[VTIME] = 0; //  timeout in deciseconds for non-canonical read, 0 is wait indefinitely
        tty.c_cc[VMIN] = 0; // min number of characters to read, before returning from read()
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

    // flushing buffers
    tcflush(serial_port, TCIFLUSH);
    tcflush(serial_port, TCIOFLUSH);
    usleep(1000000);  // 1 sec delay

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

        sleep(1);
        if(tcflush(serial_port, TCIOFLUSH) != 0) {
            error = CAVE_TALK_ERROR_SOCKET_CLOSED;
        }

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

    if(port_open)
    {
        int bytes_read = read(serial_port, data, size);

        if (-1 == bytes_read)
        {
            if ((errno != EAGAIN) && (errno != EWOULDBLOCK))
            {
                error = CAVE_TALK_ERROR_INCOMPLETE;
            }
        }
        else if (0 == bytes_read)
        {
            port_open = false;
            *bytes_received = 0U;
            error = CAVE_TALK_ERROR_SOCKET_CLOSED;
        }
        else
        {
            *bytes_received = (size_t)bytes_read;
        }
    }
    else
    {
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