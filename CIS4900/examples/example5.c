/* This is a test file for the RPi_GPIO library written by Bill Vandenberk
Date: August 15, 2013
Purpose: loop until we detect that the lights have been turned off
then light an LED, then keep it on until the lights turn back on
NOTE: careful not to point the LED at the light sensor.

To compile this program use this command:
gcc example5.c -o example5 -lRPi_GPIO
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

	/*Pins that the components are hooked up to*/
	int phrpin=13;
	int ledpin=10;
	/*Value of the photoresistor*/
	bool isitbright = true;

	/*We need a photoresistor, lets make one and check that it got made*/
	PHR * sense = RPi_PHR_open(pin);
	if (sense==NULL){
		printf("Error: %s\n",RPi_errorstr(RPi_errorno()));
		return 1;
	}

	/*We also need an LED*/
	LED * light = RPi_LED_open(pin);
	if (light==NULL){
		printf("Error: %s\n",RPi_errorstr(RPi_errorno()));
		return 1;
	}

	/*While it is bright, we do not want to do anyhting*/
	while(isitbright){
		if(0!=RPi_PHR_read(sense, &isitbright)){
			printf("Error: %s\n",RPi_errorstr(RPi_errorno()));
			break;
		}
	}

	/*Once it is dark, turn on the LED*/
	if(0!=RPi_LED_on(light)){
		printf("Error: %s\n",RPi_errorstr(RPi_errorno()));
		return 1;
	}

	/*Leave the LED on until it is bright again*/
	/*While it is dark, we do not want to do anyhting*/
	while(!isitbright){
		if(0!=RPi_PHR_read(sense, &isitbright)){
			printf("Error: %s\n",RPi_errorstr(RPi_errorno()));
			break;
		}
	}

	/*Once it is bright, turn off the LED*/
	if(0!=RPi_LED_off(light)){
		printf("Error: %s\n",RPi_errorstr(RPi_errorno()));
		return 1;
	}

	/*Close our devices*/
	if(0!=RPi_LED_close(light) || 0!=RPi_PHR_close(sense)){
		printf("Error: %s\n",RPi_errorstr(RPi_errorno()));
		return 1;
	}
	return 0;
}
