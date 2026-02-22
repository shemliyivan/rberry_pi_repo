#pragma once
#include <string>

struct UartDevice{
    std::string path;
    unsigned long baud_rate;
};

UartDevice parser(std::string filename);