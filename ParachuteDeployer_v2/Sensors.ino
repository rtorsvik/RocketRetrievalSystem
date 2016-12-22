/*
	Aquiring and filtering of sansor data
*/



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

	//Barometer 
	//_________________________________________________________________________________________________________________________________________
	sensors_event_t bmp_event;

	bmp.getEvent(&bmp_event);
	if (bmp_event.pressure)
	{
		//Get ambient temperature in *C
		bmp.getTemperature(&temperatureRaw);

		//Calculate the altitude using the barometric pressure sensor
		//Convert atmospheric pressure, SLP and temp to altitude
		altitudeMean = 0;
		altitudeRaw = bmp.pressureToAltitude(seaLevelPressure,	bmp_event.pressure,	temperatureRaw);		
		for (int i = 9; i > 0; i--)
		{
			altitude[i] = altitude[i - 1];
			altitudeMean += altitude[i];
		}
		altitude[0] = altitudeRaw;
		altitudeMean += altitudeRaw;

		altitudeMean /= 10;


		if (digitalRead(pin_button_zero)) {
			altitudeOffset = altitudeMean;
		}

		altitudeMean -= altitudeOffset;

	}

}