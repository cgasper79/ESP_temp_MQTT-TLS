//Variables versiones
String version = "v1.0";

//Configuración WIFI
const char* ssid     = "Domo";
const char* password = "domotica2022";
const char* hostname = "ESP32_Temp";

const char* MQTT_BROKER_ADRESS = "192.168.1.140";
const uint16_t MQTT_PORT = 1883;
const char* MQTT_CLIENT_NAME = "ESP32_Temp";
const char* MQTT_USER = "guest";
const char* MQTT_PASS = "123456";

IPAddress ip(192, 168, 1, 5);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(1, 1, 1, 1); 
IPAddress secondaryDNS(1, 0, 0, 1);

// Variables información conexión
IPAddress miIP;
IPAddress mascaraRed;
IPAddress puertaEnlace;
IPAddress miDNS;
String miRSSI;
String macAddr;


// variables para tiempo reconexion wifi (cada 10 segundos comprueba) 
unsigned long previousWifiMillis = 0;
unsigned long currentMillis = 0;
unsigned long previousPushMillis = 0;
unsigned long intervalWifi = 10000;