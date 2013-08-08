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
/*int pinMap[26] = {-3,-5,0,-5,1,-1,2,16,-1,15,3,14,4,-1,5,13,-3,12,6,-1,7,11,8,10,-1,9}; /*Maps all the pins including voltage and ground pins*/
/*int softPinMap[17] = {2,4,6,10,12,14,18,20,22,25,23,21,17,15,11,9,7}; /*Keeps track of soft pins position in the map above*/

/*Base directory for the GPIO controller fs
DEFAULT: /sys/class/gpio/
*/
static const char const * GPIODIR = "/sys/class/gpio/";
static const char const * GPIOEX = "/sys/class/gpio/export";
static const char const * GPIOUNEX = "/sys/class/gpio/unexport";

struct pin {
	int location;
	enum direction dire;
	enum logicType logic;
};

bool _export(int pin);
bool _unexport(int pin);
bool _direction(int pin, enum direction dire);
bool _logic(int pin, enum logicType logic);

/*High Level GPIO stuff, basic get/set THESE PINS ARE ASSUMED ACTIVE HIGH
 *Functions: Read, Write, Multi-Read, Map Printing, Mapping
 */
bool getValue (int pinNum){
	if(_export(pinNum)){
		int value = -1;
		
		char * direfn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+13);
		sprintf(direfn,"%s%s%d/direction",GPIODIR,"gpio",hardPin[pinNum]);
		FILE * dire = fopen(direfn,"w");
		if(dire == NULL){
			return undef;
		}
		fprintf(dire,"%s","in");
		fclose(dire);
		free(direfn);

		char * fn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+9);
		sprintf(fn,"%s%s%d/value",GPIODIR,"gpio",hardPin[pinNum]);
		FILE * val = fopen(fn,"r");
		if(val == NULL){
			return undef;
		}
		fscanf(val,"%d",&value);
		fclose(val);
		free(fn);
		_unexport(pinNum);
		return value;
	}
	fprintf(stderr,"getValue ERROR: Unable to export pin: %d.\n",pinNum);
	return undef;
}

bool setValue (int pinNum, bool value){
	if(_export(pinNum)){
		char * direfn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+13);
		sprintf(direfn,"%s%s%d/direction",GPIODIR,"gpio",hardPin[pinNum]);
		FILE * dire = fopen(direfn,"w");
		if(dire == NULL){
			return false;
		}
		fprintf(dire,"%s","out");
		fclose(dire);
		free(direfn);

		char * fn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+9);
		sprintf(fn,"%s%s%d/value",GPIODIR,"gpio",hardPin[pinNum]);
		FILE * val = fopen(fn,"w");
		if(val == NULL){
			free (fn);
			return false;
		}
		fprintf(val,"%d",value);
		fclose(val);
		free(fn);
		return true;
	}
	fprintf(stderr,"setValue ERROR: Unable to export pin: %d.\n",pinNum);
	return undef;
} 

int setValues (bool value, ...){
	if(value != true && value != false){
		return false;
	}
	int numSet = 0;
	va_list pins;
	va_start(pins,value);
	int pin;
	while ((pin=va_arg(pins,int))>=0){
		if(setValue(pin,value)){
			numSet++;
		}
	}
	va_end(pins);
	return numSet;
} 

bool cleanUp (int pinNum){
	return _unexport(pinNum);
}

/*Low Level GPIO ADT... pin management, etc
 *Functions: PinOn(Create), PinOff(Destroy), Read, Write, SetLogic, SetDirection
 */

PIN PinOn(int number){
	if(_export(number)){
		PIN newPin = malloc(sizeof(struct pin));
		newPin->location = number;
		newPin->dire = IN;
		newPin->logic = ACTIVE_HIGH;
		return newPin;
	}
	fprintf(stderr,"EXPORT ERROR: Given PIN: %d could not be exported.\n",number);
	return NULL;
}

bool PinOff(PIN p){
	if(p != NULL && _unexport(p->location)){
		free(p);
		return true;
	}
	return false;
} 

bool Read(PIN p){
	if(p != NULL){
		if(p->dire==IN){
			return getValue(p->location);
		}
		else{
			fprintf(stderr,"READ ERROR: Given PIN is not an input pin.\n");
			return undef;
		}
	}
	fprintf(stderr,"ERROR: Can't read NULL pin.\n");
	return undef;
}

