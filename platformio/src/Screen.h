#pragma once

#include "Frames.h"
#include "Vars.h"

struct Animation
{
  const unsigned char *const *frames; // pointer to frame array
  uint16_t frameCount;                // number of frames
};

Animation animations[] = {
    {
        .frames = star_bitmap_allArray,
        .frameCount = star_bitmap_allArray_LEN,
    },
    {
        .frames = nyancat_bitmap_allArray,
        .frameCount = nyancat_bitmap_allArray_LEN,
    },
    {
        .frames = bango_bitmap_allArray,
        .frameCount = bango_bitmap_allArray_LEN,
    },
    {
        .frames = jake_bitmap_allArray,
        .frameCount = jake_bitmap_allArray_LEN,
    },
};
const uint8_t animationCount = sizeof(animations) / sizeof(animations[0]);

void setup_oled()
{
  Serial.println("OLED init");
  if (!display.begin(0x3C, true))
  {
    Serial.println("SH1106 not found");
    while (1)
      ;
  }
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setContrast(50);
  display.setTextSize(1);
  const char *message = "Initializing...";
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(message, 0, 0, &x1, &y1, &w, &h);
  int xPos = (SCREEN_WIDTH - w) / 2;
  int yPos = (SCREEN_HEIGHT - h) / 2;
  display.setCursor(xPos, yPos);
  display.print(message);
  display.display();
  Serial.println("OLED done");
}

void hande_animation()
{
  unsigned long now = millis();
  Animation &anim = animations[currentAnimationIndex];
  if (now - animationStartTime >= animationDuration)
  {
    currentAnimationIndex++;
    if (currentAnimationIndex >= animationCount)
      currentAnimationIndex = 0;
    currentFrame = 0;
    animationStartTime = now;
  }
  if (now - lastFrameTime >= frameInterval)
  {
    lastFrameTime = now;
    display.clearDisplay();
    display.drawBitmap(
        0, 0,
        anim.frames[currentFrame],
        128, 64,
        SH110X_WHITE);

    display.display();
    currentFrame++;
    if (currentFrame >= anim.frameCount)
      currentFrame = 0;
  }
}

void show_info_screen()
{
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Desk Sensor");

  display.drawLine(0, 10, 128, 10, SH110X_WHITE);

  display.setCursor(0, 16);
  display.print("Temp(DHT): ");
  display.print(storedTempDHT, 1);
  display.println("C");

  display.setCursor(0, 28);
  display.print("Humidity: ");
  display.print(storedHumidity, 0);
  display.println("%");

  display.setCursor(0, 40);
  display.print("Temp(BMP): ");
  display.print(storedTempBMP, 1);
  display.println("C");

  display.setCursor(0, 52);
  display.print("Pressure: ");
  display.print(storedPressure, 0);
  display.println("hPa");

  display.display();
}