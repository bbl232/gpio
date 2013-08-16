/* This is a test file for the RPi_GPIO library written by Bill Vandenberk
Date: August 15, 2013
Purpose: loop until we detect that the lights have been turned off then light an LED, then keep it on until the lights turn back on

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
