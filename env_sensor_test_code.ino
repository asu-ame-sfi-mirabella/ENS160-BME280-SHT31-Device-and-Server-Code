#include <Wire.h>
#include "SparkFun_ENS160.h"

SparkFun_ENS160 myENS;

void setup()
{
    Wire.begin(8, 9);
    Serial.begin(115200);

    if (!myENS.begin())
    {
        Serial.println("ENS160 did not begin.");
        while (1)
            ; // Halt execution if the sensor does not start
    }

    myENS.setOperatingMode(SFE_ENS160_STANDARD);
}

void loop()
{
    if (myENS.checkDataStatus())
    {
        Serial.print("Air Quality Index (AQI): ");
        Serial.println(myENS.getAQI());

        Serial.print("Total Volatile Organic Compounds (TVOC) [ppb]: ");
        Serial.println(myENS.getTVOC());

        Serial.print("Estimated CO2 (eCO2) [ppm]: ");
        Serial.println(myENS.getECO2());
    }

    delay(200); // Delay to prevent flooding the serial output
}
