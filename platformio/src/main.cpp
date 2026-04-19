
#include <Arduino.h>
#include <EspCommander.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "Properties.h"
#include "Vars.h"
#include "Device.h"
#include "Led.h"
#include "Screen.h"

void onSent(const esp_now_send_info_t *tx_info, esp_now_send_status_t status)
{
  Serial.print("ESP-NOW: Send status -> ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

static char output[ESP_COMMANDER_JSON_BUFFER_SIZE];
static char input[ESP_COMMANDER_JSON_BUFFER_SIZE];
void onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len)
{
  if (!data || len != sizeof(EspNowMqttGateway::EspNowMessage))
  {
    Serial.println("ESP-NOW: Invalid frame size");
    return;
  }

  const EspNowMqttGateway::EspNowMessage *msg = reinterpret_cast<const EspNowMqttGateway::EspNowMessage *>(data);

  switch (msg->type)
  {
  case EspNowMqttGateway::MessageType::TEXT_MESSAGE:
  {
    char safeTopic[ESP_NOW_MQTT_GATEWAY_TOPIC_SIZE];
    memcpy(safeTopic, msg->payload.mqttEspNowMessage.topic, ESP_NOW_MQTT_GATEWAY_TOPIC_SIZE - 1);
    safeTopic[ESP_NOW_MQTT_GATEWAY_TOPIC_SIZE - 1] = '\0';

    memcpy(input, msg->payload.mqttEspNowMessage.text, ESP_COMMANDER_JSON_BUFFER_SIZE);
    input[ESP_COMMANDER_JSON_BUFFER_SIZE - 1] = '\0';

    Serial.println("------------------------------------------");
    Serial.printf("Data Length: %d\n", len);
    Serial.printf("Topic: %s\n", safeTopic);
    Serial.printf("Text: %s\n", msg->payload.mqttEspNowMessage.text);
    Serial.println("------------------------------------------\n");

    if (strcmp(mqtt_discovery_request_topic, safeTopic) == 0)
    {
      Serial.println("Discovery called");
      device.discovery(output, ESP_COMMANDER_JSON_BUFFER_SIZE);
      peer.mqttMessage(mqtt_discovery_response_topic, output);
    }
    else if (strcmp(mqtt_request_topic, safeTopic) == 0)
    {
      Serial.println("Request called");
      device.request(input, output, ESP_COMMANDER_JSON_BUFFER_SIZE);
      Serial.println("Request result: ");
      Serial.println(output);
      peer.mqttMessage(mqtt_response_topic, output);
    }

    break;
  }

  default:
    Serial.printf("ESP-NOW: Unknown type %d\n", msg->type);
  }
}

void setup_sensors()
{
  Serial.println("Sensors init");
  dht.begin();
  if (!bmp.begin(0x76))
  {
    if (!bmp.begin(0x77))
    {
      Serial.println("BMP280 not found! Check wiring/address.");
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
  Wire.begin(BMP_SDA, BMP_SCL);
  I2C_OLED.begin(OLED_SDA, OLED_SCL, 400000);
  setup_strip();
  setup_oled();
  setup_sensors();
  // udp.begin(WOL_PORT);

  EspNowMqttGateway::PeerConfig peerConfig{
      .pmk = pmk,
      .lmk = lmk,
      .gatewayMac = gatewayMac,
      .peerMac = peerMac,
      .channel = channel,
  };
  peer.init(peerConfig);

  esp_now_register_recv_cb(onReceive);
  esp_now_register_send_cb(onSent);
}

void loop()
{
  // Detect touch
  bool currentTouch = digitalRead(TOUCH_PIN);
  if (currentTouch && !lastTouchState)
  {
    // Read sensors ONCE
    storedTempDHT = dht.readTemperature();
    storedHumidity = dht.readHumidity();
    storedTempBMP = bmp.readTemperature();
    storedPressure = bmp.readPressure() / 100.0F;
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