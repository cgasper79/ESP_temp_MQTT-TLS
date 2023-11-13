/**
 * @file main.cpp
 * @author cgasper79
 * @brief 
 * @version 1.0
 * @date 2023-11-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#include <Adafruit_Sensor.h>
//#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <AsyncMqttClient.h>

#include "config.h"
#include "WifiUtils.hpp"
#include "MQTT.hpp"
#include "MqttUtils.hpp"


#define DHTPIN 13     // Digital pin connected to the DHT sensor - Pin D7 (GPIO13)
#define DHTVCC 5      // Pin connected to the VCC DHT sensor - Pin D1 (GPIO5)
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.
#define DHTTYPE    DHT11     // DHT 11

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
sensors_event_t event;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); // Led Wifi connection
  pinMode(DHTVCC, OUTPUT); // Power Supply DHT11 - Pin D1 (GPIO5)


  ConnectWiFi_STA(true);
  InitMqtt();
  

  // Initialize sensor device.
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);

  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void getValues(){
  // Get temperature event and print its value.
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}

float getTemperature(){
   dht.temperature().getEvent(&event);
   return event.temperature;
}

float getHumidity(){
   dht.humidity().getEvent(&event);
   return event.relative_humidity;
}

void loop() {
  HandleMqtt();
  
  // Enable DHT11
  digitalWrite(DHTVCC, HIGH);
  delay (500);

  //Read Temperature and humidity
  for (int i = 0; i <= 5; i++) {
    PublisMqtt("dispositivos/sensores/rack/temperature",getTemperature());
    delay (2000);
    PublisMqtt("dispositivos/sensores/rack/humidity",getHumidity());
	  delay(2000);
  }
   
  // Disable DHT11
  digitalWrite(DHTVCC, LOW);
  delay(500);

  //Deep Sleep
  Serial.print ("Go to sleep");
  ESP.deepSleep(180000000);
}

