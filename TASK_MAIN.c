#include "EV3_FileIO.c"
#include "EV3Servo-lib-UW.c"

const int MAX_SIZE = 72;
const int POSITION_X[12] = {-340, -440, 0, -105, -210, -340, -440, 0, -105, 0, 
-105, -210};
const int POSITION_Y[12] = {0, 0, 35, 35, 35, 35, 35, 75, 75, 0, 0, 0};
const int COMMIT_POWER = 100;
const int POWER_X = 100;
const int STRUM_POSITION_1 = 5;
const int STRUM_POSITION_2 = -25;
const int ENC_LIM_Z = -225;

int chordPositionX[MAX_SIZE];
int chordPositionY[MAX_SIZE];
int timings[MAX_SIZE];

int chordNum(char chordLetter);
int getTimeValue(char timeSymbol);
int readFile(TFileHandle & fin, int* chordsPositionX, int* chordsPositionY, 
int* timings);
void setGantryPos(int chordPositionX, int chordPositionY, int lastPositionX);
void strum(bool & isLeft);
void commitOrRelease();
void terminate();
void displaySong(bool isPixel);
void pause(bool timerNeeded);
task stopAll();

task main()
{
	SensorType[S1] = sensorI2CCustom9V;
	SensorType[S2] = sensorEV3_Touch;
	
	startTask(stopAll);
	displayBigTextLine(0, "Smoke on the Water");
	displayBigTextLine(2, "By Deep Purple");
	displayString(4, "Press Enter to Select");
	displayString(5, "Press Left or Right to Change");
	displayString(6, "Press Touch Sensor to End");

	int songChoice = 0;

	while (!getButtonPress(buttonEnter))
	{
		if(getButtonPress(buttonLeft) || getButtonPress(buttonRight))
		{
			while(getButtonPress(buttonLeft) || getButtonPress(buttonRight)){}
			songChoice++;

			if(songChoice % 2 == 1)
			{
				displayBigTextLine(0, "Free Fallin'");
				displayBigTextLine (2, "By Tom Petty");
			}
			else
			{
				displayBigTextLine(0, "Smoke on the Water");
				displayBigTextLine(2, "By Deep Purple");
			}
		}
	}

	while(getButtonPress(buttonEnter))
	{}

	eraseDisplay();
	displayString(0, "Choose a Speed:");
	displayString(1, "Press Enter for Normal Speed");
	displayString(2, "Press Right for half Speed");
	displayString(3, "Press Left for quarter Speed");
	displayString(4, "Press Touch Sensor to End");

	int baseTempo = 4;

	while (!getButtonPress(buttonAny))
	{}
	if(getButtonPress(buttonRight))
		baseTempo *= 2;
	else if(getButtonPress(buttonLeft))
		baseTempo *= 4;
	
	while (getButtonPress(buttonAny))
	{}
	
	for (int count = 0; count < MAX_SIZE; count++)
	{
		chordPositionX[count] = 0;
		chordPositionY[count] = 0;
		timings[count] = 0;
	}

	TFileHandle fin;
	bool fileOkay;
	
	eraseDisplay();
	if(songChoice % 2 == 1)
	{
		fileOkay = openReadPC(fin, "free_fallin.txt");
		setPixel(0,0);
	}
	else
		fileOkay = openReadPC(fin, "smoke.txt");

	displaySong(getPixel(0,0));

	if(!fileOkay)
		terminate();

	int chordNumber = readFile(fin, chordPositionX, chordPositionY, timings);
	closeFilePC(fin);

	bool isLeft = true;

	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorC] = 0;

	int currChord = 0;
	int lastPositionX = 0;
	int lastPositionY = 0;
	time1[T1] = 0;

	commitOrRelease();
	while (currChord < chordNumber)
	{
		while(time1[T1] < timings[currChord] * baseTempo)
			if(getButtonPress(buttonEnter))
				pause(true);

		commitOrRelease();
		setGantryPos(chordPositionX[currChord], chordPositionY[currChord], 
		lastPositionX);

		commitOrRelease();
		strum(isLeft);

		if(getButtonPress(buttonEnter))
			pause(false);

		lastPositionX = chordPositionX[currChord];
		lastPositionY = chordPositionY[currChord];

		currChord++;
		time1[T1] = 0;
	}

	terminate();

}

int chordNum(char chordLetter)
{
	switch (chordLetter)
	{
	case 'a':
		return 0;
	case 'A':
		return 1;
	case 'b':
		return 2;
	case 'B':
		return 3;
	case 'C':
		return 4;
	case 'd':
		return 5;
	case 'D':
		return 6;
	case 'e':
		return 7;
	case 'E':
		return 8;
	case 'F':
		return 9;
	case 'g':
		return 10;
	case 'G':
		return 11;
	}
	return 0;
}

