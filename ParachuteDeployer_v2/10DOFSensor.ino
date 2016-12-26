/*
	Adafruit 10DOF sensor

	Altimeter (barometer) and temperature sensor
	Acceleration and orientation sesnor
	Magnetometer (compass)
	
*/





//Global instances
//_____________________________________________________________________________________________________________________________________________
Adafruit_10DOF					dof = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified	accel = Adafruit_LSM303_Accel_Unified(30301);
//Adafruit_LSM303_Mag_Unified   mag = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified			bmp = Adafruit_BMP085_Unified(18001);



//Global constants
//_____________________________________________________________________________________________________________________________________________
const float		seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;





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
		e |= 1 << 2;		//set bit nr 2, Accelerometer or magnetometer error

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
	#if DEBUG
	Serial.print("Magnetometer...\t");
	#endif

	if (mag.begin())
	{
		#if DEBUG
		Serial.println("initialized!");
		#endif
	}

	//else, set an error message
	else
	{
		e |= 1 << 2;		//set bit nr 2, Accelerometer or magnetometer error

		#if DEBUG
		Serial.println("failed to initialize");
		#endif
	}
	*/
}



void getSensorData()
{
	//sensors_event_t accel_event;
	//sensors_event_t mag_event;
	//sensors_vec_t   orientation;

	/*

	//Calculate pitch and roll from the raw accelerometer data
	accel.getEvent(&accel_event);
	if (dof.accelGetOrientation(&accel_event, &orientation))
	{
	//'orientation' should have valid .roll and .pitch fields
	Serial.print(F("Roll: "));
	Serial.print(orientation.roll);
	Serial.print(F("; "));
	Serial.print(F("Pitch: "));
	Serial.print(orientation.pitch);
	Serial.print(F("; "));
	}

	//Calculate the heading using the magnetometer
	mag.getEvent(&mag_event);
	if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
	{
	//'orientation' should have valid .heading data now
	Serial.print(F("Heading: "));
	Serial.print(orientation.heading);
	Serial.print(F("; "));
	}
	*/

	/*
	//Barometer 
	//_________________________________________________________________________________________________________________________________________
	sensors_event_t bmp_event;

	bmp.getEvent(&bmp_event);
	if (bmp_event.pressure)
	{
		//Get ambient temperature in *C
		bmp.getTemperature(&temperatureRaw);


		altitudeRaw = bmp.pressureToAltitude(seaLevelPressure, bmp_event.pressure, temperatureRaw);		


	}
	*/

}



//Get altitude from atimeter (barometer)
//_________________________________________________________________________________________________________________________________________
float getAltitude()
{
	sensors_event_t bmp_event;
	float altitude;

	bmp.getEvent(&bmp_event);
	if (bmp_event.pressure)
	{
		//Get ambient temperature in *C
		bmp.getTemperature(&temperatureRaw);

		//Get altitude raw value
		altitude = bmp.pressureToAltitude(seaLevelPressure, bmp_event.pressure, temperatureRaw);
	}

	return altitude;
}



//Get acceleration from accelerometer
//_________________________________________________________________________________________________________________________________________
sensors_vec_t getAcceleration()
{
	sensors_event_t accel_event;
	sensors_vec_t   orientation;

	//Calculate pitch and roll from the raw accelerometer data
	accel.getEvent(&accel_event);
	if (dof.accelGetOrientation(&accel_event, &orientation))
		return orientation;
}