#pragma once

#include <stdint.h>
#include "Config.h"

namespace EspNowMqttGateway
{
  enum MessageType
  {
    TEXT_MESSAGE = 1,
    NOTIFICATION_MESSAGE = 2,
    TIME_SYNC_MESSAGE = 3,
    SLEEPY_COMMAND_MESSAGE = 4,
    SLEEPY_DATA_MESSAGE = 5,
  };

  struct __attribute__((packed)) MqttEspNowMessage
  {
    char topic[ESP_NOW_MQTT_GATEWAY_TOPIC_SIZE];
    char text[ESP_NOW_MQTT_GATEWAY_MQTT_MESSAGE_TEXT_PAYLOAD_SIZE];
  };

  struct __attribute__((packed)) NotificationEspNowMessage
  {
    char title[ESP_NOW_MQTT_GATEWAY_NOTIFICATION_TITLE_SIZE];
    char body[ESP_NOW_MQTT_GATEWAY_NOTIFICATION_BODY_SIZE];
  };

  struct __attribute__((packed)) TimeSyncEspNowMessage
  {
    uint32_t epoch;
  };

  struct __attribute__((packed)) SleepyCommandEspNowMessage
  {
    char text[ESP_NOW_MQTT_GATEWAY_MQTT_MESSAGE_TEXT_PAYLOAD_SIZE];
  };

  struct __attribute__((packed)) SleepyDataEspNowMessage
  {
    char text[ESP_NOW_MQTT_GATEWAY_MQTT_MESSAGE_TEXT_PAYLOAD_SIZE];
  };

  struct __attribute__((packed)) EspNowMessage
  {
    MessageType type;
    union
    {
      MqttEspNowMessage mqttEspNowMessage;
      NotificationEspNowMessage notificationEspNowMessage;
      TimeSyncEspNowMessage timeSyncEspNowMessage;
      SleepyCommandEspNowMessage sleepyCommandEspNowMessage;
      SleepyDataEspNowMessage sleepyDataEspNowMessage;
    } payload;
  };
}