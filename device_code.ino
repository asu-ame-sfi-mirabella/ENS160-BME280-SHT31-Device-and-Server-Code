#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "SparkFun_ENS160.h"
#include "SparkFunBME280.h"
#include "Adafruit_SHT31.h"

// Wi-Fi credentials
const char *ssid = "SETUP-AE05";
const char *password = "faucet4039dozed";

// AWS server details
const char *serverName = "44.225.155.76";
const int serverPort = 7771;

SparkFun_ENS160 myENS;
BME280 myBME280;
Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup()
{
    Wire.begin(8, 9);
    Serial.begin(115200);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected to WiFi");

    if (!myENS.begin())
    {
        Serial.println("ENS160 did not begin.");
        while (1)
            ;
    }

    if (!myBME280.beginI2C())
    {
        Serial.println("BME280 did not respond.");
        while (1)
            ;
    }

    myENS.setOperatingMode(SFE_ENS160_STANDARD);
    sht31.begin(0x44);
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        if (myENS.checkDataStatus())
        {
            HTTPClient http;
            http.begin(serverName, serverPort);

            int httpResponseCode = http.POST(createJSONPayload());
            if (httpResponseCode > 0)
            {
                Serial.print("HTTP Response code: ");
                Serial.println(httpResponseCode);
            }
            else
            {
                Serial.print("Error code: ");
                Serial.println(httpResponseCode);
            }

            http.end();
        }
    }
    else
    {
        Serial.println("WiFi Disconnected");
    }

    delay(200);
}

String createJSONPayload()
{
    String payload = "{";

    payload += "\"Air Quality Index (AQI)\": " + String(myENS.getAQI()) + ",";
    payload += "\"Total Volatile Organic Compounds (TVOC) [ppb]\": " + String(myENS.getTVOC()) + ",";
    payload += "\"Estimated CO2 (eCO2) [ppm]\": " + String(myENS.getECO2()) + ",";
    payload += "\"Humidity [%]\": " + String(myBME280.readFloatHumidity()) + ",";
    payload += "\"Atmospheric Pressure [hPa]\": " + String(myBME280.readFloatPressure() / 100.0) + ",";
    payload += "\"Soil Temperature [°C]\": " + String(sht31.readTemperature()) + ",";
    payload += "\"Soil Humidity [%]\": " + String(sht31.readHumidity());

    payload += "}";

    return payload;
}