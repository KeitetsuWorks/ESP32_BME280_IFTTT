/**
 * @file        ESP32_BME280_IFTTT.ino
 * @brief       Send Temperature, Humidity and Atmospheric Pressure to IFTTT Maker Webhooks Service from ESP32
 * @author      Keitetsu
 * @date        2018/03/02
 * @copyright   Copyright (c) 2018 Keitetsu
 * @par         License
 *              This software is released under the MIT License.
 */

#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "WiFi_config.h"


/**
 * @name    Pin Assignments
 */
/** @{ */
#define PIN_LED         2       /**< Green LED */
/** @} */

/**
 * @def     Time ESP32 will go to sleep (in micro seconds)
 */
#define TIME_TO_SLEEP   (9 * 60 * 1000 * 1000)


/**
 * @var     Bosch Sensortec BME280 (I2C)
 */
Adafruit_BME280 bme;

/**
 * @name    Wi-Fi Configuration
 */
/** @{ */
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
/** @} */


void setup()
{
    Serial.begin(115200);
    pinMode(PIN_LED, OUTPUT);
    delay(1000);
    
    Serial.println();
    Serial.println();
    Serial.println("Welcome to ESP32 IoT Express.");
    
    Serial.println();
    Serial.println();
    bool bmeStatus = bme.begin();
    if (!bmeStatus) {
        Serial.println("Failed to detect BME280 sensor.");
        
        Serial.println("Restarting ...");
        ESP.restart();
    }
    Serial.println("BME280 sensor was detected.");
    delay(1000);
    
    digitalWrite(PIN_LED, HIGH);
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.print(" ");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    int count = 0;
    while (WiFi.status() != WL_CONNECTED) {
        count++;
        if (count > 20) {
            break;
        }
        
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    digitalWrite(PIN_LED, LOW);
    
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Connection failed.");
        
        Serial.println("Restarting ...");
        ESP.restart();
    }
    else {
        Serial.print("Connected to ");
        Serial.print(ssid);
        Serial.println(".");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }
    
    configTzTime("JST-9", "ntp.nict.jp", "time.google.com", "ntp.jst.mfeed.ad.jp");
    
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP);
    
    return;
}


void loop()
{
    float bmeTemperature;
    float bmeHumidity;
    float bmePressure;
    bool bmeResult;
    struct tm timeInfo;
    
    while (1) {
        if (WiFi.status() != WL_CONNECTED) {
            digitalWrite(PIN_LED, HIGH);
            Serial.println();
            Serial.println();
            Serial.print("Reconnecting ");
            WiFi.reconnect();
            int count = 0;
            while (WiFi.status() != WL_CONNECTED) {
                count++;
                if (count > 20) {
                    break;
                }
                
                delay(500);
                Serial.print(".");
            }
            Serial.println();
            digitalWrite(PIN_LED, LOW);
            
            if (WiFi.status() != WL_CONNECTED) {
                Serial.println("Reconnection failed.");
                
                Serial.println("Restarting ...");
                ESP.restart();
            }
            else {
                Serial.println("Reconnected.");
                Serial.println("IP address: ");
                Serial.println(WiFi.localIP());
            }
        }
        
        bmeResult = readBME280Values(&bmeTemperature, &bmeHumidity, &bmePressure);
        if (!bmeResult) {
            Serial.println("Failed to read values from BME280 sensor.");
            
            Serial.println("Restarting ...");
            ESP.restart();
        }
        
        getLocalTime(&timeInfo);
        
        Serial.println();
        Serial.println();
        Serial.println(&timeInfo, "%Y/%m/%d (%a) %H:%M:%S");
        printBME280Values(bmeTemperature, bmeHumidity, bmePressure);
        
        if ((timeInfo.tm_min % 10) == 0) {
            sendRequestToIFTTT(
                String(bmeTemperature),
                String(bmeHumidity),
                String(bmePressure)
            );
            
            break;
        }
        
        delay(10000);
    }
    
    Serial.println();
    Serial.println();
    Serial.println("Going to sleep now ...");
    esp_deep_sleep_start();
    Serial.println("Zzz ...");
    
    return;
}


bool readBME280Values(float *bmeTemperature, float *bmeHumidity, float *bmePressure)
{
    float bmeTemperatureTemp;
    float bmeHumidityTemp;
    float bmePressureTemp;
    bool result;
    
    result = false;
    
    int count = 0;
    while (1) {
        bmeTemperatureTemp = bme.readTemperature();
        bmeHumidityTemp = bme.readHumidity();
        bmePressureTemp = bme.readPressure() / 100.0F;
        
        if ((bmeTemperatureTemp > -100.0F) && (bmePressureTemp < 1100.0F)) {
            *bmeTemperature = bmeTemperatureTemp;
            *bmeHumidity = bmeHumidityTemp;
            *bmePressure = bmePressureTemp;
            result = true;
            
            break;
        }
        
        count++;
        if (count > 5) {
            break;
        }
        
        delay(1000);
    }
    
    return result;
}


void printBME280Values(float bmeTemperature, float bmeHumidity, float bmePressure)
{
    Serial.print("Temperature = ");
    Serial.print(bmeTemperature);
    Serial.println(" *C");

    Serial.print("Humidity = ");
    Serial.print(bmeHumidity);
    Serial.println(" %");

    Serial.print("Pressure = ");
    Serial.print(bmePressure);
    Serial.println(" hPa");

    return;
}
