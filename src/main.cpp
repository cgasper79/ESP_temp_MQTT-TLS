/**
 * @file main.cpp
 * @author cgasper79
 * @brief 
 * @version 2.0-SSL
 * @date 2024-01-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include <Adafruit_Sensor.h>
//#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <AsyncMqttClient.h>
#include <WiFiClientSecure.h> 
#include <time.h>


#include "config.h"
#include "NtpUtils.hpp"
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

  ConnectWiFi_STA(true); //Ini WIFI
  InitMqtt(); //Ini MQTT
  

  // Initialize sensor device.
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);

  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

// Read Temperature
float getTemperature(){
   dht.temperature().getEvent(&event);
   while (isnan(event.temperature)) {
    dht.temperature().getEvent(&event);
   }
   return event.temperature;
}

// Read Humidity
float getHumidity(){
   dht.humidity().getEvent(&event);
   while (isnan(event.temperature)) {
    dht.humidity().getEvent(&event);
   }
   return event.relative_humidity;
}

//Wait to read
void waitRead(){
 //Serial.println("Esperando 1 segundo");
 while ((currentMillis - previousMillis) <= delayMS){
    currentMillis = millis();
  }
  previousMillis = currentMillis;
  currentMillis = millis();
}

//Main Loop

void loop() {
  
  HandleMqtt();
 
  // Enable DHT11
  digitalWrite(DHTVCC, HIGH);
  waitRead();
   
  //Read Temperature and humidity and publish in topic
  PublisMqtt(MQTT_PUB_TOPIC_TEMP,getTemperature());
  Serial.println(getTemperature());
  waitRead();
  PublisMqtt(MQTT_PUB_TOPIC_HUMIDITY,getHumidity());
	Serial.println(getHumidity());
 

  //Deep Sleep only 10 seconds connected
  #ifdef ESP_SLEEP 
    if (currentMillis >= intervalSleep){
      Serial.println ("Go to sleep");
      ESP.deepSleep(ESP_SLEEP);
    }
  #endif

}

