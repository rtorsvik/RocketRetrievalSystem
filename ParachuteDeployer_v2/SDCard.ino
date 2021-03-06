/*
	SD-Card
*/



#if SDCARD
//Global instances
//_____________________________________________________________________________________________________________________________________________
File						logFile;			//log file for logging sensor data



//Global variables
//_____________________________________________________________________________________________________________________________________________
String logString = "";							//string assembly for data to log file
char logFile_name[] = "log000.csv";				//char array to create filename

char SEPARATOR = ';';



//Initialize SD-Card and generate new log file
//_____________________________________________________________________________________________________________________________________________
void initSDCARD()
{
	#if DEBUG
	Serial.print("SD-Card...\t");
	#endif

	//if the SD-Card is successfully initialized
	if (SD.begin(pin_SD_CS))
	{
		#if DEBUG
		Serial.println("initialized!\n\tCreating new log file...");
		#endif

		createNewNumberedFile();
	}

	//else, set an error message
	else
	{
		e |= 1 << 1;		//set bit nr 1, Error on SD card

		#if DEBUG
		Serial.println("failed to initialize");
		#endif
	}

}



//Create a numbered csv log file with for logging data
//program searches through SD-card to find next available file name, 
//i.e. if "log000.csv", "log001.csv" and "log002.csv" already exist, create a new file called "log003.csv"
//_____________________________________________________________________________________________________________________________________________
void createNewNumberedFile()
{

	for (uint8_t cnt = 0; cnt < 1000; cnt++)
	{
		logFile_name[3] = (cnt / 100) % 10 + '0';
		logFile_name[4] = (cnt /  10) % 10 + '0';
		logFile_name[5] = (cnt /   1) % 10 + '0';

		if (!SD.exists(logFile_name))		//create new file only if current filename doesn't already exist
		{			
			//add description for data
			/*
			logString = "";
			logString += "Explaination of tags: ";
			logString += "ms = time since program start [ms], ";
			logString += "dt = program loop-time [ms], ";
			logString += "e = error message, ";
			logString += "alt = altitude [m], ";
			logString += "altR = raw altitude [m], ";
			logString += "tempR = raw temperature [*C], ";
			logString += "aX, aY and aZ = acceleration in x(up), y and z [G], ";
			logString += "para = parachute deployed";
			
			SDWrite(logString);
			*/

			//create header for data
			logString = "";
			logString += "ms;e;alt;altR;tempR";

			#if ACCEL
			logString += ";aX;aY;aZ";
			#endif

			logString += ";para";

			SDWrite(logString);
			

			#if DEBUG
			Serial.print("\tNew logfile ");
			Serial.print(logFile_name);
			Serial.println(" created");
			#endif

			break;

		}

	}

	#if DEBUG
	if (!logFile) Serial.print("\tCould not create log file");
	#endif

	

	//*/

}



//log current data to log file
//_____________________________________________________________________________________________________________________________________________
void logData()
{
	
	

	//create string to write to log file
	logString = "";
	logString += ms;
	logString += SEPARATOR;

	logString += e;
	logString += SEPARATOR;
		
	logString += altitude;
	logString += SEPARATOR;
	logString += altitudeRaw;
	logString += SEPARATOR;
	logString += temperatureRaw;
	logString += SEPARATOR;

	#if ACCEL
	logString += acceleration.x;
	logString += SEPARATOR;
	logString += acceleration.y;
	logString += SEPARATOR;
	logString += acceleration.z;
	logString += SEPARATOR;
	#endif

	logString += deployParachute;
	

	//if file is open, write to it
	if (SDWrite(logString))
	{
		e &= ~(1 << 1);		//reset bit nr 0, Error on SD card, if the error is resolved
	}
	//else, there was an error opening the file, set an error message
	else
	{
		e |= 1 << 1;		//set bit nr 0, Error on SD card

	}

}


bool SDWrite(String s)
{
	//open file
	logFile = SD.open(logFile_name, FILE_WRITE);

	//if file is open, write to it
	if (logFile){
		logFile.println(s);
		logFile.close();

		return true;
	}

	return false;
}



#endif
