/*
	Code for collecting data and for deploying parachute for experimental rocket.

	Author: Rein Åsmund Torsvik
	2016-01-23

	Updates
	2016-12-22
*/



//precompile options
//_____________________________________________________________________________________________________________________________________________
#define DEBUG true
#define VERSION 2.0



//Includes
//_____________________________________________________________________________________________________________________________________________
#include <Servo.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
//#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
//#include <Adafruit_L3GD20_U.h>
//#include <Adafruit_10DOF.h>



//GPOI pins
//_____________________________________________________________________________________________________________________________________________
#define pin_led_red 3
#define pin_led_green 5
#define pin_led_blue 6
#define pin_servo 10
#define pin_button_zero 22



//Global instances
//_____________________________________________________________________________________________________________________________________________
Servo servo; //deployment servo
//Adafruit_10DOF                dof = Adafruit_10DOF();
//Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
//Adafruit_LSM303_Mag_Unified   mag = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp = Adafruit_BMP085_Unified(18001);



//Global constants
//_____________________________________________________________________________________________________________________________________________
int8_t led_r_intensity = 255;
int8_t led_g_intensity = 255;
int8_t led_b_intensity = 255;


//Global variables
//_____________________________________________________________________________________________________________________________________________
float altitudeRaw;
float altitudeMean;
float altitude[10];
float altitudeOffset = 0;
float temperatureRaw;

float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;


//system state
boolean deployParachute = false;

int e = 0;					//error messages
//bit#, error message
//0, <spare>
//1, Altimeter error
//2, SD-card error





//Program values
//_____________________________________________________________________________________________________________________________________________
long runCount = 0; //counts number of program itterations
long ms = 0; //milliseconds holder
long ms_prev = 0;
long dt = 0; //loop time





//SETUP
//#############################################################################################################################################
//#############################################################################################################################################
void setup(void)
{

#if DEBUG
	Serial.begin(9600);
	Serial.print("Initializing parachute deployment and data acquisition module \nVersion: "); Serial.println(VERSION);
#endif

	//set the indikator LED red to indicate start of initialization
	digitalWrite(pin_led_red, 255);

	//Initialize GPIO pins
	initPins();

	//Initialise the sensors
	initAltimeter();

	//Initialize serv motor
	initServo();

}





//LOOP
//#############################################################################################################################################
//#############################################################################################################################################
void loop(void)
{
	//update time and loop time
	ms = millis();
	dt = ms - ms_prev;
	ms_prev = ms;

	




	//temporary code follows
	
	getSensorData();
	delay(100);




	//handle indikating of system status via status LED
	updateStatusLED();

	

	

	//end of loop
	runCount++;
}