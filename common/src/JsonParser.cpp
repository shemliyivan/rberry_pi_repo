#include "JsonParser.hpp"
#include <fstream>
#include <iostream>
#include "json/json.h"

UartDevice parser(std::string filename)
{
    Json::Value root;

    std::ifstream str(filename);

    Json::CharReaderBuilder builder;

    JSONCPP_STRING errs;

    if(!Json::parseFromStream(builder, str, &root, &errs)){
        std::cout << errs << std::endl;
        std::exit(0);
    }
    
    return UartDevice(root["path"].asString(), root["baud_rate"].asUInt());
}