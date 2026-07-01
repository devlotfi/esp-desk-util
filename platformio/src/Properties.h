#pragma once

#include <Arduino.h>

// ----------------------
// Preferences namespace
// ----------------------
#define PREFERENCES_NAMESAPCE "LED"

// ----------------------
// PC MAC
// ----------------------
#define WOL_PORT 9
const uint8_t pcMac[6] = {0x30, 0x9C, 0x23, 0x7E, 0x15, 0xA0};

// ----------------------
// Touch module
// ----------------------
#define TOUCH_PIN 21

// ----------------------
// BMP280
// ----------------------
#define ALTITUDE 190
#define BME_SDA 18
#define BME_SCL 19

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
// ESP-NOW MQTT Gateway config
// ----------------------
const char *gatewayMac = "30:AE:A4:11:22:33";
const char *peerMac = "E6:95:83:3F:5C:A3";
const char *pmk = "1187BA60CAA9A7FA52B2B8C4141B4A9D";
const char *lmk = "94232C172B047D53C2FFCFA4D88ECD86";
uint8_t channel = 1;

// ----------------------
// MQTT Topics
// ----------------------
const char *mqtt_discovery_request_topic = "esp-commander/discovery/request";
const char *mqtt_discovery_response_topic = "esp-commander/discovery/response";
const char *mqtt_request_topic = "esp-commander/esp-desk-util/request";
const char *mqtt_response_topic = "esp-commander/esp-desk-util/response";

// ----------------------
// Device
// ----------------------
const char *device_id = "c677dae7-fafe-4ee6-892c-841779e6a0bf";
const char *device_name = "DESK_UTIL";