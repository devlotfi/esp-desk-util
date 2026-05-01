
#include <Arduino.h>
#include <EspCommander.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "Properties.h"
#include "Vars.h"
#include "Device.h"
#include "Led.h"
#include "Screen.h"

static char output[ESP_NOW_MQTT_GATEWAY_MQTT_MESSAGE_TEXT_PAYLOAD_SIZE];
void onReceive(const char *topic, const char *text)
{
  if (strcmp(mqtt_discovery_request_topic, topic) == 0)
  {
    Serial.println("Discovery called");
    device.discovery(output, ESP_COMMANDER_JSON_BUFFER_SIZE);
    EspNowMqttGateway::Peer::mqttMessage(mqtt_discovery_response_topic, output);
  }
  else if (strcmp(mqtt_request_topic, topic) == 0)
  {
    Serial.println("Request called");
    device.request(text, output, ESP_COMMANDER_JSON_BUFFER_SIZE);
    Serial.println("Request result: ");
    Serial.println(output);
    EspNowMqttGateway::Peer::mqttMessage(mqtt_response_topic, output);
  }
}

void setup_sensors()
{
  Serial.println("Sensors init");
  if (!bme.begin(0x76))
  {
    if (!bme.begin(0x77))
    {
      Serial.println("BME280 not found! Check wiring/address.");
      while (1)
        ;
    }
  }
  Serial.println("Sensors done");
}

void setup()
{
  Serial.begin(115200);
  pinMode(TOUCH_PIN, INPUT);
  Wire.begin(BME_SDA, BME_SCL);
  I2C_OLED.begin(OLED_SDA, OLED_SCL, 400000);
  setup_strip();
  setup_oled();
  setup_sensors();

  EspNowMqttGateway::PeerConfig peerConfig{
      .pmk = pmk,
      .lmk = lmk,
      .gatewayMac = gatewayMac,
      .peerMac = peerMac,
      .channel = channel,
      .handleRecieve = onReceive,
  };
  EspNowMqttGateway::Peer::init(peerConfig);
}

void loop()
{
  // Detect touch
  bool currentTouch = digitalRead(TOUCH_PIN);
  if (currentTouch && !lastTouchState)
  {
    // Read sensors ONCE
    storedTemperature = bme.readTemperature();
    storedHumidity = bme.readHumidity();
    storedPressure = bme.readPressure() / 100.0F;
    storedQNH = bme.seaLevelForAltitude(ALTITUDE, storedPressure);
    showInfoScreen = true;
    infoStartTime = millis();
  }
  lastTouchState = currentTouch;

  // If info screen active
  if (showInfoScreen)
  {
    show_info_screen();

    if (millis() - infoStartTime >= infoDuration)
    {
      showInfoScreen = false;
    }
  }
  else
  {
    hande_animation();
  }
}