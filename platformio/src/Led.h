#pragma once

#include <stdint.h>
#include "Properties.h"
#include "Vars.h"

struct LedConfig
{
  bool powerOn;
  int brightness;
  char color[8];
};

void saveConfig(LedConfig conf)
{
  preferences.putBytes("led", &conf, sizeof(LedConfig));
}

LedConfig loadConfig()
{
  LedConfig conf = {
      .powerOn = false,
      .brightness = 128,
      .color = "#ffffff"};
  preferences.getBytes("led", &conf, sizeof(LedConfig));
  return conf;
}

void setLed(bool powerOn, const char *hexColor, const uint8_t brightness)
{
  if (!powerOn)
  {
    strip.setBrightness(0);
    strip.show();
  }

  if (hexColor[0] == '#')
  {
    hexColor++;
  }

  // Convert hex string to 24-bit number
  uint32_t color = strtol(hexColor, NULL, 16);

  // Extract RGB components
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;

  // Set the color for all 18 LEDs
  for (int i = 0; i < LED_COUNT; i++)
  {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }

  strip.setBrightness(brightness);
  strip.show();
}