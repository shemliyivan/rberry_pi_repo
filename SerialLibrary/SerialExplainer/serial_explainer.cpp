#include <iostream>
#include <string>
#include <cstdio>
#include <unistd.h>
#include "serial.h"

void my_sleep(unsigned long milliseconds){
    // Блокує виконання потоку на певний час - застаріла версія для std::this_thread::sleep_for
    usleep(milliseconds * 1000); // 100 ms
}

void enumeratePorts(){
    // list_ports - знаходить усі підключені пристрої
    std::vector<serial::PortInfo> devices_found = serial::list_ports();
    auto iter = devices_found.begin();
    while(iter != devices_found.end()){
        // PortInfo - структура, яка містить інформацію про підключений пристрій
        serial::PortInfo device = *iter++;

        // Системний шлях, те що видно у Диспетчері пристроїв, технічний паспорт пристрою
        std::cout << "(" << device.port << ", " << device.description << ", " << device.hardware_id << ")\n"; 
    }
}

int run(int argc, char** argv){
    if(argc < 2){
        std::cout << "argc < 2\n";
        return 0;
    }

    std::string port(argv[1]);

    if(port == "-e"){
        enumeratePorts();
        return 0;
    }
    else if(argc < 3){
        std::cout << "argc < 3\n";
        return 1;
    }

    unsigned long baud;
    sscanf(argv[2], "%lu", &baud);

    // Клас, який взаємодії з портом 
    // Параметри конструктора: порт, baud_rate - для мого пк, скільки мс чекати, якщо після read() - нічого не прийшло
    serial::Serial my_serial(port, baud, serial::Timeout::simpleTimeout(1000));

    std::cout << "Is the serial port open?\n";

    // Чи відбулось з'єднання з портом
    if(my_serial.isOpen()){
        std::cout << "Yes.\n";
    }
    else{
        std::cout << "No.\n";
    }

    

    int count = 0;
    std::string test_string;
    if(argc == 4){
        test_string = argv[3];
    }
    else{
        test_string = "Testing.";
    }


    // ??
    my_serial.setTimeout(serial::Timeout::max(), 250, 0, 250, 0);
    std::cout << "Timeout == 1000ms, asking for 1 more byte than written.\n";
    while(count < 10){

        // Надішляє байти, повертає скільки байтів було записано
        size_t bytes_wrote = my_serial.write(test_string);

        // Читає байти з буфера rx, тут на 1 байт більше - щоб програма почекала 250мс, але поверне length() байтів - тобто стільки - скільки прочитано 
        std::string result = my_serial.read(test_string.length() + 1);

        std::cout << "Iteration: " << count << ", Bytes written: ";
        std::cout << bytes_wrote << ", Bytes read: ";
        std::cout << result.length() << ", String read: " << result << std::endl;

        ++count;
    }

    return 0;
}

int main(int argc, char** argv){

    run(argc, argv);

    return 0;
}