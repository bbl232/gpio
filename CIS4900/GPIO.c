/*
TODO:
Change function names to something more logical incl gpio_ prefix
set up pin idle open/close
set up in out inout logic
set up errorno and errorstr functions
change read to work like stdio (scanf with bool pointer)

*/





#include "GPIO.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

bool exported[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; /*Keeps track of which pins are exported*/

#if RPi_board_revision == 1
    int hardPin[17] = {0,1,4,17,21,22,10,9,11,7,8,25,24,23,18,15,14}; /*REV1:Maps the soft pins to the hardware pins.*/
#else
    int hardPin[17] = {2,3,4,17,27,22,10,9,11,7,8,25,24,23,18,15,14}; /*REV2:Maps the soft pins to the hardware pins.*/
#endif

/*Base directory for the GPIO controller fs
DEFAULT: /sys/class/gpio/
*/
static const char const * GPIODIR = "/sys/class/gpio/";
static const char const * GPIOEX = "/sys/class/gpio/export";
static const char const * GPIOUNEX = "/sys/class/gpio/unexport";

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

/*High Level GPIO stuff, basic get/set THESE PINS ARE ASSUMED ACTIVE HIGH
 *Functions: Read, Write, Multi-Read, Map Printing, Mapping
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
			//"ERROR HANDLING HERE"
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
	fprintf(stderr,"EXPORT ERROR: Given PIN: %d could not be exported.\n",number);
	return NULL;
}

int RPi_pclose(PIN * p){
	if(p != NULL && RPi__unexport(p->location)){
		free(p);
		return 0;
	}
	return 1;
} 

int RPi_pread(PIN * p, bool * in){
	if(p != NULL){
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
			fprintf(stderr,"READ ERROR: Given PIN is not an input pin.\n");
			return 1;
		}
	}
	fprintf(stderr,"ERROR: Can't read NULL pin.\n");
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
			fprintf(stderr,"ERROR not an output pin.\n");
			return 1;
		}
	}
	fprintf(stderr,"ERROR: Can't read NULL pin.\n");
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
	fprintf(stderr,"ERROR: Could not adjust direction.\n");
	return 1;
}


/* End ADT Functions
 *
 */


int RPi__getValue (int pinNum, bool * value){
	if(0==RPi__export(pinNum)){
		char * direfn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+13);
		sprintf(direfn,"%s%s%d/direction",GPIODIR,"gpio",hardPin[pinNum]);
		FILE * dire = fopen(direfn,"w");
		if(dire == NULL){
			return 1;
		}
		fprintf(dire,"%s","in");
		fclose(dire);
		free(direfn);

		char * fn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+9);
		sprintf(fn,"%s%s%d/value",GPIODIR,"gpio",hardPin[pinNum]);
		FILE * val = fopen(fn,"r");
		if(val == NULL){
			return 1;
		}
		int temp = 27;
		fscanf(val,"%d",&temp);
		*value = (temp!=27);
		fclose(val);
		free(fn);
		RPi__unexport(pinNum);
		return 0;
	}
	fprintf(stderr,"getValue ERROR: Unable to export pin: %d.\n",pinNum);
	return 1;
}

int RPi__setValue (int pinNum, bool value){
	if(0==RPi__export(pinNum)){
		char * direfn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+13);
		sprintf(direfn,"%s%s%d/direction",GPIODIR,"gpio",hardPin[pinNum]);
		FILE * dire = fopen(direfn,"w");
		if(dire == NULL){
			return 1;
		}
		fprintf(dire,"%s","out");
		fclose(dire);
		free(direfn);

		char * fn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+9);
		sprintf(fn,"%s%s%d/value",GPIODIR,"gpio",hardPin[pinNum]);
		FILE * val = fopen(fn,"w");
		if(val == NULL){
			free (fn);
			return 1;
		}
		fprintf(val,"%d",value);
		fclose(val);
		free(fn);
		return 0;
	}
	fprintf(stderr,"setValue ERROR: Unable to export pin: %d.\n",pinNum);
	return 1;
} 

/*This function remaps the soft pin to the hard pin and does the actual sysfs interaction with the GPIO controller in order to export a pin*/
int RPi__export(int pin){

	if(pin >= 17 || pin < 0){
		fprintf(stderr,"EXPORT ERROR: Given PIN: %d is not mapped.\n",pin);
		return 1;
	}

	if(1==exported[pin]){
		return 0;
	}

	FILE * ex = fopen(GPIOEX,"w");
	if(ex==NULL){
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
		fprintf(stderr,"EXPORT ERROR: Given PIN: %d is not mapped.\n",pin);
		return 1;
	}

	if(0==exported[pin]){
		return 0;
	}

	FILE * ex = fopen(GPIOUNEX,"w");
	if(ex==NULL){
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
	if(pin >= 17 || pin < 0){
		fprintf(stderr,"ERROR: Given PIN: %d is not mapped.\n",pin);
		return 1;
	}

	if(0==exported[pin]){
		fprintf(stderr,"ERROR: Given PIN: %d is not exported.\n",pin);
		return 1;
	}

	char * direfn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+13);
	sprintf(direfn,"%s%s%d/direction",GPIODIR,"gpio",hardPin[pin]);
	FILE * direfh = fopen(direfn,"w");
	if(direfh == NULL){
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

int RPi__logic(int pin, enum RPi_logicType logic){
	if(pin >= 17 || pin < 0){
		fprintf(stderr,"ERROR: Given PIN: %d is not mapped.\n",pin);
		return 1;
	}

	if(0==exported[pin]){
		fprintf(stderr,"ERROR: Given PIN: %d is not exported.\n",pin);
		return 1;
	}

	char * lfn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+14);
	sprintf(lfn,"%s%s%d/active_low",GPIODIR,"gpio",hardPin[pin]);
	FILE * lfh = fopen(lfn,"w");
	if(lfh == NULL){
		return 1;
	}

	fprintf(lfh,"%d", logic == ACTIVE_LOW);

	fclose(lfh);
	free(lfn);
	
	return 0;
}

