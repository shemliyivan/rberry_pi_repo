#include "JsonParser.hpp"
#include <fstream>
#include <iostream>
#include "json/json.h"

UartDevice parser(std::string& filename)
{
    Json::Value root;

    std::ifstream str(filename);

    Json::CharReaderBuilder builder;

    JSONCPP_STRING errs;

    if(!Json::parseFromStream(builder, str, &root, &errs)){
        std::cout << errs << std::endl;
        std::exit(0);
    }
    
    if (!root.isMember("path") || !root.isMember("baud_rate")) {
        std::cerr << "Помилка відсутні дані." << std::endl;
        std::exit(1);
    }

    return UartDevice{root["path"].asString(), root["baud_rate"].asUInt()};
}