/**
 * @file main.cpp
 * @author cgasper79
 * @brief 
 * @version 2.0
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


float getTemperature(){
   dht.temperature().getEvent(&event);
   while (isnan(event.temperature)) {
    dht.temperature().getEvent(&event);
   }
   return event.temperature;
}

float getHumidity(){
   dht.humidity().getEvent(&event);
   while (isnan(event.temperature)) {
    dht.humidity().getEvent(&event);
   }
   return event.relative_humidity;
}

void waitRead(){
 //Serial.println("Esperando 1 segundo");
 while ((currentMillis - previousMillis) <= delayMS){
    currentMillis = millis();
  }
  previousMillis = currentMillis;
  currentMillis = millis();
}

void loop() {
  
  HandleMqtt();
  // Enable DHT11
  digitalWrite(DHTVCC, HIGH);
  waitRead();

  //Read Temperature and humidity
  PublisMqtt("dispositivos/sensores/rack/temperature",getTemperature());
  Serial.println(getTemperature());
  waitRead();
  PublisMqtt("dispositivos/sensores/rack/humidity",getHumidity());
	Serial.println(getHumidity());
  
  // Disable DHT11
  //digitalWrite(DHTVCC, LOW);
  //delay(50);

  //Deep Sleep for 1 hour only 10 seconds connected
  if (currentMillis >= intervalSleep){
    Serial.println ("Go to sleep");
    ESP.deepSleep(3600000000);
  }
  
}

