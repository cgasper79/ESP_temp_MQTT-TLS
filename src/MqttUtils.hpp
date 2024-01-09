
#ifdef MQTT_TLS
	// X.509 parsed 
	BearSSL::X509List cert(cacert);
	BearSSL::X509List client_crt(client_cert);
	BearSSL::PrivateKey key(privkey);
#endif


void InitMqtt() 
{ 
     
    #ifdef MQTT_TLS
	    //espClient.setTrustAnchors(&cert);
		espClient.setInsecure();
		espClient.setClientRSACert(&client_crt, &key);
		//setClock();
	#endif
	mqttClient.setServer(MQTT_BROKER_ADRESS, MQTT_PORT);
	//mqttClient.setCallback(OnMqttReceived);
	
}

// connect MQTT
void ConnectMqtt()
{
	while (!mqttClient.connected())
	{
		Serial.print("Starting MQTT connection...");
		if (mqttClient.connect(MQTT_CLIENT_NAME, MQTT_USER, MQTT_PASS))
		{
			//SuscribeMqtt();
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