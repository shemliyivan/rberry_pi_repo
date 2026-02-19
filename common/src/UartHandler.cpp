#include "UartHandler.hpp"
#include "serial/serial.h"
#include <thread>

size_t UartHandler::sendBytes(std::string bytes)
{
    my_serial.setTimeout(serial::Timeout::max(), 250, 0, 250, 0);
    
    return my_serial.write(bytes);
}

std::string UartHandler::receiveBytes(int size)
{
    int count = 0;

    while(count < 10){
        if(my_serial.available()){
            return std::string(my_serial.read(size));
        }
        else{
            std::cout << "Nothing received\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            ++count;
        }
    }   
}