add_library(common_lib STATIC 
    src/JsonParser.cpp 
    src/UartHandler.cpp
    src/ThreadSafeQueue.cpp
)

target_include_directories(common_lib PUBLIC 
    include/
)

target_link_libraries(common_lib PUBLIC 
    jsoncpp_lib
    serial
)