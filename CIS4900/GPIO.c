/*
TODO:
Change function names to something more logical incl gpio_ prefix
set up pin idle open/close
set up errorno and errorstr functions

*/

#include "RPi_GPIO.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

/*Where the last error code is stored*/
int LASTERR;

/*Base directory for the GPIO controller fs
DEFAULT: /sys/class/gpio/
*/
static const char const * GPIODIR = "/sys/class/gpio/";
static const char const * GPIOEX = "/sys/class/gpio/export";
static const char const * GPIOUNEX = "/sys/class/gpio/unexport";

bool exported[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; /*Keeps track of which pins are exported*/

#ifdef RPi_board_rev_1
    int hardPin[17] = {0,1,4,17,21,22,10,9,11,7,8,25,24,23,18,15,14}; /*REV1:Maps the soft pins to the hardware pins.*/
#else
    int hardPin[17] = {2,3,4,17,27,22,10,9,11,7,8,25,24,23,18,15,14}; /*REV2:Maps the soft pins to the hardware pins.*/
#endif

struct pin {
	int location;
	enum RPi_direction dire;
	enum RPi_logicType logic;
	enum RPi_direction currentDire;
};

int RPi__export(int pin);
int RPi__unexport(int pin);
int RPi__direction(int pin, enum RPi_direction dire);
int RPi__logic(int pin, enum RPi_logicType logic);
int RPi__getValue (int pinNum, bool * value);
int RPi__setValue (int pinNum, bool value);

/*NON_ADT functions
 */

int RPi__setValues (bool value, ...){
	if(value != true && value != false){
		return false;
	}
	int numSet = 0;
	va_list pins;
	va_start(pins,value);
	int pin;
	while ((pin=va_arg(pins,int))>=0){
		if(RPi__setValue(pin,value)){
			numSet++;
		}
	}
	va_end(pins);
	return numSet;
} 

int RPi_init (){
	int i = 0;
	for(;i<17;i++){
		RPi__unexport(i);
	}
	return 0;
}

/*Low Level GPIO ADT... pin management, etc
 *Functions: PinOn(Create), PinOff(Destroy), Read, Write, SetLogic, SetDirection
 */

PIN * RPi_popen(int number, enum RPi_logicType logic, enum RPi_direction direc){
	if(0==RPi__export(number)){
		PIN * newPin = malloc(sizeof(struct pin));
		if (newPin == NULL){
			LASTERR = 1;
			return NULL;
		}
		newPin->location = number;
		newPin->dire = direc;
		newPin->logic = logic;
		if(0!=RPi__logic(number, logic)){
			free(newPin);
			return NULL;
		}
		if(direc==IN || direc==INOUT){
			newPin->currentDire = IN;
			if(0!=RPi__direction(number, IN)){
				free(newPin);
				return NULL;
			}
		}
		else{
			newPin->currentDire = OUT;
			if(0!=RPi__direction(number, OUT)){
				free(newPin);
				return NULL;
			}
		}
		return newPin;
	}
	LASTERR=2;
	return NULL;
}

int RPi_pclose(PIN * p){
	if(p != NULL && 0==RPi__unexport(p->location)){
		free(p);
		return 0;
	}
	return 1;
} 

int RPi_pread(PIN * p, bool * in){
	if(p != NULL && in != NULL){
		if(p->dire==IN){
			if(0==RPi__getValue(p->location, in)){
				return 0;
			}
			return 1;
		}
		else if(p->dire == INOUT){
			if(p->currentDire == IN){
				if(0!=RPi__getValue(p->location, in)){
					return 1;
				}
				return 0;
			}
			else{
				if(0!=RPi__direction(p->location, IN)){
					return 1;
				}
				p->currentDire = IN;
				if(0!=RPi__getValue(p->location, in)){
					return 1;
				}
				return 0;
			}
		}
		else{
			LASTERR=6;
			return 1;
		}
	}
	LASTERR=4;
	return 1;
}

int RPi_pwrite(PIN * p, bool value){
	if(p != NULL){
		if(p->dire==OUT){
			if(0==RPi__setValue(p->location, value)){
				return 0;
			}
			return 1;
		}
		else if(p->dire == INOUT){
			if(p->currentDire == OUT){
				if(0!=RPi__setValue(p->location, value)){
					return 1;
				}
				return 0;
			}
			else{
				if(0!=RPi__direction(p->location, OUT)){
					return 1;
				}
				p->currentDire = OUT;
				if(0!=RPi__setValue(p->location, value)){
					return 1;
				}
				return 0;
			}
		}
		else{
			LASTERR=5;
			return 1;
		}
	}
	LASTERR=4;
	return 1;
}

int RPi_pdirection(PIN * p, enum RPi_direction dire){
	if(p != NULL){
		if(dire != p->dire){
			if (dire == INOUT){
				p->dire = INOUT;
				return 0;
			}
			else{
				if(0==RPi__direction(p->location,dire)){
					p->dire = dire;
					p->currentDire = dire;
					return 0;
				}
				else{
					return 1;
				}
			}
		}
		return 0;
	}
	LASTERR=4;
	return 1;
}

int RPi_pidle(PIN * p){
	if(p != NULL){
		return RPi__unexport(p->location);
	}
	LASTERR=4;
	return 1;
}


/* End ADT Functions
 *
 */


int RPi__getValue (int pinNum, bool * value){
	if(0==RPi__export(pinNum)){
		char * fn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+9);
		sprintf(fn,"%s%s%d/value",GPIODIR,"gpio",hardPin[pinNum]);
		FILE * val = fopen(fn,"r");
		if(val == NULL){
			LASTERR=2;
			return 1;
		}
		int temp = 27;
		fscanf(val,"%d",&temp);
		if (0==temp){
			*value = false;
		}
		else{
			*value = true;
		}
		fclose(val);
		free(fn);
		return 0;
	}
	LASTERR=2;
	return 1;
}

