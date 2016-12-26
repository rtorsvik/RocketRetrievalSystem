/*
	Servo
	Servo used to
*/





//Global instances
//_____________________________________________________________________________________________________________________________________________
Servo			servo;					//parachute deployment servo



//Global constants
//_____________________________________________________________________________________________________________________________________________
const uint8_t	servo_open = 10;		//servo possition to open hatch and deploy parachute
const uint8_t	servo_closed = 90;		//servo porrition when hacth is closed



//Global variables
//_____________________________________________________________________________________________________________________________________________
boolean open = false;
long ms_close_hatch;





//Initialize servo motor
//_____________________________________________________________________________________________________________________________________________
void initServo()
{
	servo.attach(pin_servo);
}



//Control hatch
//Check if hatch should open, and contol how it opens
//function returns true if hatch is open
//_____________________________________________________________________________________________________________________________________________
boolean hatchControl()
{
	if (open)							//if parachute should deploy, release hatch lock for 1000ms
	{
		ms_close_hatch = ms + 1000;
		open = false;
	}

	if (ms > ms_close_hatch)			//if the 1000ms has run out, detach servo (close hatch lock)
	{
		servo.detach();
		return false;
	}
	else
	{
		servo.attach(pin_servo);
		servo.write(servo_open);
		return true;
	}
}



//Open hatch
//_____________________________________________________________________________________________________________________________________________
void openHatch()
{
	open = true;
}



