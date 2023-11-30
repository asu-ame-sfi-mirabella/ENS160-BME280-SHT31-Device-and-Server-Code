# ESP32 T7 S3 Sensor Data Collection and Server Integration

## Project Overview

This project involves the ESP32 T7 S3 microcontroller interfacing with a suite of environmental sensors to collect data and send it to a server. The server, built with Flask, receives, processes, and displays this data. This setup is ideal for monitoring environmental conditions like air quality, temperature, humidity, and atmospheric pressure.

## Hardware Components

- **Microcontroller**: ESP32 T7 S3
- **Sensors**:
  - **SparkFun ENS160**: Air Quality Sensor for detecting AQI, TVOC, and eCO2.
  - **SparkFun BME280**: Sensor for measuring humidity, barometric pressure, and ambient temperature.
  - **Adafruit SHT31**: Sensor for precise measurement of soil temperature and humidity.

## Software Components

- **ESP32 Firmware**: Written in C++, this firmware collects data from the sensors and sends it to the server via HTTP POST requests.
- **Flask Server**: A Python-based server that receives sensor data and provides endpoints for data retrieval and display.

## Setup and Usage

1. **ESP32 Setup**: Flash the ESP32 T7 S3 with the provided firmware. Ensure it is connected to the same network as the server.
2. **Server Setup**: Run the Flask server on a machine in the same network. The server listens for incoming data on port 7771.
3. **Data Monitoring**: Access the `/display` route on the server to view the latest sensor readings in a user-friendly format.

## Data Format

Sensor data is sent in JSON format, with clear labels and units for each measurement, ensuring ease of readability and interpretability.

Example Data Format:

```json
{
  "Air Quality Index (AQI)": 1,
  "Atmospheric Pressure [hPa]": 924.16,
  "Estimated CO2 (eCO2) [ppm]": 458,
  "Humidity [%]": 34.71,
  "Soil Humidity [%]": 47.6,
  "Soil Temperature [°C]": 24.71,
  "Total Volatile Organic Compounds (TVOC) [ppb]": 50
}
```

## Conclusion

This project demonstrates the integration of ESP32 with various environmental sensors and a server for data collection and display. It can be adapted for various applications like home environmental monitoring, greenhouse management, or educational purposes.

---

Feel free to modify this README to include any additional details or instructions specific to your project.
