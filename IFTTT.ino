/**
 * @file        IFTTT.ino
 * @brief       Send Values as a HTTP Request to IFTTT Maker Webhooks Service from ESP32
 * @author      Keitetsu
 * @date        2018/03/02
 * @copyright   Copyright (c) 2018 Keitetsu
 * @par         License
 *              This software is released under the MIT License.
 */

#include <WiFi.h>

#include "IFTTT_config.h"


/**
 * @name    IFTTT Configuration
 */
/*! @{ */
const char *iftttHost = "maker.ifttt.com";
String makerEvent = MAKER_EVENT;
String makerKey = MAKER_KEY;
/*! @} */

WiFiClient client;


void sendRequestToIFTTT(String value1, String value2, String value3)
{
    Serial.println();
    Serial.println();
    Serial.println("Connecting to server ...");
    if (!client.connect(iftttHost, 80)) {
        Serial.println("Connection failed.");
    }
    else {
        Serial.println("Connected to server.");
        
        // Make a HTTP request
        String url = "/trigger/" + makerEvent + "/with/key/" + makerKey;
        url += "?value1=" + value1 + "&value2=" + value2 + "&value3=" + value3;
        client.println("GET " + url + " HTTP/1.1");
        client.print("Host: ");
        client.println(iftttHost);
        client.println("Connection: close");
        client.println();
        
        Serial.print("Waiting for response ");
        int count = 0;
        while (!client.available()) {
            count++;
            if (count > (20 * 20)) {  // about 20 sec
                Serial.println();
                Serial.println("Failed to send a request to server.");
                
                return;
            }
            
            delay(50);
            Serial.print(".");
        }
        Serial.println();
        
        // if there are incoming bytes available
        // from the server, read them and print them:
        while (client.available()) {
            char c = client.read();
            Serial.write(c);
        }
        Serial.println();

        // if the server's disconnected, stop the client:
        if (!client.connected()) {
            Serial.println("Disconnecting from server ...");
            client.stop();
        }
    }
    
    return;
}