int RPi__setValue (int pinNum, bool value){
	if(0==RPi__export(pinNum)){
		char * fn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+9);
		sprintf(fn,"%s%s%d/value",GPIODIR,"gpio",hardPin[pinNum]);
		FILE * val = fopen(fn,"w");
		if(val == NULL){
			free (fn);
			LASTERR=2;
			return 1;
		}
		fprintf(val,"%d",value==true);
		fclose(val);
		free(fn);
		return 0;
	}
	LASTERR=2;
	return 1;
} 

/*This function remaps the soft pin to the hard pin and does the actual sysfs interaction with the GPIO controller in order to export a pin*/
int RPi__export(int pin){

	if(pin >= 17 || pin < 0){
		LASTERR=3;
		return 1;
	}

	if(1==exported[pin]){
		return 0;
	}

	FILE * ex = fopen(GPIOEX,"w");
	if(ex==NULL){
		LASTERR=2;
		return 1;
	}
	fprintf(ex,"%d",hardPin[pin]);
	fclose(ex);
	char * check = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+4);
	sprintf(check,"%s%s%d/",GPIODIR,"gpio",hardPin[pin]);
	if(0==access(check,F_OK)){
		free(check);
		exported[pin]=true;
		return 0;
	}
	free(check);
	return 1;
}

/*This function remaps the soft pin to the hard pin and does the actual sysfs interaction with the GPIO controller in order to unexport a pin*/
int RPi__unexport(int pin){
	if(pin >= 17 || pin < 0){
		LASTERR=3;
		return 1;
	}

	FILE * ex = fopen(GPIOUNEX,"w");
	if(ex==NULL){
		LASTERR=2;
		return 1;
	}
	fprintf(ex,"%d",hardPin[pin]);
	fclose(ex);
	char * check = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+4);
	sprintf(check,"%s%s%d/",GPIODIR,"gpio",hardPin[pin]);
	if(0!=access(check,F_OK)){
		free(check);
		exported[pin]=false;
		return 0;
	}
	free(check);
	return 1;
}

int RPi__direction(int pin, enum RPi_direction dire){
	if(0==RPi__export(pin)){
		if(pin >= 17 || pin < 0){
			LASTERR=3;
			return 1;
		}

		char * direfn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+13);
		sprintf(direfn,"%s%s%d/direction",GPIODIR,"gpio",hardPin[pin]);
		FILE * direfh = fopen(direfn,"w");
		if(direfh == NULL){
			LASTERR=2;
			return 1;
		}

		if (dire == IN){
			fprintf(direfh,"%s","in");
		}
		else{
			fprintf(direfh,"%s","out");
		}
		fclose(direfh);
		free(direfn);

		return 0;
	}
	return 1;
}

int RPi__logic(int pin, enum RPi_logicType logic){
	if(0==RPi__export(pin)){
		if(pin >= 17 || pin < 0){
			LASTERR=3;
			return 1;
		}

		char * lfn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+14);
		sprintf(lfn,"%s%s%d/active_low",GPIODIR,"gpio",hardPin[pin]);
		FILE * lfh = fopen(lfn,"w");
		if(lfh == NULL){
			LASTERR=2;
			return 1;
		}

		fprintf(lfh,"%d", logic == ACTIVE_LOW);

		fclose(lfh);
		free(lfn);
		
		return 0;
	}
	return 1;
}


