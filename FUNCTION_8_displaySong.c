
// Function 8: displaySong

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

/* Driver: Main created in order to test if the song
will display as expected.*/
task main()
{
    setPixel(0,0);
    // Free Fallin' displayed
    displaySong(getPixel(0,0));
    
    wait1Msec(3000);
    
    eraseDisplay();
    // Smoke on the Water displayed
    displaySong(getPixel(0,0));
    
    wait1Msec(3000);
}
