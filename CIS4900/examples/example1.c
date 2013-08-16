/* This is a test file for the RPi_GPIO library written by Bill Vandenberk
Date: August 15, 2013
Purpose: Show how to use the pin ADT.

To compile this program use this command:
gcc example1.c -o example1 -lRPi_GPIO
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

	/*Storing the pin number in a vcariable allows us to change it easily*/
	int pin = 10;

	/*Lets open a PIN*/	
	PIN * myP = RPi_popen(0,ACTIVE_HIGH,OUT);
	/*We need to check that the pin was actually opened before we use it*/
	if(myP == NULL){
		/*If it wasn't opened, let's find out why*/
		printf("ERROR! %s\n", RPi_errorstr(RPi_errorno()));
	}

	/*Lets put a value of true on that pin (3.3v)*/
	if(0!=RPi_pwrite(myP,true)){
		/*If that didn't work, let's find out why*/
		printf("ERROR! %s\n", RPi_errorstr(RPi_errorno()));		
	}

	/*Sleep for one second to let our pin stay at true*/
	sleep(1);

	/*We put our pin back to false (0v)*/
	if(0!=RPi_pwrite(myP,false)){
		/*If that didn't work, let's find out why*/
		printf("ERROR! %s\n", RPi_errorstr(RPi_errorno()));		
	}

	/*We are done with our pin now, lets close it.*/
	if(0!=RPi_pclose(myP)){
		/*If we couldnt close the pin, let's find out why*/
		printf("ERROR! %s\n", RPi_errorstr(RPi_errorno()));		
	}
}
