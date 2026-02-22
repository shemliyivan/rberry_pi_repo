#include "UartHandler.hpp"
#include "serial/serial.h"
#include <thread>

#define READ_TIMEOUT_CONSTANT 255 
#define READ_TIMEOUT_MULTIPLIER 0
#define WRITE_TIMEOUT_CONSTANT 255
#define WRITE_TIMEOUT_MULTIPLIER 0

size_t UartHandler::sendBytes(std::string bytes)
{
    my_serial.setTimeout(serial::Timeout::max(), 250, 0, 250, 0);
    
    return my_serial.write(bytes);
}

std::string UartHandler::receiveBytes(int size)
{
    if(my_serial.available()){
        return std::string(my_serial.read(size));
    }
    else{
        return std::string("");
    }
}