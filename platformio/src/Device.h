#pragma once

#include <IotCommander.h>
#include <ESP32Ping.h>
#include "Properties.h"
#include "Vars.h"
#include "Led.h"
#include "Utils.h"

IotCommander::Value dht22QueryResults[] = {
    IotCommander::Value(IotCommander::Value::Params{
        .name = "temperature_C",
        .type = IotCommander::ValueType::FLOAT,
        .required = true,
    }),
    IotCommander::Value(IotCommander::Value::Params{
        .name = "humidity_%",
        .type = IotCommander::ValueType::FLOAT,
        .required = true,
    }),
};
auto dht22Query = IotCommander::Query(IotCommander::Query::Params{
    .name = "dht22",
    .results = dht22QueryResults,
    .handler = [](IotCommander::HandlerValue results[], etl::optional<const char *> &error)
    {
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();
        results[0] = temperature;
        results[1] = humidity;
    },
});

IotCommander::Value bmp280QueryResults[] = {
    IotCommander::Value(IotCommander::Value::Params{
        .name = "temperature_C",
        .type = IotCommander::ValueType::FLOAT,
        .required = true,
    }),
    IotCommander::Value(IotCommander::Value::Params{
        .name = "pressure_HPA",
        .type = IotCommander::ValueType::FLOAT,
        .required = true,
    }),
};
auto bmp280Query = IotCommander::Query(IotCommander::Query::Params{
    .name = "bmp280",
    .results = bmp280QueryResults,
    .handler = [](IotCommander::HandlerValue results[], etl::optional<const char *> &error)
    {
        float temperature = bmp.readTemperature();
        float pressure = bmp.readPressure() / 100.0F;
        results[0] = temperature;
        results[1] = pressure;
    },
});

IotCommander::Value rgbLedQueryResults[] = {
    IotCommander::Value(IotCommander::Value::Params{
        .name = "Power On",
        .type = IotCommander::ValueType::BOOL,
        .required = true,
    }),
    IotCommander::Value(IotCommander::Value::Params{
        .name = "color",
        .type = IotCommander::ValueType::COLOR,
        .required = true,
    }),
    IotCommander::Value(IotCommander::Value::Params{
        .name = "brightness",
        .type = IotCommander::ValueType::RANGE,
        .required = true,
        .min = 0,
        .max = 255,
    }),
};
auto rgbLedQuery = IotCommander::Query(IotCommander::Query::Params{
    .name = "RGB Led",
    .results = rgbLedQueryResults,
    .handler = [](IotCommander::HandlerValue results[], etl::optional<const char *> &error)
    {
        results[0] = (bool)stripPowerOn;
        results[1] = (const char *)stripCurrentColor;
        results[2] = (int)stripCurrentBrightness;
    },
});

IotCommander::Value pingPCActionResults[] = {
    IotCommander::Value(IotCommander::Value::Params{
        .name = "alive",
        .type = IotCommander::ValueType::BOOL,
        .required = true,
    }),
};
auto pingPcAction = IotCommander::Action(IotCommander::Action::Params{
    .name = "pingPC",
    .results = pingPCActionResults,
    .handler = [](IotCommander::HandlerValue parameters[], IotCommander::HandlerValue results[], etl::optional<const char *> &error)
    {
        IPAddress targetIP;
        if (!targetIP.fromString(pc_ip))
        {
            error = "INVALID_IP";
            return;
        }
        bool alive = Ping.ping(targetIP, 3);
        results[0] = alive;
    },
});

auto wakePcAction = IotCommander::Action(IotCommander::Action::Params{
    .name = "wakePC",
    .handler = [](IotCommander::HandlerValue parameters[], IotCommander::HandlerValue results[], etl::optional<const char *> &error)
    {
        uint8_t mac[6];
        if (!parseMac(pc_mac, mac))
        {
            error = "INVALID_MAC";
            return;
        }
        bool ok = sendWOL(mac);
        if (!ok)
        {
            error = "WOL_FAILED";
        }
    },
});

