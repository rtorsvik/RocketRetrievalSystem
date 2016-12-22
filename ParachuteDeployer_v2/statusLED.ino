/*
	Handle everything that has to do with the status LED
*/


long ms_next_blink = 0;		//the next time to turn led on or of
int blink_length = 250;		//length of each blink
int blink_delay = 750;		//delay between pair of blinks
int blink_interval = 2000;	//delay between each blink sequence

boolean blink_on = false;	//when true, blinking led is on
int blink_count = 0;

boolean led_r = false;		//when true, status LED glows red
boolean	led_g = false;		//when true, status LED glows green
boolean led_b = false;		//when true, status LED glows blue



void updateStatusLED()
{
	/*
	//error light blink sequence
	
	if (ms > ms_next_blink)
	{
		blink_on != blink_on;

		int t;
		if (blink_on)
		{
			t = blink_length;
			blink_count--;
		}

		else
		{
			if (blink_count <= 0) t = blink_interval;
			else if (blink_count % 2 == 0) t = blink_length;
			else t = blink_delay;
		}

		ms_next_blink = ms + t;
	}
	*/
	


	//handle blink frequency
	if (ms > ms_next_blink)
	{
		if (!blink_on)
		{
			blink_on = true;
		}

		else
		{
			blink_on = false;
			blink_count--;
		}

		if (blink_count <= 0)
		{
			blink_count = e;
			ms_next_blink = ms + blink_interval;
		}
		else
		{
			ms_next_blink = ms + blink_length;
		}

	}
	



	//Handle red status LED
	if (e > 0 && blink_on)
	{
		led_r = true;
	}
	else
	{
		led_r = false;
	}

	//Hanlde green status LED
	if (e == 0)
	{
		led_g = true;
	}
	else
	{
		led_g = false;
	}

	//handle blue status LED
	if (deployParachute)
	{
		led_b = true;
	}
	else
	{
		led_b = false;
	}



	//turn status LED on and of (prioritize)
	if (led_r)
	{
		led_r_on();
		led_g_off();
		led_b_off();
	}
	else if (led_b)
	{
		led_r_off();
		led_g_off();
		led_b_on();
	}
	else if (led_g)
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




//turn red LED on
void led_r_on()
{
	digitalWrite(pin_led_red, led_r_intensity);
}

void led_r_off()
{
	digitalWrite(pin_led_red, 0);
}

//turn green LED on
void led_g_on()
{
	digitalWrite(pin_led_green, led_g_intensity);
}

void led_g_off()
{
	digitalWrite(pin_led_green, 0);
}

//turn blue status LED on
void led_b_on()
{
	digitalWrite(pin_led_blue, led_b_intensity);
}

void led_b_off()
{
	digitalWrite(pin_led_blue, 0);
}


