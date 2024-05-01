#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "SparkFun_ENS160.h"
#include "SparkFunBME280.h"
#include "Adafruit_SHT31.h"

// Wi-Fi credentials - Changed to match old sensor code
const char *ssid = "LGMesa";
const char *password = "mediatedx001";

// AWS server details - Changed to match old sensor code
const char *serverName = "http://44.242.66.1:8089/sendData";
String macAddress;

SparkFun_ENS160 myENS;
BME280 myBME280;
Adafruit_SHT31 sht31 = Adafruit_SHT31();

unsigned long lastTime = 0;
unsigned long timerDelay = 2000;

void setup()
{
    Wire.begin(14, 13);
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected to WiFi");

    // chek if the ENS sensor is connected
    if (!myENS.begin())
    {
        Serial.println("ENS160 did not begin.");
        while (1)
            ;
    }
    // check if the BME280 sensor is connected
    if (!myBME280.beginI2C())
    {
        Serial.println("BME280 did not respond.");
        while (1)
            ;
    }
    // check if the SHT31 sensor is connected
    if (!sht31.begin(0x44))
    {
        Serial.println("SHT31 did not begin.");
        while (1)
            ;
    }

    myENS.setOperatingMode(SFE_ENS160_STANDARD);
    sht31.begin(0x44);
    macAddress = WiFi.macAddress(); // Get MAC address

    // print macAddress
    Serial.println(macAddress);
}

String createSensorDataString(float dhtTemp, float dhtHum, float lux,
                              float sht31Temp, float sht31Hum,
                              int aqi, int tvoc, int eco2,
                              float bme280Hum, float bme280Press)
{
    String dataString = "";

    // DHT Sensor (Old Device)
    dataString += "&dht_temp=" + (isnan(dhtTemp) ? "" : String(dhtTemp));
    dataString += "&dht_hum=" + (isnan(dhtHum) ? "" : String(dhtHum));

    // Light Sensor (Old Device)
    dataString += "&lux=" + (isnan(lux) ? "" : String(lux));

    // SHT31 Sensor (Common)
    dataString += "&sht31_temp=" + (isnan(sht31Temp) ? "" : String(sht31Temp));
    dataString += "&sht31_hum=" + (isnan(sht31Hum) ? "" : String(sht31Hum));

    // ENS160 Sensor (New Device)
    dataString += "&aqi=" + (aqi < 0 ? "" : String(aqi));
    dataString += "&tvoc=" + (tvoc < 0 ? "" : String(tvoc));
    dataString += "&eco2=" + (eco2 < 0 ? "" : String(eco2));

    // BME280 Sensor (New Device)
    dataString += "&bme280_hum=" + (isnan(bme280Hum) ? "" : String(bme280Hum));
    dataString += "&bme280_press=" + (isnan(bme280Press) ? "" : String(bme280Press / 100.0)); // Convert to hPa

    return dataString;
}

void loop()
{

    // Check if it's time to send data
    if ((millis() - lastTime) > timerDelay)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            WiFiClient client;
            HTTPClient http;

            String sensorData = createSensorDataString(NAN, NAN, NAN,                                    // Old device sensors
                                                       sht31.readTemperature(), sht31.readHumidity(),    // Common sensor
                                                       myENS.getAQI(), myENS.getTVOC(), myENS.getECO2(), // New device sensors
                                                       myBME280.readFloatHumidity(), myBME280.readFloatPressure());

            String serverPath = String(serverName) + "?mac=" + String(macAddress) + sensorData;

            http.begin(client, serverPath.c_str());

            // Send HTTP GET request
            int httpResponseCode = http.GET();

            // Handle server response
            if (httpResponseCode > 0)
            {
                Serial.print("HTTP Response code: ");
                Serial.println(httpResponseCode);
                String payload = http.getString();
                Serial.println(payload);
            }
            else
            {
                Serial.print("Error code: ");
                Serial.println(httpResponseCode);
            }

            // Free resources
            http.end();
        }
        else
        {
            Serial.println("WiFi Disconnected");
        }

        lastTime = millis(); // Update last time data was sent
    }
}
