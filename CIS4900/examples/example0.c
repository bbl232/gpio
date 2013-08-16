/* This is a test file for the RPi_GPIO library written by Bill Vandenberk
Date: August 15, 2013
Purpose: Show how to use the LED abstraction.

To compile this program use this command:
gcc example0.c -o example0 -lRPi_GPIO
*/
/*First off, we need access to the library*/
#include "RPi_GPIO.h"
/*We also need printf*/
#include <stdio.h>
/*Finally, we need sleep*/
#include <stdlib.h>

/*This gets run when you run the program*/
int main(){
	/*The RPi_init function must be called at the beginning of each program that uses the library*/
	RPi_init();

	/*Storing the pin number in a variable allows us to change it easily*/
	int pin = 10;

	/*Lets open an LED*/	
	LED * myL = RPi_LED_open(pin);
	/*We need to check that the LED was actually opened before we use it*/
	if(myL == NULL){
		/*If it wasn't opened, let's find out why*/
		printf("ERROR! %s\n", RPi_errorstr(RPi_errorno()));
	}

	/*Lets light up this LED*/
	if(0!=RPi_LED_on(myL)){
		/*If that didn't work, let's find out why*/
		printf("ERROR! %s\n", RPi_errorstr(RPi_errorno()));		
	}

	/*Sleep for one second to let our LED stay on*/
	sleep(1);

	/*We turn the LED back off*/
	if(0!=RPi_LED_off(myL)){
		/*If that didn't work, let's find out why*/
		printf("ERROR! %s\n", RPi_errorstr(RPi_errorno()));		
	}

	/*We are done with our LED now, lets close it.*/
	if(0!=RPi_LED_close(myL)){
		/*If we couldnt close the LED, let's find out why*/
		printf("ERROR! %s\n", RPi_errorstr(RPi_errorno()));		
	}
}
