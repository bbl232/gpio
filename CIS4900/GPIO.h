/*GPIO Library for the Raspberry Pi
Written by Bill Vandenberk

Description: this is a library for use with the Raspberry Pi's GPIO controller, intended for use with the University of Guelph's CIS 1500 Fall 13 students
Default Pin Map Below
LEFT(END of board)
+--+--+
|3v|5v|
+--+--+
| 0|5v|
+--+--+
| 1|GD|
+--+--+
| 2|16|
+--+--+
|GD|15|
+--+--+
| 3|14|
+--+--+
| 4|GD|
+--+--+
| 5|13|
+--+--+
|3v|12|
+--+--+
| 6|GD|
+--+--+
| 7|11|
+--+--+
| 8|10|
+--+--+
|GD| 9|
+--+--+
RIGHT(MIDDLE of board)

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

typedef struct pin * PIN;

/*Easy Functions - NON ADT*/

/* 
	boolean getValue (int pinNum)
	
	This function will attempt to set up the pin, set the pin direction to in, read the value from the pin, and clean up the pin.

	Parameters: int pinNum - an integer in the range 0-16 representing a pin on the pinmap
	Returns: boolean, the value on the pin as a boolean value
*/
boolean getValue (int pinNum);


/* boolean setValue (int pinNum, boolean value)

	This function will attempt to set up the pin, set the direction to out, and set the value on the pin. NOTE: This function does not clean up the pin.

	Parameters: int pinNum - an integer in the range 0-16 representing a pin on the pinmap, boolean value - a boolean value to place on the pin NOTE: these pins are assumed active high
	Returns: boolean, the result of the attempt
*/
boolean setValue (int pinNum, boolean value);


/* int setValues (boolean value, ...)

	This funtion sets multiple pins to one value

	Parameters: boolean value - the value to set on the pins, ... a list of integers in the range 0-16 followed by a negative number (to end the list)
	Returns: int, the number of pins that were sucessfully set
*/
int setValues (boolean value, ...); /*Set multiple pins on or off, end the list with a negative number*/

/* boolean cleanUp(int pinNum)

	This function will clean up from a pin that you used setValue on

	Parameters: int pinNum - an integer in the range 0-16 representing a pin on the pinmap
	Returns: boolean, the result of cleaning up the pin
*/
boolean cleanUp (int pinNum);

/*NOTE: DO NOT USE THIS FUNCTION AFTER USING ANY PINS.*/
void swapPins(int pin1, int pin2); /*Used to make the library remap pins (by swapping them)*/

void printPinMap();



/*GPIO Pin ADT*/
PIN PinOn(int number);
boolean PinOff(PIN p);
boolean Read(PIN p);
boolean Write(PIN p, boolean value);
boolean SetLogic(PIN p, enum logicType);
int getPinLocation (PIN p); /*Gets the location of a previously exported pin*/
#endif

