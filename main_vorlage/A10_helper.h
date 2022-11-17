
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
		MQTTclient.publish("Info/Reset", "Done" );
		delay(1000);
		MQTTclient.publish("Info/Reset", "OFF" );
	#endif
}

void Sub_printVersion(){
  Serial.println();
  Serial.println(MAINFILE);
  Serial.println();
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);
  Serial.print("Version=");
  Serial.println(VERSION);
  Serial.println();
  Serial.println();

}
