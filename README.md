## Project: Temperature and Humidity Sensor with Wemos D1 mini and DHT11 Sensor (PlatformIO)

This project consists of creating a temperature and humidity sensor using a Wemos D1 mini microcontroller along with a DHT11 sensor. The data collected by the sensor is securely published to an MQTT topic using the TLS/SSL protocol.

### Hardware Requirements:

- Wemos D1 mini (ESP8266)
- DHT11 temperature and humidity sensor
- Connection to a Wi-Fi network with internet access

### Software Requirements:

- PlatformIO development platform
- Necessary libraries for the DHT11 sensor and MQTT communication
- MQTT broker with TLS/SSL support (e.g., Mosquitto)


### Schematic
![Schematic](https://github.com/cgasper79/ESP_temp_MQTT-TLS/assets/51058790/48172904-360a-4b9c-a591-77bfcdc4dc29)


### Setup:

1. **Hardware:**
   - Connect the DHT11 sensor to the data pin of the Wemos D1 mini.
   - Power the Wemos D1 mini with an appropriate power source.

2. **Software:**
   - Open the project in PlatformIO.
   - Ensure that the `platformio.ini` file is configured correctly with the compilation and device setup options.
   - Install the necessary libraries for the DHT11 sensor and MQTT.
   - Configure the Wi-Fi connection parameters and MQTT server settings in the code.
   - Compile and upload the code to the Wemos D1 mini.

### Usage:

Once the device is powered and connected to the Wi-Fi network, it will begin collecting temperature and humidity data using the DHT11 sensor. This data will be securely sent to an MQTT broker using the TLS/SSL protocol. You can subscribe to the corresponding MQTT topic to receive and process the data as needed.

### Additional Notes:

- It is important to ensure that the MQTT broker the device is connecting to supports secure connections via TLS/SSL.
- Additional security measures, such as authentication on the MQTT broker, are recommended as needed for your specific application.

### Contribution:

Contributions are welcome! If you'd like to improve this project, feel free to open an issue or submit a pull request.

### Credits:

This project was developed by [@cgasper79](https://github.com/cgasper79).

### License:

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
