const int ENC_LIM_Z = -200;
const int COMMIT_POWER = 100;

/*
Function 5: commitOrRelease
Toggles position of the Z motor in order to apply pressure to strings in chord
formation.
pre: port for Z motor.
post: The Z motor will either be lifted or lowered depending on its prior
position. If the motor was elevated, it will now be lowered, and vice-versa.
*/
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
Raise and lower the Z axis multiple concurrent times to test response and 
durability of hardware.
*/
task main()
{
	//Assume z-axis begins at maximum elevation
	int count = 0;
	while(count < 10)
	{
	commitOrRelease();
	commitOrRelease();
	count++;
	}
}
