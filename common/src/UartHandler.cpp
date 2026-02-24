#include "UartHandler.hpp"
#include "serial/serial.h"
#include <thread>

#define READ_TIMEOUT_CONSTANT 255 
#define READ_TIMEOUT_MULTIPLIER 0
#define WRITE_TIMEOUT_CONSTANT 255
#define WRITE_TIMEOUT_MULTIPLIER 0

UartHandler::UartHandler(std::string port, uint64_t baud_rate)
    : my_serial(port, baud_rate, serial::Timeout::simpleTimeout(1000))
{
    if(!my_serial.isOpen()){
        std::cout << "Connection problem\n";
        std::exit(0);
    }
    else{
        std::cout << "Succesful connection\n";
    }
}

size_t UartHandler::sendBytes(std::string bytes)
{
    my_serial.setTimeout(serial::Timeout::max(), READ_TIMEOUT_CONSTANT, READ_TIMEOUT_MULTIPLIER, WRITE_TIMEOUT_CONSTANT, WRITE_TIMEOUT_MULTIPLIER);
    
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