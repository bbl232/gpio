/*
	To be compiled with the RPi_GPIO library.
	
	Device abstractions in this file are as follows
	-LED	(ON/OFF)
	-Switch (Modal)
	-Button (Wait for press)
*/

#include "RPi_GPIO.h"
#include <stdlib.h>
struct LED{
	PIN * p;
	bool on; // This led is on, true/false
};

struct Switch{
	PIN * p;
	bool lastMode; // Last position of the switch
};

struct Button{
	PIN * p;
};

LED * RPi_LED_open(int pin){
	LED * newLED = malloc(sizeof(struct LED));
	if (newLED == NULL){
		LASTERR=7;
		return NULL;
	}
	newLED->p = RPi_popen(pin,ACTIVE_HIGH,OUT);
	if(newLED->p == NULL){
		free(newLED);
		return NULL;
	}
	newLED->on = false;
	if(0!=RPi_pwrite(newLED->p,false)){
		free(newLED->p);
		free(newLED);
		return NULL;
	}
	return newLED;
}

int RPi_LED_ON(LED * l){
	if(l==NULL || l->p==NULL){
		LASTERR=4;
		return 1;
	}
	return RPi_pwrite(l->p,true);
}

int RPi_LED_OFF(LED * l){
	if(l==NULL || l->p==NULL){
		LASTERR=4;
		return 1;
	}
	return RPi_pwrite(l->p,false);
}

int RPi_LED_toggle(LED * l){
	if(l==NULL || l->p==NULL){
		LASTERR=4;
		return 1;
	}
	if(l->on){
		return RPi_pwrite(l->p,false);
	}
	return RPi_pwrite(l->p,true);
}

int RPi_LED_close(LED * l){
	if(l==NULL || l->p==NULL){
		LASTERR=4;
		return 1;
	}
	free(l->p);
	free(l);
	return 0;
}