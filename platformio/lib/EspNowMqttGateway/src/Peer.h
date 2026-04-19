#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_now.h>
#include "Utils.h"
#include "Types.h"

namespace EspNowMqttGateway
{
  struct PeerConfig
  {
    const char *pmk;
    const char *lmk;
    const char *gatewayMac;
    const char *peerMac;
    uint8_t channel;
  };

  class Peer
  {
  public:
    uint8_t pmk[ESP_NOW_KEY_SIZE_BYTES];
    uint8_t lmk[ESP_NOW_KEY_SIZE_BYTES];
    uint8_t gatewayMac[MAC_SIZE_BYTES];
    uint8_t peerMac[MAC_SIZE_BYTES];
    uint8_t channel = 1;

    void init(PeerConfig &peerConfig)
    {
      Serial.println(peerConfig.pmk);
      Serial.println(peerConfig.lmk);
      Serial.println(peerConfig.gatewayMac);
      Serial.println(peerConfig.peerMac);
      Serial.println(peerConfig.channel);
      keyHexToBytes(peerConfig.pmk, pmk);
      keyHexToBytes(peerConfig.lmk, lmk);
      macStringToBytes(peerConfig.gatewayMac, gatewayMac);
      macStringToBytes(peerConfig.peerMac, peerMac);
      channel = peerConfig.channel;

      WiFi.mode(WIFI_STA);
      esp_wifi_set_mac(WIFI_IF_STA, peerMac);
      esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
      esp_wifi_set_ps(WIFI_PS_NONE);

      if (esp_now_init() != ESP_OK)
      {
        Serial.println("ESP-NOW init failed");
        return;
      }

      esp_now_set_pmk(pmk);

      esp_now_peer_info_t peerInfo = {};
      memcpy(peerInfo.peer_addr, gatewayMac, MAC_SIZE_BYTES);
      peerInfo.encrypt = true;
      peerInfo.channel = channel;
      memcpy(peerInfo.lmk, lmk, ESP_NOW_KEY_SIZE_BYTES);
      if (esp_now_add_peer(&peerInfo) != ESP_OK)
        Serial.println("Failed to add peer");
    }

    void mqttMessage(const char *topic, const char *text)
    {
      EspNowMessage msg = {};
      msg.type = MessageType::TEXT_MESSAGE;

      strncpy(msg.payload.mqttEspNowMessage.topic, topic, ESP_NOW_MQTT_GATEWAY_TOPIC_SIZE - 1);
      msg.payload.mqttEspNowMessage.topic[ESP_NOW_MQTT_GATEWAY_TOPIC_SIZE - 1] = '\0';

      strncpy(msg.payload.mqttEspNowMessage.text, text, ESP_NOW_MQTT_GATEWAY_MQTT_MESSAGE_TEXT_PAYLOAD_SIZE - 1);
      msg.payload.mqttEspNowMessage.text[ESP_NOW_MQTT_GATEWAY_MQTT_MESSAGE_TEXT_PAYLOAD_SIZE - 1] = '\0';

      esp_err_t result = esp_now_send(
          gatewayMac,
          (const uint8_t *)&msg,
          sizeof(EspNowMessage));

      if (result == ESP_OK)
        Serial.println("ESP-NOW: Packet accepted");
      else
        Serial.printf("ESP-NOW: Send failed, err=0x%x\n", result);
    }

    void notificationMessage(const char *title, const char *body)
    {
      EspNowMessage msg = {};
      msg.type = MessageType::NOTIFICATION_MESSAGE;

      strncpy(msg.payload.notificationEspNowMessage.title, title, ESP_NOW_MQTT_GATEWAY_NOTIFICATION_TITLE_SIZE - 1);
      msg.payload.notificationEspNowMessage.title[ESP_NOW_MQTT_GATEWAY_NOTIFICATION_TITLE_SIZE - 1] = '\0';

      strncpy(msg.payload.notificationEspNowMessage.body, body, ESP_NOW_MQTT_GATEWAY_NOTIFICATION_BODY_SIZE - 1);
      msg.payload.notificationEspNowMessage.body[ESP_NOW_MQTT_GATEWAY_NOTIFICATION_BODY_SIZE - 1] = '\0';

      esp_err_t result = esp_now_send(
          gatewayMac,
          (const uint8_t *)&msg,
          sizeof(EspNowMessage));

      if (result == ESP_OK)
        Serial.println("ESP-NOW: Packet accepted");
      else
        Serial.printf("ESP-NOW: Send failed, err=0x%x\n", result);
    }
  };
}
