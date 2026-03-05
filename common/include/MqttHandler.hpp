#pragma once
#include <mqtt/async_client.h>
#include <string>

class MqttHandler : public virtual mqtt::callback {
    mqtt::async_client client_;
    const std::string TOPIC_STATUS = "studio/led/status";
    const std::string TOPIC_CMD = "studio/led/cmd";

public:
    MqttHandler(const std::string& server_address, const std::string& client_id);
    void connect_and_subscribe();
    void send_led_command(int r, int g, int b);

    void message_arrived(mqtt::const_message_ptr msg) override;
    
    void connected(const std::string& cause) override { std::cout << "Connected!" << std::endl; }
    void connection_lost(const std::string& cause) override { std::cout << "Connection lost: " << cause << std::endl; }
};