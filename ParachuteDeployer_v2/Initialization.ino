/*
	Initialization of different modules
*/

//Initialize GPIO pins
//_____________________________________________________________________________________________________________________________________________
void initPins()
{
	pinMode(pin_led_red, OUTPUT);
	pinMode(pin_led_green, OUTPUT);
	pinMode(pin_led_blue, OUTPUT);
	pinMode(pin_servo, OUTPUT);
}



//Initialize servo motor
//_____________________________________________________________________________________________________________________________________________
void initServo()
{
	servo.attach(pin_servo);
}



//Initialize the altimeter (barometer)
//_____________________________________________________________________________________________________________________________________________
void initAltimeter()
{
	#if DEBUG
	Serial.print("Altimeter...\t");
	#endif

	//if the altimeter is successfully initialized
	if (bmp.begin())
	{
		#if DEBUG
		Serial.println("initialized!");
		#endif
	}

	//else, set an error message
	else
	{
		e |= 1 << 0;		//set bit nr 0, altimeter error

		#if DEBUG
		Serial.println("failed to initialize");
		#endif
	}
}



//Initialize the accelerometer
//_____________________________________________________________________________________________________________________________________________
void initAccelerometer()
{
	#if DEBUG
	Serial.print("Accelerometer...\t");
	#endif
	
	if (accel.begin())
	{
		#if DEBUG
		Serial.println("initialized!");
		#endif
	}

	//else, set an error message
	else
	{
		e |= 1 << 2;		//set bit nr 2, accelerometer error

		#if DEBUG
		Serial.println("failed to initialize");
		#endif
	}
}



//Initialize the magnetometer (compass)
//_____________________________________________________________________________________________________________________________________________
void initMagnetometer()
{
	/*
	if (!mag.begin())
	{
	// There was a problem detecting the LSM303(magnetometer) ... check your connections
	Serial.println("Ooops, no LSM303(magnetometer) detected ... Check your wiring!");
	while (1);
	}
	*/
}



//Initialize the SD-card and log files
//_____________________________________________________________________________________________________________________________________________
void initSDCard()
{

}


