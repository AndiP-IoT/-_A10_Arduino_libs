//##############################################################
//#########             BOARD ESP32               ##############
#define VERSION "Version"
#include "git-version.h"
//#########  [major].[minor].[patch]-[build/beta/rc]


const char* hostname = "BrdLoc"; // should be like 'Board_Location'
char fileName[120] = __FILE__;
#define MAINFILE fileName

/*
 * ONEWIRE:
 * https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
 * ESP32 by ESPRESSIV System Boardmanager mit 2.0.3 Kompiliert nicht
 * ESP32 by ESPRESSIV System Boardmanager mit 2.0.2 OK
 * ESP32 by ESPRESSIV System Boardmanager mit 2.0.1 OK
 *
 *
 *
 * MONport.:
 * https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
 * ESP32 by ESPRESSIV System Boardmanager mit 2.0.3 OK
 * ESP32 by ESPRESSIV System Boardmanager mit 2.0.2 Problem - funktioniert nicht
 * ESP32 by ESPRESSIV System Boardmanager mit 2.0.1 OK
*/

/*###############################################################
  ######  USER-def settings variablen hier definieren   #########
  ###############################################################*/
/*#####     ######*/
	#define Niv_Pump_zeit 30000
	#define KESSEL_ALARM_MAX_TEMPERATUR 90
	#define DS1820_AUSFALL_TEMPERATUR 5
/*##### USER-def settings variablen END                ######*/


/*###############################################################
  ########      Funktionen und Board Einstellung       ##########
  ###############################################################*/
// comment out the next lines to eliminate stuff
//#define DEBUG
//#define DEBUG_WIFI
//#define DEBUG_MQTT
//#define DEBUG_DS1820

/*  define only one of ESP32 OR ESP8266 OR W550 !!!!! */
	//#define ESP32				// *	just one of these
	//#define ESP8266			// *
	#define W5500				// *
/*  define only one of ESP32 OR ESP8266 OR W550 !!!!! END */

#define MQTT			// only if ESP32 || ESP8266 || W5500

#define DS1820

#define ADS1115_AI
#define ADS1115_PT1000
//########      Funktionen und Board Einstellung  END     ##########


/*###############################################################
  ########                  Predefined                 ##########
  ###############################################################*/
	//	z.B. Relais an DIO 32-39

	#ifdef DS1820
		//##############   OneWire   ###################
		#define ONE_WIRE_BUS_pin 32
		// byte Heiz_VL_ds1820[8] = { 0x10, 0x28, 0xB2, 0xDC, 0x01, 0x08, 0x00, 0xEE }; //1028B2DC010800EE
	#endif

	#if defined(ADS1115_AI) || defined(ADS1115_PT1000)
		byte ADS_AI_addr=0x48;
		byte ADS_PT1000_addr=0x49;
	#endif

	#define Sumpf_schw_in 7		//DIO
	#define Sumpf_P_in 	8		//DIO
	#define Sumpf_P_out 30		//DIO Relais5

	byte testled_status=true;
	#define TESTLED_PIN 13

/*#####      Predefined END    ###############################*/

#ifdef DS1820
	#include <OneWire.h> //https://github.com/PaulStoffregen/OneWire
	/*	by Paul Stoffregen V2.3.6 -> OK		*/
	#include <DallasTemperature.h>  //https://github.com/milesburton/Arduino-Temperature-Control-Library
	/*	by miles burton V3.9.0 -> OK		*/
#endif

#ifdef ESP32
	#include <WiFi.h>  // wird mit Boardmanager mit installiert
#endif
#ifdef ESP8266
	#include <ESP8266WiFi.h>  // wird mit Boardmanager mit installiert
#endif
#ifdef W5500
	#include <SPI.h>
	#include <Ethernet.h>			/*V2.0 Built in Arduino 1.5.15 Version */
#endif
#ifdef MQTT
	#include <PubSubClient.h> 	/*V2.8.0 https://pubsubclient.knolleary.net/
									--> https://github.com/knolleary/pubsubclient/releases/tag/v2.8
										* pubsubclient-2.8.zip */
