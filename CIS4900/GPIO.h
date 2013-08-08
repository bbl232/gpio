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
#define RPi_board_revision 2
#include <stdbool.h>

/*#define ADTLowLevelFunctions*/

enum direction{
	IN,
	OUT
};

enum logicType{
	ACTIVE_HIGH,
	ACTIVE_LOW
};

typedef struct pin PIN;

/* PIN PinOn(int number)

	This function will create a pin struct and export a pin, setting its direction to in and logic type to active high

	Parameters: int number - an integer in the range 0-16 representing a pin on the pinmap
	Returns: PIN, the pin struct
*/
PIN* PinOn(int pinNum);


/* bool PinOff(PIN p)

	This function destroys a pin struct and unexports the pin

	Parameters: PIN p - the pin to be destroyed
	Returns: bool, the result of the attempted destory
*/
bool PinOff(PIN* p);


/* bool Read(PIN p)

	This function will read the value of a pin that has been created

	Parameters: PIN p - the pin to be read
	Returns: bool, the value on the pin
*/
bool Read(PIN* p);


/* bool Write(PIN p, bool value)

	This function will write a value to a pin that has been created

	Parameters: PIN p - the pin to be written to, bool value - the value to put on the pin
	Returns: bool, the result of the write attempt
*/
bool Write(PIN* p, bool value);


/* bool SetLogic(PIN p, enum logicType);

	This function will change the logic type of a pin that has been created

	Parameters: PIN p - the pin that is to be changed, enum logicType - the new logic type of the pin
	Returns: bool, the result of setting the logic type
*/
bool SetLogic(PIN* p, enum logicType);


/* bool SetDirection(PIN p, enum direction)

	This function will change the direction of a pin that has already been created

	Parameters: PIN p - the pin that is to be changed, enum direction - the new direction of the pin
	Returns: bool, the result of setting the direction
*/
bool SetDirection(PIN* p, enum direction);


/* int PinLocation(PIN p)

	This function provides a way for you to get the location of a pin that you created

	Parameters: PIN p - the pin to check
	Returns: int, an integer in the range 0-16 representing a pin on the pinmap
*/
int PinLocation(PIN* p);

/*Pin Map Below
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

