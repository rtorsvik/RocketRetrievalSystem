/*
	Handle everything that has to do with the status LED

	Green LED indikates all systems are go
	Blue LED indikates the that the parachute is deployed
	Blinking red LED indicates error, read as follows:

		Number of blinks corresponds to the error message,
		the error message consists of 4 bits, each bit corresponding to a sertain error.
		Blinks comes in pairs to make it easyer to count.

		bit nr,	error
		0		altimeter is disconnected	(meaning the parachute might not deploy)
		1		SD card is disconnected		(the acquired data wil not be saved)
		2
		3

		Example:
		Blink looks as follows (where * =blink, _ =pause):

		*_*___*______________*_*___*______________*_*___*______________*_*___*______________

		How to read:
		The light blinks 3 times before pausing. This gives us a binary value 0011,
		Both bit 0 and 1 is 1, meaning both the altimeter and SD card is disconnected

*/





//Global constants
//_____________________________________________________________________________________________________________________________________________
int8_t led_r_intensity = 255;	//intensity of red LED
int8_t led_g_intensity = 255;	//intensity of green LED
int8_t led_b_intensity = 255;	//intensity of blue LED



//Global variables
//_____________________________________________________________________________________________________________________________________________
long ms_next_blink = 0;			//the next time to turn led on or of
long blink_length = 100;		//length of each blink
long blink_delay = 550;			//delay between pair of blinks
long blink_interval = 2000;		//delay between each blink sequence

boolean blink_on = false;		//when true, blinking led is on
int blink_count = 0;			//number of times to blink





void updateStatusLED()
{
	//error light blink sequence
	if (ms > ms_next_blink)
	{
		blink_on = !blink_on;

		int t;
		if (!blink_on)
		{
			blink_count--;

			if (blink_count <= 0)
			{
				t = blink_interval;
				blink_count = e;
			}
			else if (e % 2 == 0 && blink_count % 2 == 0) t = blink_delay;
			else if (e % 2 == 1 && blink_count % 2 == 1) t = blink_delay;
			else t = blink_length;

		}
		else
		{
			t = blink_length;
		}

		ms_next_blink = ms + t;

	}
	


	//turn on/off status LED
	if (e > 0 && blink_on)
	{
		led_r_on();
		led_g_off();
		led_b_off();
	}
	else if (deployParachute)
	{
		led_r_off();
		led_g_off();
		led_b_on();
	}
	else if (e == 0)
	{
		led_r_off();
		led_g_on();
		led_b_off();
	}
	else
	{
		led_r_off();
		led_g_off();
		led_b_off();
	}

}




//turn red LED on/off
//_____________________________________________________________________________________________________________________________________________
void led_r_on()
{
	digitalWrite(pin_led_red, led_r_intensity);
}

void led_r_off()
{
	digitalWrite(pin_led_red, 0);
}



//turn green LED on/off
//_____________________________________________________________________________________________________________________________________________
void led_g_on()
{
	digitalWrite(pin_led_green, led_g_intensity);
}

void led_g_off()
{
	digitalWrite(pin_led_green, 0);
}



//turn blue status LED on/off
//_____________________________________________________________________________________________________________________________________________
void led_b_on()
{
	digitalWrite(pin_led_blue, led_b_intensity);
}



void led_b_off()
{
	digitalWrite(pin_led_blue, 0);
}


