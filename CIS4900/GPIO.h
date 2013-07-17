/*GPIO Library for the Raspberry Pi
Written by Bill Vandenberk for CIS4900

Judi McCuaig
Bill Gardner

Description: this is a library for use with the Raspberry Pi's GPIO controller, intended for use with the University of Guelph's CIS 1500 Fall 13 students

Requirements:
	High and Low Level Functionality (Easy to use one-off functions, PIN ADT)
	Pin Mapping and Remapping
	Easy to Use (include, library linking)
	Easy to Work With (function calls need to be simple)
	Clean (No crashes, Confirmed results)
	Easy to Read Examples (Enumerated types help with this)
	Compatibility with components on the board for use with the class
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

#define ADTLowLevelFunctions

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

/*High Level Functions - NON ADT*/

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
int setValues (boolean value, ...);


/* boolean cleanUp(int pinNum)

	This function will clean up from a pin that you used setValue on

	Parameters: int pinNum - an integer in the range 0-16 representing a pin on the pinmap
	Returns: boolean, the result of cleaning up the pin
*/
boolean cleanUp (int pinNum);


/* void swapPins(int pin1, int pin2)

	This function will swap pin number mappings of unexported pins

	Parameters: int pin1 - the first pin to swap, int pin2 - the second pin to swap
	Returns: void
*/
void swapPins(int pin1, int pin2);


/* void printPinMap()

	This function prints the current pin map to the screen. It takes into account any pins you may have swapped

	Parameters: none
	Returns: none
*/
void printPinMap();


/*Low Level Functions - GPIO Pin ADT*/

/* PIN PinOn(int number)

	This function will create a pin struct and export a pin, setting its direction to in and logic type to active high

	Parameters: int number - an integer in the range 0-16 representing a pin on the pinmap
	Returns: PIN, the pin struct
*/
PIN PinOn(int pinNum);


/* boolean PinOff(PIN p)

	This function destroys a pin struct and unexports the pin

	Parameters: PIN p - the pin to be destroyed
	Returns: boolean, the result of the attempted destory
*/
boolean PinOff(PIN p);


/* boolean Read(PIN p)

	This function will read the value of a pin that has been created

	Parameters: PIN p - the pin to be read
	Returns: boolean, the value on the pin
*/
boolean Read(PIN p);


/* boolean Write(PIN p, boolean value)

	This function will write a value to a pin that has been created

	Parameters: PIN p - the pin to be written to, boolean value - the value to put on the pin
	Returns: boolean, the result of the write attempt
*/
boolean Write(PIN p, boolean value);


/* boolean SetLogic(PIN p, enum logicType);

	This function will change the logic type of a pin that has been created

	Parameters: PIN p - the pin that is to be changed, enum logicType - the new logic type of the pin
	Returns: boolean, the result of setting the logic type
*/
boolean SetLogic(PIN p, enum logicType);


/* boolean SetDirection(PIN p, enum direction)

	This function will change the direction of a pin that has already been created

	Parameters: PIN p - the pin that is to be changed, enum direction - the new direction of the pin
	Returns: boolean, the result of setting the direction
*/
boolean SetDirection(PIN p, enum direction);


/* int PinLocation(PIN p)

	This function provides a way for you to get the location of a pin that you created

	Parameters: PIN p - the pin to check
	Returns: int, an integer in the range 0-16 representing a pin on the pinmap
*/
int PinLocation(PIN p);

/*Default Pin Map Below
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
#endif