IotCommander::Value powerRgbLedActionParameters[] = {
    IotCommander::Value(IotCommander::Value::Params{
        .name = "power",
        .type = IotCommander::ValueType::BOOL,
        .required = true,
    }),
};
auto powerRgbLedAction = IotCommander::Action(IotCommander::Action::Params{
    .name = "power RGB LED",
    .parameters = powerRgbLedActionParameters,
    .handler = [](IotCommander::HandlerValue parameters[], IotCommander::HandlerValue results[], etl::optional<const char *> &error)
    {
        bool powerParam = etl::get<bool>(parameters[0].value());
        LedConfig ledConfig = {};
        ledConfig.powerOn = powerParam;
        ledConfig.brightness = stripCurrentBrightness;
        strncpy(ledConfig.color, stripCurrentColor, sizeof(ledConfig.color) - 1);
        ledConfig.color[sizeof(ledConfig.color) - 1] = '\0';
        stripPowerOn = powerParam;
        saveConfig(ledConfig);
        setLed(powerParam, stripCurrentColor, (uint8_t)stripCurrentBrightness);
    },
});

IotCommander::Value setRgbLedActionParameters[] = {
    IotCommander::Value(IotCommander::Value::Params{
        .name = "color",
        .type = IotCommander::ValueType::COLOR,
        .required = true,
    }),
    IotCommander::Value(IotCommander::Value::Params{
        .name = "brightness",
        .type = IotCommander::ValueType::RANGE,
        .required = true,
        .min = 0,
        .max = 255,
    }),
};
auto setRgbLedAction = IotCommander::Action(IotCommander::Action::Params{
    .name = "set RGB LED",
    .parameters = setRgbLedActionParameters,
    .handler = [](IotCommander::HandlerValue parameters[], IotCommander::HandlerValue results[], etl::optional<const char *> &error)
    {
        const char *colorParam = etl::get<const char *>(parameters[0].value());
        int brightnessParam = etl::get<int>(parameters[1].value());

        strncpy(stripCurrentColor, colorParam, sizeof(stripCurrentColor) - 1);
        stripCurrentColor[sizeof(stripCurrentColor) - 1] = '\0';
        stripCurrentBrightness = brightnessParam;
        stripPowerOn = true;

        Serial.println(stripCurrentColor);
        Serial.println(stripCurrentBrightness);

        LedConfig ledConfig = {};
        ledConfig.powerOn = true;
        ledConfig.brightness = stripCurrentBrightness;
        strncpy(ledConfig.color, stripCurrentColor, sizeof(ledConfig.color) - 1);
        ledConfig.color[sizeof(ledConfig.color) - 1] = '\0';
        saveConfig(ledConfig);
        setLed(true, colorParam, (uint8_t)stripCurrentBrightness);
    },
});

static uint8_t requestBuffer[IOTC_JSON_BUFFER_SIZE];
static uint8_t responseBuffer[IOTC_JSON_BUFFER_SIZE];
IotCommander::StaticBufferAllocator requestAllocator(requestBuffer, sizeof(requestBuffer));
IotCommander::StaticBufferAllocator responseAllocator(responseBuffer, sizeof(responseBuffer));

IotCommander::Query queries[] = {
    dht22Query,
    bmp280Query,
    rgbLedQuery,
};
IotCommander::Action actions[] = {
    pingPcAction,
    wakePcAction,
    powerRgbLedAction,
    setRgbLedAction,
};
auto device = IotCommander::Device(IotCommander::Device::Params{
    .id = device_id,
    .name = device_name,
    .requestTopic = mqtt_request_topic,
    .responseTopic = mqtt_response_topic,
    .queries = queries,
    .actions = actions,
    .requestAllocator = requestAllocator,
    .responseAllocator = responseAllocator,
});