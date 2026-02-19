#pragma once
#include <string>

class UartDevice{
    public:
        std::string path;
        unsigned long baud_rate;

        UartDevice(std::string path, unsigned long baud_rate)
            : path(path), baud_rate(baud_rate)
        {

        }
};

UartDevice parser(std::string filename);