/*	
	Device abstractions in this file are as follows
	-LED		  	(on/off/toggle)
	-3-way Switch 	(getPosition) (on-off-on)
	-Photoresistor	(read)
*/

struct LED{
	PIN * p;
	bool on; // This led is on, true/false
};

struct TWS{
	PIN * p1;
	PIN * p2;
};

struct PHR{
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

int RPi_LED_on(LED * l){
	if(l==NULL || l->p==NULL){
		LASTERR=4;
		return 1;
	}
	if(0==RPi_pwrite(l->p,true)){
		l->on=true;
		return 0;
	}
	return 1;
}

int RPi_LED_off(LED * l){
	if(l==NULL || l->p==NULL){
		LASTERR=4;
		return 1;
	}
	if(0==RPi_pwrite(l->p,false)){
		l->on=false;
		return 0;
	}
	return 1;
}

int RPi_LED_toggle(LED * l){
	if(l==NULL || l->p==NULL){
		LASTERR=4;
		return 1;
	}
	if(l->on){
		if(0==RPi_pwrite(l->p,false)){
			l->on=false;
			return 0;
		}
		return 1;
	}
	if(0==RPi_pwrite(l->p,true)){
		l->on=true;
		return 0;
	}
	return 1;
}

int RPi_LED_close(LED * l){
	if(l==NULL || l->p==NULL){
		LASTERR=4;
		return 1;
	}
	if(0!=RPi_pclose(l->p)){
		return 1;
	}
	free(l);
	return 0;
}

TWS * RPi_TWS_open(int pin1, int pin2){
	TWS * newTWS = malloc(sizeof(struct TWS));
	if (newTWS == NULL){
		LASTERR=8;
		return NULL;
	}
	newTWS->p1 = RPi_popen(pin1,ACTIVE_HIGH,IN);
	if(newTWS->p1==NULL){
		free(newTWS);
		return NULL;
	}
	newTWS->p2 = RPi_popen(pin2,ACTIVE_HIGH,IN);
	if(newTWS->p2==NULL){
		free(newTWS);
		return NULL;
	}
	return newTWS;
}

int RPi_TWS_readPosition(TWS * s, int * readto){
	if(s==NULL || s->p1==NULL || s->p2==NULL || readto==NULL){
		LASTERR=4;
		return 1;
	}

	bool pin1, pin2;
	if (0==RPi_pread(s->p1, &pin1) && 0==RPi_pread(s->p2, &pin2)){
		if(pin1){
			*readto = 1;
		}
		else if(pin2){
			*readto = 2;
		}
		else{
			*readto = 0;
		}
		return 0;
	}
	return 1;
}

int RPi_TWS_close(TWS * s){
	if(s==NULL || s->p1==NULL || s->p2==NULL){
		LASTERR=4;
		return 1;
	}
	if(0==RPi_pclose(s->p1) && 0==RPi_pclose(s->p2)){
		free(s);
		return 0;
	}
	return 1;
}

PHR * RPi_PHR_open(int pin){
	PHR * newPHR = malloc(sizeof(struct PHR));
	if(newPHR==NULL){
		LASTERR=9;
		return NULL;
	}
	newPHR->p = RPi_popen(pin,ACTIVE_HIGH,IN);
	if(newPHR->p==NULL){
		free(newPHR);
		return NULL;
	}
	return newPHR;
}

int RPi_PHR_read(PHR * r, bool * readto){
	if(r==NULL || r->p==NULL){
		LASTERR=4;
		return 1;
	}
	return RPi_pread(r->p,readto);
}

int RPi_PHR_close(PHR * r){
	if(r==NULL || r->p==NULL){
		LASTERR=4;
		return 1;
	}
	if(0==RPi_pclose(r->p)){
		free(r);
		return 0;
	}
	return 1;
}

int RPi_errorno(){
	return LASTERR;
}

char * RPi_errorstr(int err){
	switch(err){
		case 1:
			return "Unable to create PIN, memory not available.";
		case 2:
			return "Unable to interact with board, permissions are probably set up incorrectly.";
		case 3:
			return "Invalid pin number.";
		case 4:
			return "Recieved an argument that was NULL.";
		case 5:
			return "Attempt to write to an input only pin.";
		case 6:
			return "Attempt to read from a write only pin.";
		case 7:
			return "Unable to create LED, memory not available.";
		case 8:
			return "Unable to create three-way-switch, memory not available.";
		case 9:
			return "Unable to create photoresistor, memory not available.";
		default:
			return "Unknown Error.";
		break;
	}
}
