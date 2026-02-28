#pragma once

#include <Arduino.h>

// ----------------------
// Preferences namespace
// ----------------------
#define PREFERENCES_NAMESAPCE "LED"

// ----------------------
// Touch module
// ----------------------
#define TOUCH_PIN 21

// ----------------------
// DHT22
// ----------------------
#define DHTPIN 13
#define DHTTYPE DHT22

// ----------------------
// BMP280
// ----------------------
#define BMP_SDA 18
#define BMP_SCL 19

// ----------------------
// LED STRIP
// ----------------------
#define LED_PIN 22
#define LED_COUNT 18

// ----------------------
// OLED
// ----------------------
#define OLED_SDA 16
#define OLED_SCL 17
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// ----------------------
// Pc info
// ----------------------
const char *pc_mac = "pc_mac";
const char *pc_ip = "pc_ip";

// ----------------------
// Wi-Fi credentials
// ----------------------
const char *ssid = "ssid";
const char *password = "password";

// ----------------------
// MQTT credentials
// ----------------------
const char *mqtt_server = "mqtt_server";
const int mqtt_port = 8883;
const char *mqtt_user = "mqtt_user";
const char *mqtt_password = "mqtt_password";

// ----------------------
// MQTT Topics
// ----------------------
const char *mqtt_discovery_request_topic = "iot-commander/discovery/request";
const char *mqtt_discovery_response_topic = "iot-commander/discovery/response";
const char *mqtt_request_topic = "iot-commander/esp-desk-util/request";
const char *mqtt_response_topic = "iot-commander/esp-desk-util/response";

// ----------------------
// Device
// ----------------------
const char *device_id = "c677dae7-fafe-4ee6-892c-841779e6a0bf";
const char *device_name = "IOT_DESK_UTIL";

// ----------------------
// UDP for WOL
// ----------------------
const int WOL_PORT = 9;

// ----------------------
// TLS Root Certificate
// ----------------------
const char rootCA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
...
-----END CERTIFICATE-----
)EOF";