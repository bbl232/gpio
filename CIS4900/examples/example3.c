/* This is a test file for the RPi_GPIO library written by Bill Vandenberk
Date: August 15, 2013
Purpose: Blink 2 different coloured LEDs based on the position of a three way switch until the switch is at position 2.

To compile this program use this command:
gcc example3.c -o example3 -lRPi_GPIO
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

	/*These variables keep track of the pins everything is connected to*/
	int led1pin=9;
	int led2pin=10;
	int twspin1=5;
	int twspin2=13;
	int switchPosition=0;

	/*We need to open our LEDs and check that they were opened correctly*/
	LED * led1 = RPi_LED_open(led1pin);
	if(led1==NULL){
		/*If there was an error, we should report it*/
		printf("Error: %s\n", RPi_errorstr(RPi_errorno()));
		return 1;
	}

	LED * led2 = RPi_LED_open(led2pin);
	if(led2==NULL){
		printf("Error: %s\n", RPi_errorstr(RPi_errorno()));
		return 1;
	}

	/*Now we need to open our switch, and make sure it worked*/
	TWS * sw = RPi_TWS_open(twspin1,twspin2);
	if(sw==NULL){
		printf("Error: %s\n", RPi_errorstr(RPi_errorno()));
		return 1;
	}

	/*We want to loop until the switch is in position 2*/
	while(switchPosition!=2){
		/*Read the position of the switch, report any errors*/
		if(0!=RPi_TWS_readPosition(sw,&switchPosition)){
			printf("Error: %s\n", RPi_errorstr(RPi_errorno()));
			break;
		}

		/*A simple way to light one of the two LEDs*/
		switch(switchPosition){
			case 0:
				/*Position 0 lights led1*/
				if(0!=RPi_LED_on(led1)){
					printf("Error: %s\n", RPi_errorstr(RPi_errorno()));
				}
				break;
			default:
				/*Anything else will light led2*/
				if(0!=RPi_LED_on(led2)){
					printf("Error: %s\n", RPi_errorstr(RPi_errorno()));
				}
				break;
		}

		sleep(1); /*Let our light shine bright for one second*/

		/*Turn off both LEDs, report errors*/
		if(0!=RPi_LED_off(led1) || 0!=RPi_LED_off(led2)){
			printf("Error: %s\n", RPi_errorstr(RPi_errorno()));
		}

		sleep(1); /*Enjoy the darkness for a moment*/
	}

	/*Close all our devices afterwards*/
	if(0!=RPi_LED_close(led1) || 0!=RPi_LED_close(led2) || 0!=RPi_TWS_close(sw)){
		printf("Error: %s\n", RPi_errorstr(RPi_errorno()));
		return 1;
	}

	return 0;
}