#include <iostream>
#include <fstream>
#include <unistd.h>
#include "JsonParser.hpp"
#include "UartHandler.hpp"

int main(){

    UartDevice DevInfo = parser("../configuration.json");

    UartHandler my_serial(DevInfo.path, DevInfo.baud_rate);

    auto bytes_sended = my_serial.sendBytes("Hello from Ivan");

    std::cout << "Bytes sended: " << bytes_sended << std::endl;


    std::cout << "Received message: " << my_serial.receiveBytes(bytes_sended) << std::endl;

    return 0;
}