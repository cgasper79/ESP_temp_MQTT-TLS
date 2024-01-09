//Version
String version = "v2.0-SSL";

//define user, if you don't use comment please
#define MQTT_TLS  // to use MQTT TLS 
#define ESP_SLEEP 3600000000 // Time in uS to sleep 

//WIFI config
const char* ssid     = "YOUR SSID";
const char* password = "YOUR PASSWORD";
const char* hostname = "ESP32_Temp";

IPAddress ip(192, 168, 0, 6);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(1, 1, 1, 1); 
IPAddress secondaryDNS(1, 0, 0, 1);

// info 
IPAddress miIP;
IPAddress mascaraRed;
IPAddress puertaEnlace;
IPAddress miDNS;
String miRSSI;
String macAddr;

// Variables to reconnect wifi (verify each 10 seconds) 
unsigned long previousWifiMillis = 0;
unsigned long currentMillis = 0;
unsigned long previousPushMillis = 0;
unsigned long intervalWifi = 10000;

// Variables to time Read Sensor
unsigned long previousMillis = 0;
unsigned long intervalRead = 1000;
unsigned long intervalSleep = 10000;

//MQTT config
const char* MQTT_BROKER_ADRESS = "YOUR IP OR HOST MQTT";
#ifdef MQTT_TLS
	const uint16_t MQTT_PORT = 8883;
#else
	const uint16_t MQTT_PORT = 1883;
#endif
const char* MQTT_CLIENT_NAME = "ESP32_Temp_Blanes";
const char* MQTT_USER = "guest";
const char* MQTT_PASS = "123456";
const char MQTT_PUB_TOPIC_TEMP[] = "your/topic/sensor/temperature";
const char MQTT_PUB_TOPIC_HUMIDITY[] = "your/topic/sensor/humidity";


// Certificates

#ifdef MQTT_TLS

static const char cacert[] PROGMEM = R"EOF(

	-----BEGIN CERTIFICATE-----
MIIDxTCCAq2gAwIBAgIUStHVcPqUgPvCpfC5b5jg4s9pPUMwDQYJKoZIhvcNAQEL
BQAwcjELMAkGA1UEBhMCRVMxEDAOBgNVBAgMB1NhbnRha28xEjAQBgNVBAcMCUJh
cmNlbG9uYTEYMBYGA1UECgwPR2FzcGVyd2ViZGVzaWduMQswCQYDVQQLDAJDQTEW
MBQGA1UEAwwNMTkyLjE2OC4xLjE0MDAeFw0yMzEyMjcxNjU0NDFaFw0zMzEyMjQx
NjU0NDFaMHIxCzAJBgNVBAYTAkVTMRAwDgYDVQQIDAdTYW50YWtvMRIwEAYDVQQH
DAlCYXJjZWxvbmExGDAWBgNVBAoMD0dhc3BlcndlYmRlc2lnbjELMAkGA1UECwwC
Q0ExFjAUBgNVBAMMDTE5Mi4xNjguMS4xNDAwggEiMA0GCSqGSIb3DQEBAQUAA4IB
DwAwggEKAoIBAQCsBDg3A4ShnG0e7PvRdkVOoecigD+3AosC+x70tR62ez7d//AV
ohK+UkZdXZvF0h2BgKL7RDMPpucTU7KqQEKADiYItoCeTm+Wc+L/O2CDu74APzPJ
m8xKzXw0OaBPCDLaqP7Gyw4qvbZ6s2BFCgwxQZ0PTA89sMSHxwDHC0/UKBnankUV
uxBE5T7/+k/W79hB5NRIhEvGbev7cMAGc+EzlYlPucGRY8MZPQsRqErQ0jt6jD/f
VWLitDE7MdnPYjooxSE3B6MGm5naX3zxfEWw4Q8eNem949FXs3lJD2vpGR+rGkj8
XQYIlupR3T2BvlJt0P+BuO8jA+GENTuYzBf7AgMBAAGjUzBRMB0GA1UdDgQWBBSN
SSDvc2Uot9DE
-----END CERTIFICATE-----
)EOF";

