cmake_minimum_required(VERSION 3.10)
project(app)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE JSON_LIB_SOURCES "/home/ivan/JsonReader/jsoncpp/src/lib_json/*.cpp")

if(NOT JSON_LIB_SOURCES)
    message(FATAL_ERROR "Файли JsonCpp не знайдено за шляхом: /home/ivan/JsonReader/jsoncpp/src/lib_json/")
endif()

add_library(cool_library STATIC ${JSON_LIB_SOURCES})
target_include_directories(cool_library PUBLIC "/home/ivan/JsonReader/jsoncpp/include")


file(GLOB SERIAL_LIB_SOURCES "/home/ivan/SerialLibrary/serial/src/*.cc")
set(SERIAL_UNIX_SOURCES 
    "/home/ivan/SerialLibrary/serial/src/impl/list_ports/list_ports_linux.cc"
    "/home/ivan/SerialLibrary/serial/src/impl/unix.cc"
)

set(ALL_SERIAL_SOURCES ${SERIAL_LIB_SOURCES} ${SERIAL_UNIX_SOURCES})

if(NOT ALL_SERIAL_SOURCES)
    message(FATAL_ERROR "Файли Serial не знайдено за шляхом: /home/ivan/SerialLibrary/serial/src/")
endif()

add_library(serial_library STATIC ${ALL_SERIAL_SOURCES})
target_include_directories(serial_library PUBLIC "/home/ivan/SerialLibrary/serial/include")

add_executable(app main.cpp common/src/JsonParser.cpp common/src/UartHandler.cpp) 
target_include_directories(app PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common/include")
target_include_directories(app PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/common/")

find_package(Threads REQUIRED)

target_link_libraries(app PRIVATE 
    cool_library 
    serial_library 
    Threads::Threads 
    rt
)