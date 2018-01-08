/* This driver allowed us to find the appropriate encoder
 limit for the chord commission system.*/

task main()
{
	while(!getButtonPress(buttonAny))
    	{}
    
	nMotorEncoder[motorA] = 0;
	motor[motorA] = -30;

	while (getButtonPress(buttonAny))
	{}
    
	motor[motorA] = 0;
    	displayString(0, "Encoder Value: %i", nMotorEncoder[motorA]);
	
    	motor[motorA] = 30;
    
    	while(nMotorEncoder[motorA] < 0)
    	{}

	motor[motorA] = 0;
    
   	wait1Msec(2000);
}
