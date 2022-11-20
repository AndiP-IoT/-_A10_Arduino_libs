void LAN_setup(){
	Serial.println("extSUB->A10_w5500:LAN_setup START");
    Ethernet.begin(mac, ip);
	if (Ethernet.hardwareStatus() == EthernetNoHardware) {
		Serial.println(F("   Ethernet shield was not found."));
	}
	else if (Ethernet.hardwareStatus() == EthernetW5100) {
		Serial.println(F("   W52. Ethernet controller detected."));
	}
	else if (Ethernet.hardwareStatus() == EthernetW5200) {
		Serial.println(F("   W5200 Ethernet controller detected."));
	}
	else if (Ethernet.hardwareStatus() == EthernetW5500) {
		Serial.println(F("   W5500 Ethernet controller detected."));
	}
	Serial.print (F("   Ethernet.linkStatus : "));
	Serial.println(Ethernet.linkStatus());
	Serial.print (F("   Ethernet.localIP : "));
	Serial.println(Ethernet.localIP());
	Serial.print(F("   Ethernet.gatewayIP : "));
	Serial.println(Ethernet.gatewayIP());
	Serial.print(F("   DNS Server's IP Address: "));
	Serial.println(Ethernet.dnsServerIP());
	Serial.print(F("   Network's Subnet Mask: "));
	Serial.println(Ethernet.subnetMask());	
	Serial.println();
	Serial.println("   extSUB->A10_w5500:LAN_setup END\n");
	Serial.println();
}

