#include "EV3Servo-lib-UW.c"
const int STRUM_POSITION_1 = 5;
const int STRUM_POSITION_2 = -25;

/*
Function 6: strum
Moves the strumming arm in a two dimensional plane as to simulate strumming a guitar.
pre: boolean passed by reference to keep track of position, time measure int, and base tempo integer.
post: strumming arm is moved in accordace to the strum position angle.
*/
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

/*
DRIVER:
Test Function by running the code of the function through a task main.
Calls for movement of the strumming arm in both directions.
*/
task main()
{

	SensorType[S1]=sensorI2CCustom9V;
	bool position = true;
	int timeMeasure = 200; //2 seconds
	int baseTempo = 2; // Base measure of time
	int counter = 0;

   //Each strum in this test case takes 1 second (1000ms), because the tempo of 0.5 halves the time of 2 seconds

	while(counter < 2)
	{
	time1[T1] = 0;
	strum(position, timeMeasure, baseTempo);
	counter++;
    }
}
/*
Strummer tested with times of:

Test set 1:

timeMeasure: 1 seconds
baseTempo: 1

timeMeasure: 1 seconds
baseTempo: 0.75

timeMeasure: 1 seconds
baseTempo: 0.5

timeMeasure: 1 seconds
baseTempo: 0.4

Test set 2:

timeMeasure: 2 seconds
baseTempo: 1

timeMeasure: 2 seconds
baseTempo: 0.75

timeMeasure: 2 seconds
baseTempo: 0.5

timeMeasure: 2 seconds
baseTempo: 0.4

Test set 3:

timeMeasure: 3 seconds
baseTempo: 1

timeMeasure: 3 seconds
baseTempo: 0.75

timeMeasure: 3 seconds
baseTempo: 0.5

timeMeasure: 3 seconds
baseTempo: 0.4

Test set 4:

timeMeasure: 4 seconds
baseTempo: 1

timeMeasure: 4 seconds
baseTempo: 0.75

timeMeasure: 4 seconds
baseTempo: 0.5

timeMeasure: 4 seconds
baseTempo: 0.4

*/
