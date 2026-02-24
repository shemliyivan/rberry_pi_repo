#pragma once
#include <string>
#include <iostream>
#include "serial/serial.h"

class UartHandler{
    private:
        serial::Serial my_serial;
    public:
        UartHandler(std::string port, uint64_t baud_rate);

        size_t sendBytes(std::string bytes);
        std::string receiveBytes(int size = 100);
};