
void Sub_blink_testLED(int verzoegerung, int pulsecount = 1,int TEST_LED=13){
    for (int i = 0; i < pulsecount; i++) {
        digitalWrite(TEST_LED, HIGH);   // Turn the LED on
        delay(verzoegerung);
        digitalWrite(TEST_LED, LOW);   // Turn the LED off
        delay(verzoegerung);
    }
}
//----------------------------------------------------

//void Sub_had_reset(PubSubClient &MQTTclient){
void Sub_had_reset(){
	Serial.println("extSUB->A10_mqtt: sub_had_reset:-----> RESET done");
	#ifdef MQTT
		strcpy(mqtt_publish_topic, hostname);
		strcat(mqtt_publish_topic, "/Info/Reset/");
				
		MQTTclient.publish(mqtt_publish_topic, "Done" );
		delay(1000);
		MQTTclient.publish(mqtt_publish_topic, "OFF" );
	#endif
}

void Sub_printVersion(){
  Serial.println();
  Serial.println(MAINFILE);
  Serial.println();
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.print(__TIME__);
  Serial.print("   Version=");
  Serial.print(VERSION);
  Serial.print("   GIT_VERSION=");
  Serial.println(GIT_VERSION);
  Serial.println();
  Serial.println();

}
