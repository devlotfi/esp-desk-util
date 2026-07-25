idf_component_register(
  SRCS "Adafruit_BME280.cpp"
  INCLUDE_DIRS "."
  REQUIRES 
    espressif__arduino-esp32 
    adafruit-sensor
    adafruit-busio
)