bool Write(PIN p, bool value){
	if(p != NULL){
		if(p->dire==OUT){
			return setValue(p->location,value);
		}
		else{
			fprintf(stderr,"WRITE ERROR: Given PIN is not an output pin.\n");
			return false;
		}
	}
	fprintf(stderr,"ERROR: Can't write to NULL pin.\n");
	return false;
}

bool SetLogic(PIN p, enum logicType logic){
	if(p != NULL && _logic(p->location, logic)){
		p->logic=logic;
		return true;
	}
	fprintf(stderr,"ERROR: Could not adjust logic type.\n");
	return false;
}

bool SetDirection(PIN p, enum direction dire){
	if(p != NULL && _direction(p->location, dire)){
		p->dire=dire;
		return true;
	}
	fprintf(stderr,"ERROR: Could not adjust direction.\n");
	return false;
}

int PinLocation (PIN p){
	if(p != NULL){
		return p->location;
	}
	fprintf(stderr,"ERROR: NULL does not have a location.\n");
	return undef;
} 

/* End ADT Functions
 *
 */



/*This function remaps the soft pin to the hard pin and does the actual sysfs interaction with the GPIO controller in order to export a pin*/
bool _export(int pin){

	if(pin >= 17 || pin < 0){
		fprintf(stderr,"EXPORT ERROR: Given PIN: %d is not mapped.\n",pin);
		return false;
	}

	if(1==exported[pin]){
		return true;
	}

	FILE * ex = fopen(GPIOEX,"w");
	if(ex==NULL){
		return false;
	}
	fprintf(ex,"%d",hardPin[pin]);
	fclose(ex);
	char * check = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+4);
	sprintf(check,"%s%s%d/",GPIODIR,"gpio",hardPin[pin]);
	if(0==access(check,F_OK)){
		free(check);
		exported[pin]=true;
		return true;
	}
	free(check);
	return false;
}

/*This function remaps the soft pin to the hard pin and does the actual sysfs interaction with the GPIO controller in order to unexport a pin*/
bool _unexport(int pin){
	if(pin >= 17 || pin < 0){
		fprintf(stderr,"EXPORT ERROR: Given PIN: %d is not mapped.\n",pin);
		return false;
	}

	if(0==exported[pin]){
		fprintf(stderr,"EXPORT ERROR: Given PIN: %d is not exported.\n",pin);
		return false;
	}

	FILE * ex = fopen(GPIOUNEX,"w");
	if(ex==NULL){
		return false;
	}
	fprintf(ex,"%d",hardPin[pin]);
	fclose(ex);
	char * check = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+4);
	sprintf(check,"%s%s%d/",GPIODIR,"gpio",hardPin[pin]);
	if(0!=access(check,F_OK)){
		free(check);
		exported[pin]=false;
		return true;
	}
	free(check);
	return false;
}

bool _direction(int pin, enum direction dire){
	if(pin >= 17 || pin < 0){
		fprintf(stderr,"ERROR: Given PIN: %d is not mapped.\n",pin);
		return false;
	}

	if(0==exported[pin]){
		fprintf(stderr,"ERROR: Given PIN: %d is not exported.\n",pin);
		return false;
	}

	char * direfn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+13);
	sprintf(direfn,"%s%s%d/direction",GPIODIR,"gpio",hardPin[pin]);
	FILE * direfh = fopen(direfn,"w");
	if(direfh == NULL){
		return false;
	}

	if (dire == IN){
		fprintf(direfh,"%s","in");
	}
	else{
		fprintf(direfh,"%s","out");
	}
	fclose(direfh);
	free(direfn);

	return true;
}

bool _logic(int pin, enum logicType logic){
	if(pin >= 17 || pin < 0){
		fprintf(stderr,"ERROR: Given PIN: %d is not mapped.\n",pin);
		return false;
	}

	if(0==exported[pin]){
		fprintf(stderr,"ERROR: Given PIN: %d is not exported.\n",pin);
		return false;
	}

	char * lfn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+14);
	sprintf(lfn,"%s%s%d/active_low",GPIODIR,"gpio",hardPin[pin]);
	FILE * lfh = fopen(lfn,"w");
	if(lfh == NULL){
		return false;
	}

	fprintf(lfh,"%d", logic == ACTIVE_LOW);

	fclose(lfh);
	free(lfn);
	
	return true;
}

