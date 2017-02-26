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
#define DEBUG	false		//enable/disable debug mode
#define SDCARD	true		//include SD-card to sketch
#define ACCEL	false		//include accelerometer to sketch
#define MAGN	false		//include magnetometer to sketch



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
#define altitudeDropThreshold 3			//how many meters of drop below the max height before the parachute deploys

#define filterConstant 0.95				//filter constant for the filtering of sensor values (value between 0 and 1)



//Global variables
//_____________________________________________________________________________________________________________________________________________
float	altitudeRaw;					//raw altidude value from altimeter
float	altitudeRaw_prev;
float	altitude;						//current altitude (filtered value)
int		altitude_ground;				//altitude at ground level (where the rocket was initialized)
float	altitude_max;					//the maximum altitude the rocket has reached

float	temperatureRaw;					//raw temperature value
//float	temperatureRaw_prev;

#if ACCEL
sensors_vec_t   acceleration;
//sensors_vec_t   acceleration_prev;
#endif

#if MAGN
sensors_vec_t   orientation;
//sensors_vec_t   orientation_prev;
#endif

boolean deployParachute = false;		//is set to true when system should deploy parashute



//System variables
//_____________________________________________________________________________________________________________________________________________
#if DEBUG
long	runCount = 0;					//counts number of program itterations
long	dt = 0;							//loop time
#endif
long	ms = 0;							//current time [ms]
long	ms_prev = 0;					//time in previous loop, used to calculate loop time


byte	e = 0;							//error messages
											//bit nr, error
											//0		altimeter is disconnected(meaning the parachute might not deploy)
											//1		SD card is disconnected(the acquired data wil not be saved)
											//2		accelerometer is disconnected
											//3

byte	e_cnt_0 = 0;					//counts how many times in a row the alltitude value from the sensor has been the same.
//byte		e_cnt_1 = 0;

#if ACCEL
byte	e_cnt_2 = 0;
#endif

//byte		e_cnt_3 = 0;




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

	#if ACCEL
	//Initialize accelerometer
	initAccelerometer();
	#endif

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
	#if DEBUG
	dt = ms - ms_prev;
	#endif
	ms_prev = ms;

	
	#if ACCEL
	//Acceleration
	acceleration = getAcceleration();
	#endif


	#if MAGN
	//Magnetometer (heading)
	getMag();
	#endif

	
	//Altitude
	//get the altitude from the altimeter (barometer) and filter the raw value
	//10 seconds startup, set the current altitude as the grount level
	altitudeRaw = getAltitude();											//get altitude from altimeter (barometer)
	altitudeRaw = altitudeRaw - altitude_ground;							//subtract ground level from raw altitude value

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
	float _1g = 9.81;
	if (runCount % 16 == 0)
	{
		Serial.println("\nSystem data:");
		Serial.print("\trun time    [ms]:\t"); Serial.println(ms);
		Serial.print("\tloop time   [ms]:\t"); Serial.println(dt);
		Serial.print("\terror:           \t"); Serial.println(e);
		Serial.println();
		Serial.print("\taltitude     [m]:\t"); Serial.println(altitude);
		Serial.print("\taltitude max [m]:\t"); Serial.println(altitude_max);
		Serial.println();
		Serial.print("\ttemperature [*C]:\t"); Serial.println(temperatureRaw);

		#if ACCEL
		Serial.println();
		Serial.print("\taccel x (up) [G]:\t"); Serial.println(acceleration.x / _1g);
		Serial.print("\taccel y      [G]:\t"); Serial.println(acceleration.y / _1g);
		Serial.print("\taccel z      [G]:\t"); Serial.println(acceleration.z / _1g);
		Serial.print("\tstatus:          \t"); Serial.println(acceleration.status);
		#endif

		Serial.println();
		deployParachute ? Serial.println("\tparachute:\t\tdeployed") : Serial.println("\tparachute:\t\tready");
		hatchControl() ? Serial.println("\thatch:\t\t\topened") : Serial.println("\thatch:\t\t\tclosed");
	}
	runCount++;
	#endif


	
	#if SDCARD
	//log data
	logData();
	#endif

	//handle indikating of system status via status LED
	updateStatusLED();

	//end of loop
	
}