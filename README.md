IoT Device for Sending the Meteorological Data to IFTTT Maker Webhooks Service using ESP-WROOM-32
========

## Description

This repository includes Arduino sketch of IoT Device for sending temperature, humidity and atmospheric pressure to IFTTT maker webhooks service from ESP-WROOM-32.
The meteorogical data are measured by the Bosch Sensortec BME280 interfaced with the I2C bus.
The sending processing to IFTTT are performed every 10 minites based on local time synchronized with the NTP server.
Untill next sending precessing, ESP-WROOM-32 will go to deep sleep mode for power saving.


## Requirement

### Hardware

* [Wi-Fi Arduino IoT Express by CQ Publishing Co., Ltd.](http://toragi.cqpub.co.jp/tabid/848/Default.aspx)

* GY-BME280


### Software

* [Arduino 1.8.5](https://www.arduino.cc/en/Main/Software)

* [Arduino core for ESP32 WiFi chip](https://github.com/espressif/arduino-esp32)

* [ESP32 BLE for Arduino](https://github.com/nkolban/ESP32_BLE_Arduino)

* [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)

* [Adafruit BME280 Library](https://github.com/adafruit/Adafruit_BME280_Library)


## License

* MIT
