/*
	To be compiled with the RPi_GPIO library.
	
	Device abstractions in this file are as follows
	-LED	(ON/OFF)
	-Switch (Modal)
	-Button (Wait for press)
*/

#include "RPi_GPIO.h"

struct LED{
	PIN * p;
	bool on; // This led is on, true/false
};

struct Switch{
	PIN * p;
	bool lastMode;
};

struct Button{
	PIN * p;
};

