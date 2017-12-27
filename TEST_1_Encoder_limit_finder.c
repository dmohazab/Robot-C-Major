/* Encoder Finder: This code allows us to find the encoder values
corresponding to each chord. This test was run multiple times per
fret in order to give us a range of encoder values for which the
chord is still correct*/
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
