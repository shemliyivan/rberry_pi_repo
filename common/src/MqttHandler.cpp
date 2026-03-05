#include "MqttHandler.hpp"
#include <iostream>
#include "json/json.h"

MqttHandler::MqttHandler(const std::string& server_address, const std::string& client_id)
    : client_(server_address, client_id) {
    client_.set_callback(*this);
}

void MqttHandler::connect_and_subscribe() {
    auto connOpts = mqtt::connect_options_builder()
        .clean_session(true)
        .finalize();
    
    client_.connect(connOpts)->wait();
    client_.subscribe(TOPIC_STATUS, 1)->wait();
}

void MqttHandler::send_led_command(int r, int g, int b) {
    Json::Value root;
    root["power"] = true;
    Json::Value rgb(Json::arrayValue);
    rgb.append(r); rgb.append(g); rgb.append(b);
    root["color"] = rgb;

    std::string payload = Json::writeString(Json::StreamWriterBuilder(), root);
    client_.publish(mqtt::make_message(TOPIC_CMD, payload))->wait();
}

void MqttHandler::message_arrived(mqtt::const_message_ptr msg) {
    Json::Value root;
    Json::CharReaderBuilder builder;
    std::string errs;
    std::stringstream ss(msg->get_payload_str());

    if (Json::parseFromStream(builder, ss, &root, &errs)) {
        std::cout << "[FEEDBACK] Device: " << root["device_id"].asString() 
                  << " | State: " << root["state"].asString()
                  << " | Uptime: " << root["uptime"].asInt() << "s" << std::endl;
    }
}