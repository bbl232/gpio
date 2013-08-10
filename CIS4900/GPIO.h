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
/*Uncomment the next line if you have a revision one board (the board that has no mounting holes on it)*/
#define RPi_board_rev_1
#include <stdbool.h>

/*#define ADTLowLevelFunctions*/

enum RPi_direction{
	IN,
	OUT,
	INOUT
};

enum RPi_logicType{
	ACTIVE_HIGH,
	ACTIVE_LOW
};

typedef struct pin PIN;


int RPi_init ();

/* PIN PinOn(int number)

	This function will create a pin struct and export a pin, setting its direction to in and logic type to active high

	Parameters: int number - an integer in the range 0-16 representing a pin on the pinmap
	Returns: PIN, the pin struct
*/
PIN * RPi_popen(int number, enum RPi_logicType logic, enum RPi_direction direc);

/* bool PinOff(PIN p)

	This function destroys a pin struct and unexports the pin

	Parameters: PIN p - the pin to be destroyed
	Returns: bool, the result of the attempted destory
*/
int RPi_pclose(PIN * p);

/* bool Read(PIN p)

	This function will read the value of a pin that has been created

	Parameters: PIN p - the pin to be read
	Returns: bool, the value on the pin
*/
int RPi_pread(PIN * p, bool * in);

/* bool Write(PIN p, bool value)

	This function will write a value to a pin that has been created

	Parameters: PIN p - the pin to be written to, bool value - the value to put on the pin
	Returns: bool, the result of the write attempt
*/
int RPi_pwrite(PIN * p, bool value);


/* bool SetDirection(PIN p, enum direction)

	This function will change the direction of a pin that has already been created

	Parameters: PIN p - the pin that is to be changed, enum direction - the new direction of the pin
	Returns: bool, the result of setting the direction
*/
int RPi_pdirection(PIN * p, enum RPi_direction dire);

int RPi_pidle(PIN * p);
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

