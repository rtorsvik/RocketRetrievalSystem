/*
	Code for collecting data and for deploying parachute for experimental rocket.

	Author: Rein Åsmund Torsvik
	2016-01-23

	Updates:
	2016-12-22
	2016-12-25	ver. 2.1	Added control of servo and temperature meassurement
	2016-12-26				Added functions for saving data to SD-card, not tested yet
*/





//precompile options
//_____________________________________________________________________________________________________________________________________________
#define VERSION 2.1
#define DEBUG true
#define SDCARD false



//Includes
//_____________________________________________________________________________________________________________________________________________
#include <Servo.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>



//GPOI pins
//_____________________________________________________________________________________________________________________________________________
#define pin_led_red 6
#define pin_led_green 5
#define pin_led_blue 9
#define pin_servo 10
#define pin_SD_CS 7





//Global constants
//_____________________________________________________________________________________________________________________________________________
const int		altitudeDropThreshold = 2;				//how many meters of drop below the max height before the parachute deploys

const float		filterConstant = 0.95;					//filter constant for the filtering of sensor values (value between 0 and 1)



//Global variables
//_____________________________________________________________________________________________________________________________________________
float	altitudeRaw;					//raw altidude value from altimeter
float	altitude;						//current altitude (filtered value)
int		altitude_ground;				//altitude at ground level (where the rocket was initialized)
float	altitude_max;					//the maximum altitude the rocket has reached

float	temperatureRaw;					//raw temperature value

sensors_vec_t   orientation;

boolean deployParachute = false;		//is set to true when system should deploy parashute



//System variables
//_____________________________________________________________________________________________________________________________________________
long	runCount = 0;					//counts number of program itterations
long	ms = 0;							//current time [ms]
long	ms_prev = 0;					//time in previous loop, used to calculate loop time
long	dt = 0;							//loop time

int		e = 0;							//error messages
											//bit nr, error
											//0		altimeter is disconnected(meaning the parachute might not deploy)
											//1		SD card is disconnected(the acquired data wil not be saved)
											//2		accelerometer is disconnected
											//3





//SETUP
//#############################################################################################################################################
//#############################################################################################################################################
void setup(void)
{

#if DEBUG
	delay(1000);
	Serial.begin(9600);
	Serial.print("\nInitializing parachute deployment and data acquisition module \nVersion: "); Serial.println(VERSION);
	Serial.println();
#endif

	//Initialize GPIO pins
	initPins();

	//set the indikator LED red to indicate start of initialization
	led_r_on();

#if SDCARD
	//Initialize SD-Card and generate new log file
	initSDCARD();
#endif

	//Initialize serv motor
	//initServo();							//temporerily removed, attach servo when deploying parachute instead

	//Initialize accelerometer
	//initAccelerometer();

	//Initialize magnetometer
	//initMagnetometer();

	//Initialize the altimeter (barometer) and find ground level before exiting initialization
	initAltimeter();

	#if DEBUG
	Serial.print("\tSetting current altitude as ground level (takes about 10 seconds)\n\t");
	int t = 1000;
	#endif

	altitude = getAltitude();				//set initial altitude value
	while (millis() < 10000)
	{
		#if DEBUG
		if (millis() > t)					//every 1 secont, print "." to show progress
		{
			Serial.print(".");
			t += 1000;
		}
		#endif

		altitudeRaw = getAltitude();		//get altitude from altimeter (barometer)
		altitude = filterConstant*altitude + (1 - filterConstant)*altitudeRaw;	//filter raw altitude values with FIR filter			
	}

	#if DEBUG
	Serial.print(" ground level set to: "); Serial.println(altitude);
	#endif

	altitude_ground = altitude;				//current altitude is set as the ground level
	altitude = altitude - altitude_ground;	//reset altitude to ground leven as new initial altitude value



	//end of initializatino
	//if all systems are go, green status LED should light up.
	#if DEBUG
	Serial.println("\nFinished initialization\n\n");
	delay(3000);
	#endif

}





//LOOP
//#############################################################################################################################################
//#############################################################################################################################################
void loop(void)
{
	//System
	//update time and loop time
	ms = millis();
	dt = ms - ms_prev;
	ms_prev = ms;

	

	//Acceleration
	//TODO



	//Magnetometer (heading)
	//TODO


	
	//Altitude
	//get the altitude from the altimeter (barometer) and filter the raw value
	//10 seconds startup, set the current altitude as the grount level
	altitudeRaw = getAltitude();							//get altitude from altimeter (barometer)
	altitudeRaw = altitudeRaw - altitude_ground;			//subtract ground level from raw altitude value

	altitude = filterConstant*altitude + (1 - filterConstant)*altitudeRaw;	//filter raw altitude values with FIR filter



	//Parachute deployment
	//Deploy parachute when rocket is decending (altitude begins to drop)
	//Latch that secures the parachute is released by the servo for 1000ms, alowing the hatch to open and the parachute to deploy
	//Hatch might allso be opened by some button, in the future, by having the button set the openHatch variable
	altitude_max = max(altitude, altitude_max);

	if (altitude < altitude_max - altitudeDropThreshold
		&& !deployParachute)								//if altitude drops beneth a sertain threshold, deploy parchute
	{
		deployParachute = true;
		openHatch();
	}

	//if (digitalRead(pin_button)
	//	&& !openHatch)
	//{
	//	openHatch();
	//}

	hatchControl();

		



	//print debug values
	#if DEBUG
	if (runCount % 16 == 0)
	{
		Serial.println("\nSystem data:");
		Serial.print("\trun time [ms]:\t\t"); Serial.println(ms);
		Serial.print("\tloop time [ms]:\t\t"); Serial.println(dt);
		Serial.print("\terror:\t\t\t"); Serial.println(e);
		Serial.println();
		Serial.print("\taltitude [m]:\t\t"); Serial.println(altitude);
		Serial.print("\taltitude max [m]:\t"); Serial.println(altitude_max);
		Serial.println();
		Serial.print("\ttemperature [*C]:\t"); Serial.println(temperatureRaw);
		Serial.println();
		deployParachute ? Serial.println("\tparachute:\t\tdeployed") : Serial.println("\thatch:\t\t\tready");
		hatchControl() ? Serial.println("\thatch:\t\t\topened") : Serial.println("\thatch:\t\t\tclosed");
	}
	#endif



	//handle indikating of system status via status LED
	updateStatusLED();

	//end of loop
	runCount++;
}