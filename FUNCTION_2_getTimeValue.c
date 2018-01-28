// Function 2: Get integer Time Value

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

/* DRIVER:
Test Function by running the code of the function through a task main.
Run through every return case within the switch statement and print the values returned to the screen.
*/
task main()
{
	displayString(0,"Time value: %f", getStrumTimeValue('_'));
	displayString(1,"Time value: %f", getStrumTimeValue(':'));
	displayString(2,"Time value: %f", getStrumTimeValue('/'));
	displayString(3,"Time value: %f", getStrumTimeValue('.'));
	displayString(4,"Time value: %f", getStrumTimeValue(','));
	displayString(5,"Time value: %f", getStrumTimeValue('$'));
	displayString(5,"Time value: %f", getStrumTimeValue('-'));
	displayString(5,"Time value: %f", getStrumTimeValue('{'));
	displayString(5,"Time value: %f", getStrumTimeValue('@'));

	wait1Msec(10000);
}

/*
Code Output:

Time value: 400
Time value: 300
Time value: 200
Time value: 100
Time value: 50
Time value: 150
Time value: 250
Time value: 350
Time value: 0
*/
