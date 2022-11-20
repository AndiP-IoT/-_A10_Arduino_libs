void MQTT_connect(){
	static int reconnect_count=0;
	#if defined(DEBUG) || defined(DEBUG_MQTT)
		Serial.println("extSUB->A10_mqtt: MQTT_reconnect START");
		Serial.println("   -> Attempting MQTT_reconnect ");
    #endif
    //while(1){
//************ choose if mqqt WITH or WITHOUT user/pwd ****************
		//if (MQTTclient.connect(MQTT_clientId)) {
		if (MQTTclient.connect(MQTT_clientId, MQTT_userName, MQTT_pwd)) {
//*********************************************************************
			#if defined(DEBUG) || defined(DEBUG_MQTT)
				Serial.println("   -> MQTT connected");
			#endif
			//once connected to MQTT broker, subscribe command if any
			strcpy(mqtt_publish_topic, hostname);
			strcat(mqtt_publish_topic, "/cmd/#");
			MQTTclient.subscribe(mqtt_publish_topic);
			//break;
		} else {
			Serial.print("   -> failed, rc=");
			Serial.print(MQTTclient.state());
			#if defined(DEBUG) || defined(DEBUG_MQTT)
				Serial.print("   -> try again in 3 seconds");
			#endif
			// Wait 3 seconds before retrying
			reconnect_count++;
			if (reconnect_count>=10){
				LAN_WIFI_reconnect();
			}
			delay(3000);
		}
	//}
	#if defined(DEBUG) || defined(DEBUG_MQTT)
		Serial.println("-> MQTT_reconnect END\n");
    	#endif
}

//######  ACHTUNG  diese Vorlage in Hautprogramm einkopieren!!!!!!  #########
//              und NAMEN ändern !!!!
void MQTT_callback_vorlage(char* topic, byte* payload, unsigned int length){
	#ifdef DEBUG
		Serial.println("extSUB mqtt_callback START\n");
		Serial.print("##########   sub_callback ----> Message arrived [");
		Serial.print(topic);
		Serial.print("] ");
		for (int i = 0; i < length; i++) {
			Serial.print((char)payload[i]);
		}
		Serial.println("");
	#endif
	/* cmd 	(Befehl wird von Broker an uC geschickt)
	* stat (Status einer Werte-Veriable
	* info (Status der Programmabfolge, der Programmablaufs)
	* DS1820 (alle angeschlossenen DS1820)
	* DI (Digital Inputs)
	*/

	//##### Das ist ein Beispiel für RESET ########
	strcpy(mqtt_publish_topic, hostname);
	strcat(mqtt_publish_topic, "/cmd/RESET");
	if (strcmp(topic,mqtt_publish_topic) == 0) {
		payload[length] = '\0';
		//###### Integerwert
		int test_check_reset = atoi((char *)payload);
		//###### Textwert
		if (strcmp((char *)payload, "ON") == 0) {
		} else {
		}

		#ifdef DEBUG
			Serial.println("mqtt_callback ----> RESET=");
			Serial.println(test_check_reset);
		#endif
    }
	// Note: Do not use the client in the callback to publish, subscribe or
	// unsubscribe as it may cause deadlocks when other things arrive while
	// sending and receiving acknowledgments. Instead, change a global variable,
	// or push to a queue and handle it in the loop after calling `client.loop()`.
	/* ************************************* Sendoptions
	 *  NEVER, NEVER use # in topic !!!!!!!
		*****   Strings ***********
		strcpy(mqtt_publish_topic, "Heiz/DS1820/");
		strcat(mqtt_publish_topic, DS1820_name[i]);
		dtostrf(DS1820_value[i], 4, 1, mqtt_publish_msg);
		MQTTclient.publish(topic, mqtt_publish_msg);

		****    Text **************
		MQTTclient.publish("WF/info/impuls", "10sec");

		****    Integer ***********
		itoa(Kessel_Servo_soll, mqtt_publish_msg, 10);
		MQTTclient.publish("Ofen/stat/Servo", mqtt_publish_msg );

		****    Byte **************
		byte ofen_pumpe=digitalRead(PUMP_PIN);
		dtostrf(ofen_pumpe, 1, 0, mqtt_publish_msg);
		MQTTclient.publish("Ofen/stat/Kesselpumpe", mqtt_publish_msg);
	*/
}
