/* This is a test file for the RPi_GPIO library written by Bill Vandenberk
Date: August 15, 2013
Purpose: Show how to use the photoresistor abstraction.

To compile this program use this command:
gcc phr_abs_eg.c -o phr_abs_eg -lRPi_GPIO
*/

/*First, we need access to the library*/
#include <RPi_GPIO.h>
/*Second, lets get some i/o access*/
#include <stdio.h>

int main (){
	/*We need to call this function at the beginning of all out programs that use the library*/
	RPi_init();

	/*The pin that the resistor is connected to*/
	int pin = 13;

	bool isitbright;

	/*We need a photoresistor, and we're goingto want to make sure it got created*/
	PHR * sense = RPi_PHR_open(pin);
	if(sense==NULL){
		printf("Error: %s\n",RPi_errorstr(RPi_errorno()));
		return 1;
	}

	/*Lets see if its light or dark out there*/
	if(0!=RPi_PHR_read(sense,&isitbright)){
		printf("Error: %s\n", RPi_errorstr(RPi_errorno()));
		return 1;
	}

	/*Tell the world what we have discovered!*/
	if(isitbright){
		printf("It's bright!\n");
	}
	else{
		printf("It's dark!\n");
	}

	/*Close our connection to the photoresistor*/
	if(0!=RPi_PHR_close(sense)){
		printf("Error: %s\n", RPi_errorstr(RPi_errorno()));
		return 1;
	}

	return 0;
}