int getTimeValue(char timeSymbol)
{
	switch(timeSymbol)
	{
		case '_':
			return 400;
		case ':':
			return 300;
		case '/':
			return 200;
		case '.':
			return 100;
		case ',':
			return 50;
		case '$':
			return 150;
		case '-':
			return 250;
		case '{':
			return 350;
	}
	return 0;
}

int readFile(TFileHandle & fin, int* chordsPositionX, int* chordsPositionY, 
int* timings)
{
	int count = 0;
	char currentChord;
	char timingSymbol;
	while(readCharPC(fin, currentChord) && readCharPC(fin, timingSymbol))
	{
		chordsPositionX[count] = POSITION_X[chordNum(currentChord)];
		chordsPositionY[count] = POSITION_Y[chordNum(currentChord)];
		timings[count+1] = getTimeValue(timingSymbol);

		count++;
	}
	return count;
}

void setGantryPos(int chordPositionX, int chordPositionY, int lastPositionX)
{
	int distanceX = chordPositionX - lastPositionX;

	if (distanceX < 0)
	{
		motor[motorA] = motor[motorB] = -POWER_X;
		while(nMotorEncoder[motorA] > chordPositionX)
			if(getButtonPress(buttonEnter))
				pause(false);
	}
	else
	{
		motor[motorA] = motor[motorB] = POWER_X;
		while(nMotorEncoder[motorA] < chordPositionX)
			if(getButtonPress(buttonEnter))
				pause(false);
	}
	motor[motorA] = motor[motorB] = 0;

	setServoPosition(S1, 1, chordPositionY); //RIGHT
	setServoPosition(S1, 2, -chordPositionY); //LEFT
}

void commitOrRelease()
{

	// Raise Arm
	if(nMotorEncoder[motorC] < 0)
	{
		motor[motorC] = COMMIT_POWER;
		while(nMotorEncoder[motorC] < 0)
			if(getButtonPress(buttonEnter))
				pause(false);
	}

	// Lower Arm
	else
	{
		motor[motorC] = -COMMIT_POWER;
		while(nMotorEncoder[motorC] > ENC_LIM_Z)
			if(getButtonPress(buttonEnter))
				pause(false);
	}
	motor[motorC] =  0;
}

void strum(bool & isLeft)
{
	if(getButtonPress(buttonEnter))
		pause(false);

	if (isLeft)
	{
		setServoPosition(S1, 3, STRUM_POSITION_1);
		isLeft = false;
	}
	else
	{
		setServoPosition(S1, 3, STRUM_POSITION_2);
		isLeft = true;
	}
}

void terminate()
{
	motor[motorC] = COMMIT_POWER;
	while	(nMotorEncoder[motorC] < 0)
	{}
	motor[motorC] = 0;
	motor[motorA] = motor[motorB] = POWER_X;
	while(nMotorEncoder[motorA] < 0)
	{}
	motor[motorA] = motor[motorB] = 0;
	setServoPosition(S1,1,0);
	setServoPosition(S1,2,0);
	setServoPosition(S1,3,STRUM_POSITION_2);
	stopAllTasks();
}

void displaySong(bool isPixel)
{
	if(isPixel)
	{
		displayBigTextLine(0, "Free Fallin'");
		displayBigTextLine (2, "By Tom Petty");
	}
	else
	{
		displayBigTextLine(0, "Smoke on the Water");
		displayBigTextLine(2, "By Deep Purple");
	}

	displayString(4, "Press Enter to Pause");
	displayString(5, "Press Touch Sensor to End");
	drawBmpfile(0, 75, "Fire");
}

void pause(bool timerNeeded)
{
	while(getButtonPress(buttonEnter))
	{}
	
	int timer = time1[T1];
	bool isPixel = getPixel(0,0);
	int motorAPower = getMotorSpeed(motorA);
	int motorBPower = getMotorSpeed(motorB);
	int motorCPower = getMotorSpeed(motorC);
	motor[motorA] = motor[motorB] = motor[motorC] = 0;

	eraseDisplay();
	displayBigTextLine(0, "PAUSED");
	displayString(2, "Press Enter to Unpause");
	displayString(3, "Press Touch Sensor to End");

	while(!getButtonPress(buttonEnter))
	{}
	while(getButtonPress(buttonEnter))
	{}

	if(timerNeeded)
	{
		time1[T1] = 0;
		while(time1[T1] < timer)
		{}
	}
	motor[motorA] = motorAPower;
	motor[motorB] = motorBPower;
	motor[motorC] = motorCPower;
	eraseDisplay();
	displaySong(isPixel);
}

task stopAll()
{
	while(SensorValue[S2] == 0)
	{}
	while(taskStateRunning){
	if(SensorValue[S2] == 1)
		terminate();
	}
}


