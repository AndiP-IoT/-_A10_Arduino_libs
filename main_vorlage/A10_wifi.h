void wifi_connect(){
	int wifi_conn_retries=0;
	#if defined(DEBUG) || defined(DEBUG_WIFI)
		Serial.print("extSUB->A10_wifi:wifi_connect START");
	#endif
    while (WiFi.status() != WL_CONNECTED) {
        wifi_conn_retries++;
        if (wifi_conn_retries > 20) {
			Serial.print("   -> wifi_connect NO>20");
            break;
        }
        delay(500);
        Serial.print("#");
    }
    #if defined(DEBUG) || defined(DEBUG_WIFI)
		Serial.println("-> wifi_connect END");
	#endif
}



void wifi_setup(){
	#if defined(DEBUG) || defined(DEBUG_WIFI)
		Serial.println("extSUB->A10_wifi:wifi_setup START");
    #endif
    byte wifi_conn_retries = 0;

    /*	https://github.com/esp8266/Arduino/issues/
     * 2235#issuecomment-248916617
     */
    WiFi.disconnect();
    delay(100);

    // We start by connecting to a WiFi network
    WiFi.hostname(hostname);
    WiFi.config(ip, gw, sn, gw);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    WiFi.persistent(true);
    //WiFi.setAutoConnect(true);
    //WiFi.setAutoReconnect(true);
    while (WiFi.status() != WL_CONNECTED) {
        wifi_conn_retries++;
        if (wifi_conn_retries > 20) {
            break;
        }
        delay(500);
        Serial.print("#");
    }
	wifi_connect();
    randomSeed(micros());
    //Serial.println("");
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("   -> WiFi connected");
        Serial.println("   -> IP address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("   -> OhOH WIFI not conected");
    }
	Serial.println("-> wifi_setup END\n");
}

