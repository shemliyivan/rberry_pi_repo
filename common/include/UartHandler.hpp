#pragma once
#include <string>
#include <iostream>
#include "serial/serial.h"

class UartHandler{
    private:
        serial::Serial my_serial;
    public:
        UartHandler(std::string port, uint64_t baud_rate)
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

        size_t sendBytes(std::string bytes);
        std::string receiveBytes(int size = 100);
};