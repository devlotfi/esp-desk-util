
#include <Arduino.h>
#include <IotCommander.h>
#include "Properties.h"
#include "Vars.h"
#include "Device.h"
#include "Animation.h"

static char output[IOTC_JSON_BUFFER_SIZE];
static char input[IOTC_JSON_BUFFER_SIZE];
void callback(char *topic, byte *payload, unsigned int length)
{
  if (length >= IOTC_JSON_BUFFER_SIZE)
  {
    Serial.println("Payload too large!");
    return;
  }
  memcpy(input, payload, length);
  input[length] = '\0';

  Serial.println(topic);
  Serial.println(input);
  if (strcmp(mqtt_discovery_request_topic, topic) == 0)
  {
    Serial.println("Discovery called");
    device.discovery(output, IOTC_JSON_BUFFER_SIZE);
    client.publish(mqtt_discovery_response_topic, output);
  }
  else if (strcmp(mqtt_request_topic, topic) == 0)
  {
    Serial.println("Request called");
    device.request(input, output, IOTC_JSON_BUFFER_SIZE);
    Serial.println("Request result: ");
    Serial.println(output);
    client.publish(mqtt_response_topic, output);
  }
}

void setup_wifi()
{
  Serial.println();
  Serial.printf("Connecting to WiFi: %s\n", ssid);

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  WiFi.begin(ssid, password);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000)
  {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("\nWiFi Failed");
    ESP.restart();
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void reconnectMQTT()
{
  while (!client.connected())
  {
    Serial.print("Connecting to MQTT...");
    if (client.connect(device_id, mqtt_user, mqtt_password))
    {
      Serial.println(" connected!");
      client.subscribe(mqtt_discovery_request_topic);
      client.subscribe(mqtt_request_topic);
    }
    else
    {
      Serial.print(" failed, rc=");
      Serial.println(client.state());
      delay(3000);
    }
  }
}

void setup_strip()
{
  preferences.begin(PREFERENCES_NAMESAPCE, false);
  auto ledConfig = loadConfig();
  strncpy(stripCurrentColor, ledConfig.color, sizeof(stripCurrentColor) - 1);
  stripCurrentColor[sizeof(stripCurrentColor) - 1] = '\0';
  stripCurrentBrightness = ledConfig.brightness;
  setLed(ledConfig.powerOn, ledConfig.color, ledConfig.brightness);
}

void setup_sensors()
{
  Serial.println("Sensors init");
  dht.begin();
  if (!bmp.begin(0x76))
  {
    if (!bmp.begin(0x77))
    {
      Serial.println("BMP280 not found! Check wiring/address.");
      while (1)
        ;
    }
  }
  Serial.println("Sensors done");
}

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

void setup()
{
  Serial.begin(115200);
  pinMode(TOUCH_PIN, INPUT);
  Wire.begin(BMP_SDA, BMP_SCL);
  I2C_OLED.begin(OLED_SDA, OLED_SCL, 400000);
  setup_strip();
  setup_oled();
  setup_sensors();
  setup_wifi();
  espClient.setCACert(rootCA);
  client.setBufferSize(IOTC_JSON_BUFFER_SIZE);
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  udp.begin(WOL_PORT);
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi disconnected! Reconnecting...");
    setup_wifi();
  }

  if (!client.connected())
    reconnectMQTT();

  client.loop();

  // Detect touch
  bool currentTouch = digitalRead(TOUCH_PIN);
  if (currentTouch && !lastTouchState)
  {
    // Read sensors ONCE
    storedTempDHT = dht.readTemperature();
    storedHumidity = dht.readHumidity();
    storedTempBMP = bmp.readTemperature();
    storedPressure = bmp.readPressure() / 100.0F;
    showInfoScreen = true;
    infoStartTime = millis();
  }
  lastTouchState = currentTouch;

  // If info screen active
  if (showInfoScreen)
  {
    show_info_screen();

    if (millis() - infoStartTime >= infoDuration)
    {
      showInfoScreen = false;
    }
  }
  else
  {
    hande_animation();
  }
}