/*
 * BinClock.c
 * Jarrod Olivier
 * Modified for EEE3095S/3096S by Keegan Crankshaw
 * August 2019
 *
 * <STUDNUM_1> <STUDNUM_2>
 * Date
*/

#include <wiringPi.h>
#include <stdio.h>  	    // For printf functions
#include <stdlib.h>         // For system functions
#include <signal.h> 	    // For catching interrupt and aborts

#include "BinClock.h"
#include "CurrentTime.h"

//Global variables
long lastInterruptTime = 0; // Used for button debounce
int HH,MM,SS;

/*
 * The main function
 * This function is called, and calls all relevant functions we've written
 */
int main(void){

	printf("Setting up\n");
	wiringPiSetup(); //This is the default mode. If you want to change pinouts, be aware
	printf("Setup done\n");
	
	// Repeat this until we shut down
	for (;;){
		//Fetch the time from the RTC
		toggleTime();

		// Print out the time we have stored on our RTC
		printf("The current time is: %x:%x:%x\n", HH , MM, SS);

		// using a delay to make our program "less CPU hungry"
		delay(1000); //milliseconds
	}
	return 0;
}

/*
 * Change the hour format to 12 hours
 */
int hFormat(int hours){
	/*formats to 12h*/
	if (hours >= 24){
		hours = 0;
	}
	return (int)hours;
}

/*
 * hexCompensation
 * This function may not be necessary if you use bit-shifting rather than decimal checking for writing out time values
 */
int hexCompensation(int units){
	/*Convert HEX or BCD value to DEC where 0x45 == 0d45 
	  This was created as the lighXXX functions which determine what GPIO pin to set HIGH/LOW
	  perform operations which work in base10 and not base16 (incorrect logic) 
	*/
	int unitsU = units%0x10;

	if (units >= 0x50){
		units = 50 + unitsU;
	}
	else if (units >= 0x40){
		units = 40 + unitsU;
	}
	else if (units >= 0x30){
		units = 30 + unitsU;
	}
	else if (units >= 0x20){
		units = 20 + unitsU;
	}
	else if (units >= 0x10){
		units = 10 + unitsU;
	}
	return units;
}


/*
 * decCompensation
 * This function "undoes" hexCompensation in order to write the correct base 16 value through I2C
 */
int decCompensation(int units){
	int unitsU = units%10;

	if (units >= 50){
		units = 0x50 + unitsU;
	}
	else if (units >= 40){
		units = 0x40 + unitsU;
	}
	else if (units >= 30){
		units = 0x30 + unitsU;
	}
	else if (units >= 20){
		units = 0x20 + unitsU;
	}
	else if (units >= 10){
		units = 0x10 + unitsU;
	}
	return units;
}


//This interrupt will fetch current time from another script and write it to the clock registers
//This functions will toggle a flag that is checked in main
void toggleTime(void){
	long interruptTime = millis();

	if (interruptTime - lastInterruptTime > 500){
		HH = decCompensation(getHours());
		MM = decCompensation(getMins());
		SS = decCompensation(getSecs());
	}
	lastInterruptTime = interruptTime;
}

/*
 * signalHandler
 * This function handles any KeyBoardInterrupts.
 */
void signalHandler(int signum) {
	printf("Interrupt signal received.\n");
	exit(signum);
}

