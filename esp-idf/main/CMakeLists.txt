idf_component_register(
  SRCS "main.cpp"
  INCLUDE_DIRS "../include"
  REQUIRES
    adafruit-bme280
    adafruit-neopixel
    adafruit-sh110x
    etl
    esp-commander
    esp-now-mqtt-gateway
)
