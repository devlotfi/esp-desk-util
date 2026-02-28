<img src="https://raw.githubusercontent.com/devlotfi/esp-desk-util/master/github-assets/github-banner.png">

# 📜 esp-desk-util

The goal of this project is to create a smart desk utility

## Features

- Turn on PC remotely using WOL
- PC Status using ping
- DHT11 Temperature / Humidity
- BMP280 Temperature / Pressure
- Display multiple standby animations on OLED
- Display sensor information on OLED on touch
- Control ARGB LED strip color

# 📌 Contents

- [Tech stack](#tech-stack)
- [Building the media controller](#building-the-media-controller)
  - [Components](#components)
  - [Libraries](#libraries)
  - [Configuration](#configuration)
  - [Images](#images)

# Tech stack

<p float="left">
  <img height="50px" src="https://devlotfi.github.io/stack-icons/icons/cpp.svg">
  <img height="50px" src="https://devlotfi.github.io/stack-icons/icons/arduino.svg">
  <img height="50px" src="https://devlotfi.github.io/stack-icons/icons/espressif.svg">
</p>

## Diagrams

<p float="left">
  <img height="50px" src="https://devlotfi.github.io/stack-icons/icons/fritzing.svg">
</p>

# Building the Media Controller

## Components

- ESP32 (38 Pin)
- DHT22
- BMP280
- TTP223
- OLED Screen (1.3 Inch, SH1106 Driver)
- Cables
- Case (Optional)

<img src="https://raw.githubusercontent.com/devlotfi/esp-desk-util/master/github-assets/fritzing.png">

## Libraries

- ArduinoJson
- Embedded Template Library ETL
- DHT sensor library
- Adafruit BMP280 Library
- ESP32Ping
- Adafruit SH110X
- Adafruit NeoPixel
- PubSubClient
- [IotCommander](https://github.com/devlotfi/iot-commander)

## Configuration

Change settings in [Properties.h](https://github.com/devlotfi/esp-desk-util/blob/master/arduino/Properties.h)

The baord relied on an MQTT communication to change the RGB LED Color uisng [IotCommander](https://github.com/devlotfi/iot-commander)

## Images

Some images of the final build

<img src="https://raw.githubusercontent.com/devlotfi/esp-desk-util/master/github-assets/build-1.jpg">
<img src="https://raw.githubusercontent.com/devlotfi/esp-desk-util/master/github-assets/build-2.jpg">
