
#ifdef MQTT_TLS 
	// Secure client connection class
	BearSSL::WiFiClientSecure espClient;
	// Secure client connection class
	PubSubClient mqttClient (espClient);
#else
	WiFiClient espClient;
	PubSubClient mqttClient (espClient);
#endif


void SuscribeMqtt()
{
	mqttClient.subscribe("dispositivos/sensores/rack/ventilador");
}

String payload;
void PublisMqtt(const char* topic , unsigned long data)
{
	payload = "";
	payload = String(data);
	mqttClient.publish(topic, (char*)payload.c_str());
}

String content = "";
void OnMqttReceived(char* topic, byte* payload, unsigned int length) 
{
	Serial.print("Received on ");
	Serial.print(topic);
	Serial.print(": ");

	content = "";	
	for (size_t i = 0; i < length; i++) {
		content.concat((char)payload[i]);
	}
	Serial.print(content);
	Serial.println();
}