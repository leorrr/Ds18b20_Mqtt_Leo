#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_

#define CFG_HOLDER	0x00FF55A4	/* Change this value to load default configurations */
#define CFG_LOCATION	0x3C	/* Please don't change or if you know what you doing */
#define CLIENT_SSL_ENABLE

/*DEFAULT CONFIGURATIONS*/
#define TOPIC_info "test/sensor1/info" // topic mensaje info Croms/sensor1/info
#define TOPIC_temp "test/sensor1/temp"// topic mensaje temp Croms/sensor1/temp
#define TOPIC_lwt "test/sensor1/lwt" // topic mensaje lwt Croms/sensor1/lwt

#define MQTT_HOST			"10.0.0.9" //or "mqtt.yourdomain.com"
#define MQTT_PORT			1883
#define MQTT_BUF_SIZE		1024
#define MQTT_KEEPALIVE		120	 /*second*/

#define MQTT_CLIENT_ID		"test_001"
#define MQTT_USER			"user"
#define MQTT_PASS			"password"
//configuracion wifi
#define STA_SSID "yourssid"
#define STA_PASS "yourpassword"
#define STA_TYPE AUTH_WPA2_PSK

#define MQTT_RECONNECT_TIMEOUT 	5	/*second*/

#define DEFAULT_SECURITY	0
#define QUEUE_BUFFER_SIZE		 		2048

#define PROTOCOL_NAMEv31	/*MQTT version 3.1 compatible with Mosquitto v0.15*/
//PROTOCOL_NAMEv311			/*MQTT version 3.11 compatible with https://eclipse.org/paho/clients/testing/*/
#endif