#endif

#if defined(ADS1115_AI) || defined(ADS1115_PT1000)
	#include <Adafruit_ADS1X15.h>
	#ifdef ADS1115_AI
		Adafruit_ADS1115 ADS_AI;  /* Use this for the 16-bit version */
		int16_t ADS_AI0, ADS_AI1, ADS_AI2, ADS_AI3;
		float ADS_AI_volts0, ADS_AI_volts1, ADS_AI_volts2, ADS_AI_volts3;
	#endif
	#ifdef ADS1115_PT1000
		Adafruit_ADS1115 ADS_PT1000;  /* Use this for the 16-bit version */
		int16_t ADS_PT1000_results1;
		float ADS_PT1000_temperatur1;
		int16_t ADS_PT1000_results2;
		float ADS_PT1000_temperatur2;
		/* Be sure to update this value based on the IC and the gain settings! */
		//float ADS_AI_multiplier = 3.0F;    /* ADS1015 @ +/- 6.144V gain (12-bit results) */
		float ADS_multiplier = 0.1875F; /* ADS1115  @ +/- 6.144V gain (16-bit results) */
	#endif
#endif

//###########   Network | mqtt  ################
#if defined(ESP32) || defined(ESP8266) || defined(W5500)
	IPAddress ip(192,168,1,55);
	IPAddress gw(192, 168, 1, 1);
	IPAddress sn(255, 255, 255, 0);
	const char* ssid = "dd-wrt";		//put your wifi ssid here
	const char* password = "pwd123456";	//put your wifi password here.
#endif

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
#if defined(WIZ550io_WITH_MACADDRESS) // Use assigned MAC address of WIZ550io
	;
#else
	byte mac[] = {0xDE, 0xA8, 0xBE, 0xEF, 0xFE, 0xED};
#endif

#ifdef MQTT
	const char* MQTT_server = "192.168.1.135";
	const char* MQTT_userName = "xyz";
	const char* MQTT_pwd = "pwd123456";
	const char* MQTT_clientId = hostname;
#endif
//###########   Network | mqtt  ################

#ifdef W5500
	EthernetClient NetworkClient;			// Nur für W5500 (Mega)
	void LAN_WIFI_reconnect(){
		Ethernet.begin(mac, ip);
	}

#endif
#if defined(ESP32) || defined(ESP8266)
	WiFiClient NetworkClient;				// ESP32
	#include "A10_wifi.h"
		/* wifi_setup
		* wifi_connect
		*/
	void LAN_WIFI_reconnect(){
		wifi_connect();
	}

#endif

#ifdef MQTT
	#define MSG_BUFFER_SIZE (50)
	char mqtt_publish_msg[MSG_BUFFER_SIZE];
	char mqtt_publish_topic[MSG_BUFFER_SIZE];
	// _______	 mqtt (for incomming messages) PROTOTYPE		  ________
	void MQTT_callback(char* topic, byte* payload, unsigned int length);
	PubSubClient MQTTclient(MQTT_server, 1883, MQTT_callback, NetworkClient);
	#include "A10_mqtt.h"
		/*->MQTT_reconnect
		* ->MQTT_callback
		*/
#endif

//################## Network | MQTT END #######################



#ifdef DS1820
	//##############   OneWire   ###################
	OneWire oneWire(ONE_WIRE_BUS_pin);
	DallasTemperature DS1820_sensors(&oneWire);
	DeviceAddress DS1820_address;

	#define max_DS1820_count 15
	char DS1820_one_address[17];
	char DS1820_all_names[max_DS1820_count][17];
	float DS1820_all_values[max_DS1820_count];
	byte DS1820_deviceCount = 0;
	#include "A10_DS1820.h"  // einige DS1820-Funktionen sind ausgelagert
		/* Sub_DS1820_printAddress
		* Sub_DS1820_print_array
		* Sub_DS1820_allsensors_read
		* Sub_DS1820_all_send_MQTT	   #Ifdef MQTT  //is defined in MAINprog
		*/
	//##############################################
