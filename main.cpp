#include <iostream>
#include <fstream>
#include <unistd.h>
#include <thread>
#include "JsonParser.hpp"
#include "UartHandler.hpp"
#include "ThreadSafeQueue.hpp"
#include "UartDevice.hpp"

int main(){

    std::string path_to_json("../configuration.json");

    UartDevice DevInfo = parser(path_to_json);
    UartHandler my_serial(DevInfo.path, DevInfo.baud_rate);

    auto bytes_sended = my_serial.sendBytes("Hello from Ivan");
    std::cout << "Bytes sended: " << bytes_sended << std::endl;

    ThreadSafeQueue threadsafe_queue;

    std::thread receiver_message([&my_serial, &threadsafe_queue](){
        while(1){
            std::string received_bytes = my_serial.receiveBytes();

            if(received_bytes.length() > 1){
                threadsafe_queue.push(received_bytes);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
    });

    std::thread processor_message([&threadsafe_queue](){
        while(1){
            std::cout << "New message:\n";
            std::cout << threadsafe_queue.pop() << std::endl; 
        }
    });

    receiver_message.join();
    processor_message.join();

    return 0;
}