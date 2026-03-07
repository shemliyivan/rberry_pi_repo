#include <iostream>
#include <fstream>
#include <unistd.h>
#include <thread>
#include "JsonParser.hpp"
#include "UartHandler.hpp"
#include "ThreadSafeQueue.hpp"
#include "UartDevice.hpp"
#include "MqttHandler.hpp"

int main(){
    const std::string BROKER_IP("mqtt://broker.hivemq.com:1883");
    const std::string CLIENT_ID("Ivan_Esp32_RberryPi4");\
    MqttHandler mqtt_client(BROKER_IP, CLIENT_ID);

    try {
        std::cout << "Connecting to MQTT..." << std::endl;
        mqtt_client.connect_and_subscribe();
        std::cout << "MQTT Ready. Subscribed to studio/led/status" << std::endl;
    } catch (const mqtt::exception& exc) {
        std::cerr << "MQTT Connection Error: " << exc.what() << std::endl;
        return 1;
    }

    std::string path_to_json("/etc/my-uart-app/configuration.json");

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
            std::string msg = threadsafe_queue.pop();
            std::cout << "[UART LOG]: " << msg << std::endl;
        }
    });

    int choice;
    int r, g, b;
    
    while(true) {
        std::cout << "\nEnter RGB values (0-255) separated by space." << std::endl;
        std::cout << "Enter -1 to exit: ";
        
        if (!(std::cin >> r) || r == -1) break;
        std::cin >> g >> b;
     
        std::cout << "Select interface:\n" << "1. UART\n" << "2. MQTT\n";
        std::cin >> choice;

        if (r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255) {
            if(choice == 2){
                std::cout << "Sending Color: R=" << r << " G=" << g << " B=" << b << "..." << std::endl;
                mqtt_client.send_led_command(r, g, b);
            }
            else if(choice == 1){
                std::cout << "Sending Color: R=" << r << " G=" << g << " B=" << b << "..." << std::endl;
                std::string uart_cmd("LED:" + std::to_string(r) + "," + std::to_string(g) + "," + std::to_string(b));
                my_serial.sendBytes(uart_cmd);
            }
            else{
                std::cout << "Invalid input! Please user range 1 - 2\n";
            }
        } else {
            std::cout << "Invalid input! Please use range 0-255." << std::endl;
        }
    }

    receiver_message.join();
    processor_message.join();

    return 0;
}