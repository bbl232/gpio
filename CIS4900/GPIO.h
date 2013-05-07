/*GPIO Library for the Raspberry Pi
Written by Bill Vandenberk

Description: this is a library for use with the Raspberry Pi's GPIO controller
*/
#ifndef __GPIO_H
#define __GPIO_H

/*Boolean type definitions*/
#define true 1
#define TRUE 1
#define false 0
#define FALSE 0
#define undef -1 /*undefined boolean values*/
#define UNDEF -1 /*undefined boolean values*/
typedef int boolean;

/*Base directory for the GPIO controller fs
DEFAULT: /sys/class/gpio/
*/
static const char const * GPIODIR = "/sys/class/gpio/";
static const char const * GPIOEX = "/sys/class/gpio/export";
static const char const * GPIOUNEX = "/sys/class/gpio/unexport";

enum direction{
	IN,
	OUT
};

enum logicType{
	ACTIVE_HIGH,
	ACTIVE_LOW
};

typedef struct pin PIN;

boolean getValue (int pinNum); /*Exports the pin, sets the direction to in, gets the value, unexports the pin and returns the value*/
boolean setValue (int pinNum, boolean value); /*Exports the pin, sets the direction to out, and sets the value to 0 or 1; NOTE: This function leaves the pin exported*/

int setValues (boolean value, ...); /*Set multiple pins on or off, end the list with a negative number*/

PIN * exportPin(int number, enum direction dir, enum logicType active); /*Creates pin structure and exports the pin in sysfs*/
boolean unexportPin(PIN * p); /*Unexports the pin using sysfs and destroys the struct*/

boolean getPinValue(PIN * p); /*Gets the value for a previously exported pin*/
boolean setPinValue(PIN * p, boolean value); /*Sets the value for a previously exported pin*/

boolean setPinDirection(PIN * p, enum direction dir); /*Sets the direction of a previously eported pin*/
enum direction getPinDirection(PIN * p); /*Gets the direction of a previously exported pin*/

boolean setPinActive(PIN * p, enum logicType active); /*Sets the logic type of a previously exported pin*/
enum logicType getPinActive(PIN * p); /*Gets the logic type of a previously exported pin*/

int getPinLocation (PIN * p); /*Gets the location of a previously exported pin*/

void swapPins(int pin1, int pin2); /*Used to make the library remap pins (by swapping them)*/

void printPinMap();
#endif
