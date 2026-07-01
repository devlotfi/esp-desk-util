#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Properties.h"
#include "Vars.h"

static char metricJsonBuffer[4096];

void metricsTask(void *pvParameters)
{
  while (1)
  {
    Serial.println("Seding metrics...");
    float temperature = bme.readTemperature();
    float humidity = bme.readHumidity();
    float pressure = bme.readPressure() / 100.0F;
    float qnh = bme.seaLevelForAltitude(ALTITUDE, pressure);

    // Get current Unix time with microsecond resolution
    struct timeval tv;
    gettimeofday(&tv, nullptr);

    uint64_t timeUnixNano =
        (uint64_t)tv.tv_sec * 1000000000ULL +
        (uint64_t)tv.tv_usec * 1000ULL;
    ArduinoJson::JsonDocument doc;
    // resourceMetrics
    ArduinoJson::JsonArray resourceMetrics = doc["resourceMetrics"].to<ArduinoJson::JsonArray>();
    ArduinoJson::JsonObject resourceMetric = resourceMetrics.add<ArduinoJson::JsonObject>();
    // scopeMetrics
    ArduinoJson::JsonArray scopeMetrics = resourceMetric["scopeMetrics"].to<ArduinoJson::JsonArray>();
    ArduinoJson::JsonObject scopeMetric = scopeMetrics.add<ArduinoJson::JsonObject>();
    // metrics
    ArduinoJson::JsonArray metrics = scopeMetric["metrics"].to<ArduinoJson::JsonArray>();

    // Temperature metric
    ArduinoJson::JsonObject tempMetric = metrics.add<ArduinoJson::JsonObject>();
    tempMetric["name"] = "desk_temperature";
    tempMetric["unit"] = "Celcius";
    ArduinoJson::JsonArray tempDataPoints =
        tempMetric["gauge"]["dataPoints"].to<ArduinoJson::JsonArray>();
    ArduinoJson::JsonObject tempPoint = tempDataPoints.add<ArduinoJson::JsonObject>();
    tempPoint["asDouble"] = temperature;
    tempPoint["timeUnixNano"] = timeUnixNano;

    // Humidity metric
    ArduinoJson::JsonObject humidityMetric = metrics.add<ArduinoJson::JsonObject>();
    humidityMetric["name"] = "desk_humidity";
    humidityMetric["unit"] = "Percent";
    ArduinoJson::JsonArray humidityDataPoints =
        humidityMetric["gauge"]["dataPoints"].to<ArduinoJson::JsonArray>();
    ArduinoJson::JsonObject humidityPoint = humidityDataPoints.add<ArduinoJson::JsonObject>();
    humidityPoint["asDouble"] = humidity;
    humidityPoint["timeUnixNano"] = timeUnixNano;

    // Pressure metric
    ArduinoJson::JsonObject pressureMetric = metrics.add<ArduinoJson::JsonObject>();
    pressureMetric["name"] = "desk_pressure";
    pressureMetric["unit"] = "hPa";
    ArduinoJson::JsonArray pressureDataPoints =
        pressureMetric["gauge"]["dataPoints"].to<ArduinoJson::JsonArray>();
    ArduinoJson::JsonObject pressurePoint = pressureDataPoints.add<ArduinoJson::JsonObject>();
    pressurePoint["asDouble"] = pressure;
    pressurePoint["timeUnixNano"] = timeUnixNano;

    serializeJson(doc, metricJsonBuffer, 4096);
    Serial.println(metricJsonBuffer);

    EspNowMqttGateway::Peer::metricMessage(metricJsonBuffer);

    vTaskDelay(pdMS_TO_TICKS(1 * 60 * 1000));
  }
}
