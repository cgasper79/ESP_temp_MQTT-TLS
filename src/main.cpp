/**
 * @file main.cpp
 * @author cgasper79
 * @brief 
 * @version 2.1
 * @date 2024-01-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <Adafruit_Sensor.h>
#include <DHT.h>
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


#define DHTPIN    13     // Digital pin connected to the DHT sensor - Pin D7 (GPIO13)
#define DHTVCC    5      // Pin connected to the VCC DHT sensor - Pin D1 (GPIO5)
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.
#define DHTTYPE   DHT11  // DHT 11

//Global Variables
bool g_InitSystem = true;
float TempSensor;
float HumSensor;
DHT dht(DHTPIN, DHTTYPE);
uint32_t delayMS = 1000;


//Setup
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); // Led Wifi connection
  pinMode(DHTVCC, OUTPUT); // Power Supply DHT11 - Pin D1 (GPIO5)

  ConnectWiFi_STA(true); //Ini WIFI
  InitMqtt(); //Ini MQTT
  
  // Initialize sensor device.
  dht.begin();
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

void loop() 
{
  
  HandleMqtt();

  //Send HomeAssistant Discovery
  if(g_InitSystem)
  {
    delay(100);
    g_InitSystem = false;
    Serial.println("INIT SYSTEM...");
    MqttHomeAssistantDiscovery();     // Send Discovery Data
    Serial.println ("Sensor Unique ID: " + MQTT_UNIQUE_ID);
    Serial.println ("Sensor FW Version: " + String(MQTT_SW_VERSION));
  }

  // Enable DHT11 and Read Temperature and humidity and Publish in topic
  digitalWrite(DHTVCC, HIGH);

  while ((isnan(HumSensor) || isnan(TempSensor)) || ((HumSensor == 0) || (TempSensor == 0)))
  {
    Serial.println("Read Sensor Error");
    waitRead();
    HumSensor = dht.readHumidity();
    waitRead();
    TempSensor = dht.readTemperature();
    delay(100);
  }
  waitRead();
  Serial.println("Temp:" + String(TempSensor) + " " + "Hum:" + String(HumSensor));
  PublisMqtt(TempSensor, HumSensor);

  //Deep Sleep only 10 seconds connected
  #ifdef ESP_SLEEP 
    if (currentMillis >= intervalSleep){
      Serial.println ("Go to sleep");
      ESP.deepSleep(ESP_SLEEP);
    }
  #endif

}

