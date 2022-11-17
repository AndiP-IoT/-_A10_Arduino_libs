# A10_Arduino_libs

MAIN VORLAGE

######    A10_DS1820.h  ###########################################
	void Sub_DS1820_printAddress
		Umsetzung der Deviceadresse in Lesbare HEX-Code
		mit DEBUG / DEBUG_DS1820 info
	void Sub_DS1820_print_array
		Ausgabe des DS1820 Arrays
		mit DEBUG / DEBUG_DS1820 info
	void Sub_DS1820_allsensors_read
		Abfrage alles DS1820
		max_DS1820_count darf nicht überschritten werden - sonst Speicheroverrun
		alles in Array ablegen
		mit DEBUG / DEBUG_DS1820 info

	#ifdef MQTT	void Sub_DS1820_all_send_MQTT
		Alle Sensoren in Array werden via definiertem MQTT übertrgen
		mit DEBUG / DEBUG_DS1820 info
####################################################################


#####     A10_helper.h  ############################################
	void Sub_blink_testLED
		Arbeitet mit delay
	void Sub_had_reset
		Ausgabe über Serial.print
		und, wenn #define MQTT auch via MQTT
	void Sub_printVersion
		Ausgabe Filename / Datum / Version
####################################################################




#####     A10_mqtt.h  ###########################################
	MQTT_connect()
		(re)connect mit subscribe
		nach x ergebnislosen Wiederholungen zusätzlich LAN_WIFI_reconnect()
		mit DEBUG / DEBUG_MQTT info
####################################################################


#####     A10_wifi.h  ###########################################
	wifi_connect()
		wartet bis connected (20x500msec)
		wenn nach 10 Sekunden nichts, mach weiter im Hauptcode
		mit DEBUG / DEBUG_WIFI info
	wifi_setup()
		die notwendigen Einstellungen für WIFI
		mit STATUS Ausgabe
		mit DEBUG / DEBUG_WIFI info
####################################################################


#####     main_vorlage.ino  ###########################################
	DEFINITION verschiedener uC-BOARDS
	Grundgerüst mit LAN (W5500), WIFI (ESP), MQTT, DS1820
	Sekundentakt 1-10
	10er Sekunden 1-3
	DABUG Varianten via Serial.print


####################################################################
