/*

*/

//Initialize GPIO pins
//_____________________________________________________________________________________________________________________________________________
void initPins()
{
	pinMode(pin_led_red, OUTPUT);
	pinMode(pin_led_green, OUTPUT);
	pinMode(pin_led_blue, OUTPUT);
	pinMode(pin_button_zero, OUTPUT);
}

//Initialize servo motor
//_____________________________________________________________________________________________________________________________________________
void initServo()
{

}

//Initialize the sensors
//_____________________________________________________________________________________________________________________________________________
void initSensors()
{

	if (!bmp.begin())
	{
		/* There was a problem detecting the BMP180 ... check your connections */
		Serial.println("Ooops, no BMP180 detected ... Check your wiring!");
		while (1);
	}

	/*
	if (!mag.begin())
	{
	// There was a problem detecting the LSM303(magnetometer) ... check your connections
	Serial.println("Ooops, no LSM303(magnetometer) detected ... Check your wiring!");
	while (1);
	}

	if (!accel.begin())
	{
	// There was a problem detecting the LSM303(accelerometer) ... check your connections
	Serial.println(F("Ooops, no LSM303(accelerometer) detected ... Check your wiring!"));
	while (1);
	}
	*/
}


