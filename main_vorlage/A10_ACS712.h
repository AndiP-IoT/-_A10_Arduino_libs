


float get_ACS712 (int channel){
	unsigned int x=0;
	float mV_pro_dig = 5000.0/1024.0;
	float offset = -0.00;
	float Nullpunkt_r=508;
	float Nullpunkt_V=2.427;
	float Span_mV=1000.00;
	float Span_mA=5000.00;
	float Sensitivity = 185; // mV/A
	float Span1A=0.185;
	float AcsValue_r=0.0;
	float Samples=0.0;
	float AvgAcs_r=0.0;
	float AcsValueF=0.0;
	
	
	for (int x = 0; x < 150; x++){ //Get 150 samples
		AcsValue_r = analogRead(channel);     //Read current sensor values   
		Samples = Samples + AcsValue_r;  //Add samples together
		delay (3); // let ADC settle before next sample 3ms
	}
	AvgAcs_r=Samples/150.0;//Taking Average of Samples
	Serial.print("Analog Raw = ");//Print the read current on Serial monitor
	Serial.print(AvgAcs_r);//Print the read current on Serial monitor
	
	
	//((AvgAcs * (5.0 / 1024.0)) is converting the read voltage in 0-5 volts
	//2.5 is offset(I assumed that arduino is working on 5v so the viout at no current comes
	//out to be 2.5 which is out offset. If your arduino is working on different voltage than 
	//you must change the offset according to the input voltage)
	//0.185v(185mV) is rise in output voltage when 1A current flows at input
	
	AcsValueF = (AvgAcs_r - Nullpunkt_r) *mV_pro_dig/Sensitivity + offset;
	//AcsValueF = (AvgAcs-Nullpunkt) * (Span / 512.0)) )/Span1A;
	
	Serial.print(" Current = ");//Print the read current on Serial monitor
	Serial.println(AcsValueF);//Print the read current on Serial monitor
	return AcsValueF;
}