static const char client_cert[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDbzCCAlcCFFHOt9TQUy6Mqj3zawXyEQbAT9TZMA0GCSqGSIb3DQEBCwUAMHIx
CzAJBgNVBAYTAkVTMRAwDgYDVQQIDAdTYW50YWtvMRIwEAYDVQQHDAlCYXJjZWxv
bmExGDAWBgNVBAoMD0dhc3BlcndlYmRlc2lnbjELMAkGA1UECwwCQ0ExFjAUBgNV
BAMMDTE5Mi4xNjguMS4xNDAwHhcNMjMxMjI3MTY1NDQyWhcNMzMxMjI0MTY1NDQy
WjB2MQswCQYDVQQGEwJFUzEQMA4GA1UECAwHU2FudGFrbzESMBAGA1UEBwwJQmFy
Y2Vsb25hMRgwFgYDVQQKDA9HYXNwZXJ3ZWJkZXNpZ24xDzANBgNVBAsMBkNsaWVu
dDEWMBQGA1UEAwwNMTkyLjE2OC4xLjE0MDCCASIwDQYJKoZIhvcNAQEBBQADggEP
ADCCAQoCggEBAKS/QaQ4qpBDG38JGAjHkDxJzO3S02drQ/ErziZKBblxTZgtV9Fo
0lCQTPMa6hjykkSyx/5TYINSntPMNumTos9jqWZrlPv1eY1Kj1cmgicFTWJkmlSI
LNCPH/hhu8O+8TJqxp3XhaHom9kkK2INeUnHvMMpGcez1VKC/6r7E7bGSd5fzWKt
/udcrZbUVY/QxX+Qs8udC1xCaVrPFnNstO9UP1qao0BPdCEhZ2qYv4/nTRm8a9o+
cO2vQCWTzuDebxloAr0EguaWTIenP7/WuPEJ4XUJmTWF0MsG41KiCBcSlT42L+YQ
KC0ZEWvgRfi4nsMZg30tAC6Y4uVRktMQ5IUCAwEAATANBgkqhkiG9w0BAQsFAAOC
AQEAC3dIkIr96bLj5gGS2hWta8TVQhBe2p59IRM2l94LI75ZqTfvBrfWRyoCSY90
GNVmV+DXvMsdiPT5z+EZB2eUfs72u8Njt7cpO8yzc2RntOVYX8eiS2JnIFVuuul+
p2I7yoAHAhQChz7ydmKoUH/RfEpguEpjRToBHdEwHZIZZMJGGGMhswue8nVl6b4a
i8QB7CeGSc/pxrERKTTmgX3NQA==
-----END CERTIFICATE-----
)KEY";

static const char privkey[] PROGMEM = R"KEY(
-----BEGIN PRIVATE KEY-----
MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQCkv0GkOKqQQxt/
CRgIx5A8Sczt0tNna0PxK84mSgW5cU2YLVfRaNJQkEzzGuoY8pJEssf+U2CDUp7T
pz+/1rjxCeF1CZk1hdDLBuNSoggXEpU+Ni/mECgtGRFr4EX4uJ7DGYN9LQAumOLl
UZLTEOSFAgMBAAECggEBAJeHjqG0uTT9dHHw9FlG7OR24PWyH14VLCXMK83N8dvy
WfNpaUekSSVuxEgqbj1KB70GT6seFzxSpuGaCLvoXiywtyQMOblRvF53wMXhFn87
gkTI0aaEpeuiaT507lYJ7yF3Un3luPUF1Srw+LI0QGXbQESlLiAfyUseUHh8fVJz
Lvo4lAEv/yKtbtA+mHumLY2PvMi49KR/FSpDPCxSfFYUPqpe5toH69M1EqspAa7v
BkEWsoKQFDFeTqRZONrJz/Y+d0Dhr7+0LIn+4+CAeePTZIzfE+y8OO35Za0GDMGb
KuG+JaFfyv+dL27OkD6Kh8VkOBe3XAT11paGsN4z4QECgYEA2uaZWaoBjDddemsJ
ZbkFnVgMsbhx/DgUESnhyokiuTrPScrV4tiWKwmQwNvnqMCpVQqE0pjAKfUcFIYg
9FbsO/Q1mqNPUyyT1CFtfdg99vBpsxi/8fMadC+BaNIQSCjqWEZaoZ76ZbsJsiod
e65Qn7XlJ77oA5n9JR5FTjct9cECgYEAwKsabObeCsm7FNuhYcY6UJlPg8UCTvQf
wxN7OgmciUlZpEkj0EwkGJvSsNNQmYrtaIwOOP57nqR5q/70NJg/M6RW3lq5XfG0
PbTIhbuQKzxNXVWkkn8/U639Z4z5yqsAQFhJvZCdRdxHaPT1qwY+gKjMhf3qh0VQ
k7f/+cqah8UCgYB2X50tz7FmkcoLGTbLu3I91A+JS6TYL7sqTOu9CsusPxE5Zmaj
dVw/ibytjj2IFCd9xWJuQAwqpFG7B41qlFbLVRRnu0h6QfDvFeSP2RApKXj/ULL7
s+rqSYZBmNcVLosLkYARRldCDBMCNaQb7ffLZiG1jJcK24r5N5Vi5pQEAQKBgAHF
0UFtQl/BCWqgtrRqmYDCWFvzBRlq35vw4vJewkT1GtELJzw0ZySe0mp0F0rCX3rV
-----END PRIVATE KEY-----
)KEY";

#endif