#endif


#include "A10_helper.h"  // einige Funktionen sind ausgelagert
	/* Sub_blink_testLED
	*  Sub_had_reset
	*/

/*###############################################################
  ########       allg Variablen -> PRG Steuerung       ##########
  ###############################################################*/
	#define MONport Serial
	#define MON     MONport.print
	#define MONln   MONport.println

	//unsigned long GW_shutdown_starttime=millis();
	//byte GW_Run_stat=2;		//0 = "Zustand" GW OFF - Relais OFF
							//1 = "Step" Wartezeit zwischen SHUTDOWN und Relais OFF
							//2 = "Zustand" on durch BOOT
							// 3 =  "Zustand" ON durch MQTT
							// OFF durch MQTT schaltet auf 1

/*#####      Predefined END    ###############################*/

void setup(){
	MONport.begin(115200);
	while (!MONport) {
		; // wait for MONport.port to connect. Needed for Native USB only
	}
	#ifdef DEBUG
		MONport.println("main.ino --> SETUP() - checkpoint: STARTUP ");
	#endif
	DS1820_sensors.begin();				// DS1820 Initialiesierung
	DS1820_sensors.requestTemperatures();		// DS1820 Temp bereitstellen

	Sub_printVersion();				// HELPER -> Ausgabe Filename,
								//   Version, Compile Datum

	#if defined(ESP32) || defined(ESP8266)
		#ifdef DEBUG
			MONport.println("main.ino --> SETUP() - checkpoint: wifi_setup()");
		#endif
		wifi_setup();						// call wifi setup
	#endif
	#ifdef W5500
		#ifdef DEBUG
			MONport.println("main.ino --> SETUP() - checkpoint: LAN_setup()");
		#endif
		Ethernet.begin(mac, ip);
		#ifdef DEBUG
			if (Ethernet.hardwareStatus() == EthernetNoHardware) {
				MONport.println("Ethernet shield was not found.");
			}
			else if (Ethernet.hardwareStatus() == EthernetW5100) {
				MONport.println("W5100 Ethernet controller detected.");
			}
			else if (Ethernet.hardwareStatus() == EthernetW5200) {
				MONport.println("W5200 Ethernet controller detected.");
			}
			else if (Ethernet.hardwareStatus() == EthernetW5500) {
				MONport.println("W5500 Ethernet controller detected.");
			}
			MONport.print ("Ethernet.linkStatus : ");
			MONport.println(Ethernet.linkStatus());
			MONport.print ("Ethernet.localIP : ");
			MONport.println(Ethernet.localIP());
			MONport.print("Ethernet.gatewayIP : ");
			MONport.println(Ethernet.gatewayIP());
			MONport.print("DNS Server's IP Address: ");
			MONport.println(Ethernet.dnsServerIP());
			MONport.print("Network's Subnet Mask: ");
			MONport.println(Ethernet.subnetMask());
			MONport.println();
		#endif
	#endif
	delay(1000);
	#ifdef MQTT
		#ifdef DEBUG
			MONport.println("main.ino --> SETUP() - checkpoint:  MQTT connected");
		#endif
		//MQTTclient.setCallback(MQTT_callback); 	// define mqtt_incomming if necessary
		MQTT_connect();				// connect to broker
		//Sub_had_reset(MQTTclient);			// debug on MONport.
	#endif
	Sub_had_reset();			// debug on MONport.
						//	we had a reset
						//	and send info via mqtt
	delay(2000);

	#ifdef DS1820
		DS1820_deviceCount = DS1820_sensors.getDeviceCount();
		Sub_DS1820_allsensors_read();		// Alle DS1820, die erreichbar sind

		/* comment if call is in loop()  */
		//#ifdef MQTT
		//	Sub_DS1820_all_send_MQTT(DS1820_deviceCount);
		//#endif
	#endif
}
void loop(){
	static byte zaehler_sec=0;
	static byte zaehler_10sec=0;
	static long millis_old=0;

	#if defined(ESP32) || defined(ESP8266)
		if (WiFi.status() != WL_CONNECTED) {
			wifi_connect();
			/* It needs to run first to initiate the connection.
			* Same function works for checking the connection!
			*/
		}
	#endif
	#ifdef MQTT
		if (!MQTTclient.connected()) {
			MQTT_connect();
		}
	#endif

	if (millis() - millis_old >= 1000) {
		millis_old = millis();
		zaehler_sec++;
			#ifdef DEBUG
				//MONport.print(F("\n\n#################secImpuls="));
				//MONport.print(zaehler_sec);
				//MONport.print(F("    Millis()="));
				//MONport.print(millis_old);
				//MONport.print(F("    zaehler_10sec="));
				//MONport.print(zaehler_10sec);
				//MONport.print(F("    eigene_VAR="));
				//MONport.println(eigene_VAR);
			#endif
		switch (zaehler_sec) {
			case 1:
				#ifdef DEBUG
					MONport.print(F("checkpoint LOOP(): zaehler_sec "));
					MONport.println(zaehler_sec);
				#endif
				break;
			case 2:
				#ifdef DEBUG
					MONport.print(F("checkpoint LOOP(): zaehler_sec "));
					MONport.println(zaehler_sec);
				#endif
				break;
			case 3:
				#ifdef DS1820
					// Alle DS1820, die erreichbar sind
					// Kompletter 1Wire Bus wird abgefragt
					Sub_DS1820_allsensors_read();
				#endif
				#ifdef DEBUG
					MONport.print(F("checkpoint LOOP(): zaehler_sec "));
					MONport.println(zaehler_sec);
				#endif
				break;
			case 4:
				#ifdef DEBUG
					MONport.print(F("checkpoint LOOP(): zaehler_sec "));
					MONport.println(zaehler_sec);
				#endif
				break;
			case 5:
				#if (defined(DS1820) && defined(MQTT))
					Sub_DS1820_all_send_MQTT(DS1820_deviceCount);
				#endif
				#ifdef DEBUG
					MONport.print(F("checkpoint LOOP(): zaehler_sec "));
					MONport.println(zaehler_sec);
				#endif
				break;
			case 6:
				#ifdef DEBUG
					MONport.print(F("checkpoint LOOP(): zaehler_sec "));
					MONport.println(zaehler_sec);
				#endif
				break;
			case 7:
				#ifdef DEBUG
					MONport.print(F("checkpoint LOOP(): zaehler_sec "));
					MONport.println(zaehler_sec);
				#endif
				break;
			case 8:
				#ifdef DEBUG
					MONport.print(F("checkpoint LOOP(): zaehler_sec "));
					MONport.println(zaehler_sec);
				#endif
				break;
			case 9:
				#ifdef DEBUG
					MONport.print(F("checkpoint LOOP(): zaehler_sec "));
					MONport.println(zaehler_sec);
				#endif
				break;
			case 10:
				#ifdef DEBUG
					MONport.println("");
					MONport.println("10 sec Impuls");
					#ifdef MQTT
						MONport.println("10 sec Impuls-> MQTT");
						strcpy(mqtt_publish_topic, hostname);
						strcat(mqtt_publish_topic, "/info/debug");
						MQTTclient.publish(mqtt_publish_topic, "10secImp");
					#endif
				#endif
				zaehler_sec=0;
				zaehler_10sec++;
				#ifdef DEBUG
					MONport.print(F("checkpoint LOOP(): zaehler_sec "));
					MONport.print(zaehler_sec);
					MONport.print(F("     add zaehler_10sec "));
					MONport.println(zaehler_10sec);
				#endif
				break;
		}	// 10 Sekunden ENDE
		if (zaehler_10sec >= 3) {		// Nach 30 Sekunden vergangen
			zaehler_10sec=0;
			#ifdef DEBUG
				MONport.println("");
				MONport.println("30 sec Impuls");
			#endif
		}
	}
	#ifdef MQTT
		MQTTclient.loop();
	#endif
}
