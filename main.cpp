#include <iostream>
#include <fstream>
#include <unistd.h>
#include "json/json.h"
#include "serial/serial.h"
#include <thread>

int main(){

    Json::Value root;

    std::ifstream ifs("../configuration.json");

    Json::CharReaderBuilder builder;

    JSONCPP_STRING errs;
    if(!Json::parseFromStream(builder, ifs, &root, &errs)){
        std::cout << errs << std::endl;
        return 1;
    }

    std::cout << root << std::endl;

    // Serial part
    unsigned long baud = std::stoul(root["baud_rate"].asString());
    std::string port   = root["filename"].asString();
    auto ports = serial::list_ports();

    for(int i = 0; i < ports.size(); ++i){
        std::cout << "(" << ports[i].port << ", " << ports[i].description << ", " << ports[i].hardware_id << ")\n"; 
    }

    // Initializing port handler
    serial::Serial my_serial(port, baud, serial::Timeout::simpleTimeout(1000));

    if(my_serial.isOpen()){
        std::cout << "Succesfull connection\n";
    }
    else{
        std::cout << "Something went wrong\n";
        return 0;
    }


    my_serial.setTimeout(serial::Timeout::max(), 250, 0, 250, 0);

    // Sending bytes
    std::string test("Some text for example\n");
    size_t bytes_send = my_serial.write(test);

    std::cout << "Bytes written: " << bytes_send << std::endl; 

    //Receiving bytes
    while(1){
        if(my_serial.available()){
            std::string bytes_received = my_serial.read(test.size() + 1);

            std::cout << "Bytes received: " << bytes_received.length() << ", " << "Result = " << bytes_received << std::endl;
        }
        else{
            std::cout << "Nothing received\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        }
    }

    return 0;
}