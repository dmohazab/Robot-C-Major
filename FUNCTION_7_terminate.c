
/* Function 7: terminate
This function terminates the program at any given time and sets everything 
back to their original positions.
*/
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
