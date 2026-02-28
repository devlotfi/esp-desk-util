#pragma once

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <DHT.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <Preferences.h>
#include "Properties.h"

TwoWire I2C_OLED = TwoWire(1);
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2C_OLED, -1);
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Preferences preferences;

WiFiUDP udp;
WiFiClientSecure espClient;
PubSubClient client(espClient);

bool stripPowerOn = false;
char stripCurrentColor[8] = "#ffffff";
int stripCurrentBrightness = 128;

bool showInfoScreen = false;
bool lastTouchState = false;
unsigned long infoStartTime = 0;
const unsigned long infoDuration = 10000;
float storedTempDHT = 0;
float storedHumidity = 0;
float storedTempBMP = 0;
float storedPressure = 0;

uint8_t currentAnimationIndex = 0;
uint16_t currentFrame = 0;
unsigned long lastFrameTime = 0;
unsigned long animationStartTime = 0;
const unsigned long frameInterval = 100;
const unsigned long animationDuration = 60000;