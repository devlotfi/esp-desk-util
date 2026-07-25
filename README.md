<img src="https://raw.githubusercontent.com/devlotfi/esp-desk-util/master/github-assets/github-banner.png">

# 📜 esp-desk-util

The goal of this project is to create a smart desk utility

## Features

- DHT11 Temperature / Humidity
- BMP280 Temperature / Pressure
- Display multiple standby animations on OLED
- Display sensor information on OLED on touch
- Control ARGB LED strip color
- Turn on PC using Wake-On-Lan

# 📌 Contents

- [Tech stack](#tech-stack)
- [Build](#build)
  - [Components](#components)
  - [Libraries](#libraries)
  - [Configuration](#configuration)
  - [Images](#images)

# Tech stack

<p float="left">
  <img height="50px" src="https://devlotfi.github.io/stack-icons/icons/cpp.svg">
  <img height="50px" src="https://devlotfi.github.io/stack-icons/icons/arduino.svg">
  <img height="50px" src="https://devlotfi.github.io/stack-icons/icons/espressif.svg">
  <img height="50px" src="https://devlotfi.github.io/stack-icons/icons/mqtt.svg">
  <img height="50px" src="https://devlotfi.github.io/stack-icons/icons/esp-commander.svg">
  <img height="50px" src="https://devlotfi.github.io/stack-icons/icons/esp-now-mqtt-gateway.svg">
</p>

## Diagrams

<p float="left">
  <img height="50px" src="https://devlotfi.github.io/stack-icons/icons/fritzing.svg">
</p>

# Build

## Components

- ESP32 (38 Pin)
- BME280
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
- Adafruit SH110X
- Adafruit NeoPixel
- [EspCommander](https://github.com/devlotfi/esp-commander)
- [EspNowMqttGateway](https://github.com/devlotfi/esp-now-mqtt-gateway)

## Configuration

Change settings in [Properties.h](https://github.com/devlotfi/esp-desk-util/blob/main/platformio/src/Properties.h)

## Images

Some images of the final build

<img src="https://raw.githubusercontent.com/devlotfi/esp-desk-util/master/github-assets/build-1.jpg">
<img src="https://raw.githubusercontent.com/devlotfi/esp-desk-util/master/github-assets/build-2.jpg">
