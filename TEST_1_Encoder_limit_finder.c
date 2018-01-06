/* Encoder Finder: used to find encoder vales */
task main()
{
	nMotorEncoder[motorA] = nMotorEncoder[motorB] = 0;

	while (!getButtonPress(buttonAny))
	{}

	motor[motorA] = motor[motorB] = -50;

	while (getButtonPress(buttonAny))
	{}

	motor[motorA] = motor[motorB] = 0;

	displayString(0, "%i", nMotorEncoder[motorA]);
	displayString(1, "%i", nMotorEncoder[motorB]);

	wait1Msec(5000);
}
