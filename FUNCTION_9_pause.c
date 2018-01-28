
// Function 9: pause

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

