
#ifdef MQTT_TLS 
    // X.509 parsed 
	BearSSL::X509List cert(cacert);
	BearSSL::X509List client_crt(client_cert);
	BearSSL::PrivateKey key(privkey);
	// Secure client connection class
	BearSSL::WiFiClientSecure espClient;
	// Secure client connection class
	PubSubClient mqttClient (espClient);
#else
	WiFiClient espClient;
	PubSubClient mqttClient (espClient);
#endif

//MQTT homeAssitant Discovery
void MqttHomeAssistantDiscovery()
{
	String discoveryTopic;
    String payload;
    String strPayload;

	if (mqttClient.connected())
	{
		Serial.println("SEND HOME ASSISTANT DISCOVERY!!!");
        StaticJsonDocument<600> payload;
        JsonObject device;
        JsonArray identifiers;

		/*Temperature*/

		discoveryTopic = "homeassistant/sensor/" + MQTT_UNIQUE_ID + "/" + MQTT_CLIENT_NAME + "_temp" + "/config";
    
        payload["name"] = MQTT_CLIENT_NAME + "Temperature";
        payload["uniq_id"] = MQTT_UNIQUE_ID + "_temp";
        payload["stat_t"] = MQTT_STATUS_TOPIC;
        payload["dev_cla"] = "temperature";
        payload["val_tpl"] = "{{ value_json.temp | is_defined }}";
        payload["unit_of_meas"] = "°C";
        device = payload.createNestedObject("device");
        device["name"] = MQTT_CLIENT_NAME;
        device["model"] = MQTT_DEVICE_MODEL;
        device["sw_version"] = MQTT_SW_VERSION;
        device["manufacturer"] = MQTT_MANUFACTURED;
        identifiers = device.createNestedArray("identifiers");
        identifiers.add(MQTT_UNIQUE_ID);
        serializeJson(payload, strPayload);
        mqttClient.publish(discoveryTopic.c_str(), strPayload.c_str());

		/*Humidity*/

		payload.clear();
        device.clear();
        identifiers.clear();
        strPayload.clear();

        discoveryTopic = "homeassistant/sensor/" + MQTT_UNIQUE_ID + "/" + MQTT_CLIENT_NAME + "_hum" + "/config";
        
        payload["name"] = MQTT_CLIENT_NAME + "Humidity";
        payload["uniq_id"] = MQTT_UNIQUE_ID + "_hum";
        payload["stat_t"] = MQTT_STATUS_TOPIC;
        payload["dev_cla"] = "humidity";
        payload["val_tpl"] = "{{ value_json.hum | is_defined }}";
        payload["unit_of_meas"] = "%";
        device = payload.createNestedObject("device");
        device["name"] = MQTT_CLIENT_NAME;
        device["model"] = MQTT_DEVICE_MODEL;
        device["sw_version"] = MQTT_SW_VERSION;
        device["manufacturer"] = MQTT_MANUFACTURED;
        identifiers = device.createNestedArray("identifiers");
        identifiers.add(MQTT_UNIQUE_ID);

        serializeJson(payload, strPayload);

        mqttClient.publish(discoveryTopic.c_str(), strPayload.c_str());
	}
}


// When MQTT received
void OnMqttReceived(char* topic, byte* inFrame, unsigned int length) 
{
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;
    
    for (int i = 0; i < length; i++) 
    {
        Serial.print((char)inFrame[i]);
        messageTemp += (char)inFrame[i];
    }
    Serial.println();
  
    if(String(topic) == String("homeassistant/status")) 
    {
        if(messageTemp == "online")
            MqttHomeAssistantDiscovery();
			mqttClient.loop();
    }
}


//Ini MQTT
void InitMqtt() 
{    
    #ifdef MQTT_TLS
	    //espClient.setTrustAnchors(&cert);
		espClient.setInsecure();
		espClient.setClientRSACert(&client_crt, &key);
		//setClock();
	#endif
	mqttClient.setServer(MQTT_BROKER_ADRESS, MQTT_PORT);
	mqttClient.setCallback(OnMqttReceived);
	mqttClient.setBufferSize(512);
	
}

//Subscribe MQTT Topic for Discovery HomeAssistant
void SuscribeMqtt()
{
	mqttClient.subscribe("homeassistant/status");
	Serial.println ("Topic Subscribed");
}

// connect MQTT
void ConnectMqtt()
{
	while (!mqttClient.connected())
	{
		Serial.print("Starting MQTT connection...");
		if (mqttClient.connect(MQTT_CLIENT_NAME.c_str(), MQTT_USER, MQTT_PASS))
		{
			SuscribeMqtt();
		}
		else
		{
			Serial.print("Failed MQTT connection, rc=");
			Serial.print(mqttClient.state());
			Serial.println(" try again in 5 seconds");

			delay(5000);
		}
	}
}

//HandleMqtt
void HandleMqtt()
{
	if (!mqttClient.connected())
	{
		ConnectMqtt();
	}
	mqttClient.loop();
}



String payload;
void PublisMqtt(const char* topic , unsigned long data)
{
	payload = "";
	payload = String(data);
	mqttClient.publish(topic, (char*)payload.c_str());
}

String content = "";

