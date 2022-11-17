// function to print a device address
void Sub_DS1820_printAddress(DeviceAddress deviceAddress, char *temp2) {
	#if defined(DEBUG) || defined(DEBUG_DS1820)
		Serial.println("extSUB->A10_DS1820: Sub_DS1820_printAddress START\n");
	#endif
	if (OneWire::crc8(deviceAddress, 7) != deviceAddress[7]) {
		Serial.println("hat keinen gueltigen CRC!");
	} else {
		//alles ist ok, anzeigen
		char bufferx[3];
		for (uint8_t i = 0; i < 8; i++) {
			sprintf (bufferx, "%02X", deviceAddress[i]);
			strcat(temp2, bufferx);
		}
	}
	#if defined(DEBUG) || defined(DEBUG_DS1820)
		Serial.println("-> Sub_DS1820_printAddress END\n");
	#endif
}

void Sub_DS1820_print_array(){
	#if defined(DEBUG) || defined(DEBUG_DS1820)
		Serial.println("extSUB->A10_DS1820: Sub_DS1820_print_array START\n");
	#endif
	for (int i=0; i<DS1820_deviceCount;i++){
		Serial.print("Sensor #");
		Serial.print(i + 1);
		Serial.print(" : ");
		Serial.print("");
		Serial.print(DS1820_all_names[i]);
		Serial.print("		Value=");
		Serial.print(DS1820_all_values[i]);
		Serial.println(";");
		delay(150);
	}
	#if defined(DEBUG) || defined(DEBUG_DS1820)
		Serial.println("-> Sub_DS1820_print_array END\n");
	#endif
}

void Sub_DS1820_allsensors_read() {
	#if defined(DEBUG) || defined(DEBUG_DS1820)
		Serial.println("extSUB->A10_DS1820: Sub_DS1820_allsensors_read START\n");
	#endif
	DS1820_sensors.begin();
	DS1820_deviceCount = DS1820_sensors.getDeviceCount();
	if (max_DS1820_count<=DS1820_deviceCount){
			Serial.println("########## ACHTUNG ######### zu viele DS1820 -> 'max_DS1820_count' ändern!!");
	}
	#if ((defined(DEBUG) || defined(DEBUG_DS1820))   )
		Serial.print(F("checkpoint Sub_DS1820_allsensors_read(): DS1820_deviceCount="));
		Serial.println(DS1820_deviceCount);
	#else
		if (DS1820_deviceCount == 0) {
			Serial.println("No DS1820_sensors");
			return;
		}
	#endif
	// Send the command to get temperatures
	DS1820_sensors.requestTemperatures();
	delay(150);

	// We'll use this variable to store a found device address
	// define var from type DeviceAddress
	DeviceAddress tempAddress;
	for (int i = 0; i < DS1820_deviceCount;	i++) {
		DS1820_one_address[0] = '\0';
		DS1820_sensors.getAddress(tempAddress, i);

		Sub_DS1820_printAddress(tempAddress, DS1820_one_address);

		strcpy (DS1820_all_names[i], DS1820_one_address);
		DS1820_all_values[i] = DS1820_sensors.getTempC(tempAddress);
		delay(150);
	}
	Sub_DS1820_print_array();
}

#ifdef MQTT
	void Sub_DS1820_all_send_MQTT(byte deviceCount) {
	//deviceCount als Rückgabewert von Aufruf Sub_DS1820_allsensors_read
		#if defined(DEBUG) || defined(DEBUG_DS1820) || defined(DEBUG_MQTT)
			Serial.print("deviceCount=");
			Serial.println(deviceCount);
			Serial.println("-----------checkpoint:Sub_status_MQTT_DS1820  beginn");
		#endif
		//if (client.connect("arduinoClient", "openhabian", "affe123456")) {
			//Serial.println("Sub_status_MQTT_DS1820 connected");
			for (int i = 0; i < deviceCount;  i++) {
				//char topic[50];
				strcpy(mqtt_publish_topic, "Solar/DS1820/");
				strcat(mqtt_publish_topic, DS1820_all_names[i]);
				dtostrf(DS1820_all_values[i], 4, 1, mqtt_publish_msg);
				MQTTclient.publish(mqtt_publish_topic, mqtt_publish_msg);
				#if defined(DEBUG) || defined(DEBUG_DS1820) || defined(DEBUG_MQTT)
					Serial.print(mqtt_publish_topic);
					Serial.print(":");
					Serial.print(mqtt_publish_msg);
					Serial.print("---->");
					Serial.print(DS1820_all_names[i]);
					Serial.print("=");
					Serial.print(DS1820_all_values[i]);
					Serial.println();
				#endif
			}
		//}
		//Serial.println("-----------checkpoint:Sub_status_MQTT_DS1820 -----------------end");
	}
#endif




