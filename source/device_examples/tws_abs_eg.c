/* This is a test file for the RPi_GPIO library written by Bill Vandenberk
Date: August 15, 2013
Purpose: Show how to use the 3-way-switch abstraction.

To compile this program use this command:
gcc tws_abs_eg.c -o tws_abs_eg -lRPi_GPIO
*/

/*First, we need access to the library*/
#include <RPi_GPIO.h>
/*Second, lets get some i/o access*/
#include <stdio.h>

int main (){
	/*We need to call this function at the beginning of all out programs that use the library*/
	RPi_init();

	/*The pins that the switch is connected to*/
	int pin1 = 13;
	int pin2 = 15;

	int position=-1;

	/*We need a switch, and we're going to want to make sure it got created*/
	TWS * sw = RPi_TWS_open(pin1,pin2);
	if(sw==NULL){
		printf("Error: %s\n",RPi_errorstr(RPi_errorno()));
		return 1;
	}

	/*Lets see if its light or dark out there*/
	if(0!=RPi_TWS_readPosition(sw,&position)){
		printf("Error: %s\n", RPi_errorstr(RPi_errorno()));
		return 1;
	}

	/*Tell the world what we have discovered!*/
	printf("The switch is in position %d.\n",position);

	/*Close our connection to the photoresistor*/
	if(0!=RPi_TWS_close(sw)){
		printf("Error: %s\n", RPi_errorstr(RPi_errorno()));
		return 1;
	}

	return 0;
}