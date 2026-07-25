#pragma once

#include <EspCommander.h>
#include "Properties.h"
#include "Vars.h"
#include "Led.h"

EspCommander::Value bme280QueryResults[] = {
    EspCommander::Value(EspCommander::Value::Params{
        .name = "temperature_C",
        .type = EspCommander::ValueType::FLOAT,
        .required = true,
    }),
    EspCommander::Value(EspCommander::Value::Params{
        .name = "humidity_%",
        .type = EspCommander::ValueType::FLOAT,
        .required = true,
    }),
    EspCommander::Value(EspCommander::Value::Params{
        .name = "pressure_HPA",
        .type = EspCommander::ValueType::FLOAT,
        .required = true,
    }),
    EspCommander::Value(EspCommander::Value::Params{
        .name = "QNH",
        .type = EspCommander::ValueType::FLOAT,
        .required = true,
    }),
};
auto bme280Query = EspCommander::Query(EspCommander::Query::Params{
    .name = "bme280",
    .results = bme280QueryResults,
    .handler = [](EspCommander::HandlerValue results[], etl::optional<const char *> &error)
    {
        float temperature = bme.readTemperature();
        float humidity = bme.readHumidity();
        float pressure = bme.readPressure() / 100.0F;
        float qnh = bme.seaLevelForAltitude(ALTITUDE, pressure);
        results[0] = temperature;
        results[1] = humidity;
        results[2] = pressure;
        results[3] = qnh;
    },
});

EspCommander::Value rgbLedQueryResults[] = {
    EspCommander::Value(EspCommander::Value::Params{
        .name = "Power On",
        .type = EspCommander::ValueType::BOOL,
        .required = true,
    }),
    EspCommander::Value(EspCommander::Value::Params{
        .name = "color",
        .type = EspCommander::ValueType::COLOR,
        .required = true,
    }),
    EspCommander::Value(EspCommander::Value::Params{
        .name = "brightness",
        .type = EspCommander::ValueType::RANGE,
        .required = true,
        .min = 0,
        .max = 255,
    }),
};
auto rgbLedQuery = EspCommander::Query(EspCommander::Query::Params{
    .name = "RGB Led",
    .results = rgbLedQueryResults,
    .handler = [](EspCommander::HandlerValue results[], etl::optional<const char *> &error)
    {
        results[0] = (bool)stripPowerOn;
        results[1] = (const char *)stripCurrentColor;
        results[2] = (int)stripCurrentBrightness;
    },
});

EspCommander::Value powerRgbLedActionParameters[] = {
    EspCommander::Value(EspCommander::Value::Params{
        .name = "power",
        .type = EspCommander::ValueType::BOOL,
        .required = true,
    }),
};
auto powerRgbLedAction = EspCommander::Action(EspCommander::Action::Params{
    .name = "power RGB LED",
    .parameters = powerRgbLedActionParameters,
    .handler = [](EspCommander::HandlerValue parameters[], EspCommander::HandlerValue results[], etl::optional<const char *> &error)
    {
        bool powerParam = etl::get<bool>(parameters[0].value());
        LedConfig ledConfig = {};
        ledConfig.powerOn = powerParam;
        ledConfig.brightness = stripCurrentBrightness;
        strlcpy(ledConfig.color, stripCurrentColor, sizeof(ledConfig.color));
        stripPowerOn = powerParam;
        saveConfig(ledConfig);
        setLed(powerParam, stripCurrentColor, (uint8_t)stripCurrentBrightness);
    },
});

EspCommander::Value setRgbLedActionParameters[] = {
    EspCommander::Value(EspCommander::Value::Params{
        .name = "color",
        .type = EspCommander::ValueType::COLOR,
        .required = true,
    }),
    EspCommander::Value(EspCommander::Value::Params{
        .name = "brightness",
        .type = EspCommander::ValueType::RANGE,
        .required = true,
        .min = 0,
        .max = 255,
    }),
};
auto setRgbLedAction = EspCommander::Action(EspCommander::Action::Params{
    .name = "set RGB LED",
    .parameters = setRgbLedActionParameters,
    .handler = [](EspCommander::HandlerValue parameters[], EspCommander::HandlerValue results[], etl::optional<const char *> &error)
    {
        const char *colorParam = etl::get<const char *>(parameters[0].value());
        int brightnessParam = etl::get<int>(parameters[1].value());

        strlcpy(stripCurrentColor, colorParam, sizeof(stripCurrentColor));
        stripCurrentBrightness = brightnessParam;
        stripPowerOn = true;

        Serial.println(stripCurrentColor);
        Serial.println(stripCurrentBrightness);

        LedConfig ledConfig = {};
        ledConfig.powerOn = true;
        ledConfig.brightness = stripCurrentBrightness;
        strlcpy(ledConfig.color, stripCurrentColor, sizeof(ledConfig.color));
        saveConfig(ledConfig);
        setLed(true, colorParam, (uint8_t)stripCurrentBrightness);
    },
});

auto wakePcAction = EspCommander::Action(EspCommander::Action::Params{
    .name = "wake PC",
    .handler = [](EspCommander::HandlerValue parameters[], EspCommander::HandlerValue results[], etl::optional<const char *> &error)
    {
        EspNowMqttGateway::Peer::wolMessage(WOL_PORT, pcMac);
    },
});

static uint8_t requestBuffer[ESP_COMMANDER_JSON_BUFFER_SIZE];
static uint8_t responseBuffer[ESP_COMMANDER_JSON_BUFFER_SIZE];
EspCommander::StaticBufferAllocator requestAllocator(requestBuffer, sizeof(requestBuffer));
EspCommander::StaticBufferAllocator responseAllocator(responseBuffer, sizeof(responseBuffer));

EspCommander::Query queries[] = {
    bme280Query,
    rgbLedQuery,
};
EspCommander::Action actions[] = {
    powerRgbLedAction,
    setRgbLedAction,
    wakePcAction,
};
auto device = EspCommander::Device(EspCommander::Device::Params{
    .id = device_id,
    .name = device_name,
    .requestTopic = mqtt_request_topic,
    .responseTopic = mqtt_response_topic,
    .queries = queries,
    .actions = actions,
    .requestAllocator = requestAllocator,
    .responseAllocator = responseAllocator,
});