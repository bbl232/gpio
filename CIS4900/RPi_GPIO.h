/*GPIO Library for the Raspberry Pi
Written by Bill Vandenberk for CIS4900

Judi McCuaig
Bill Gardner

Description: this is a library for use with the Raspberry Pi's GPIO controller, intended for use with the University of Guelph's CIS 1500 Fall 13 students

Requirements:
	High Level Functionality 
	Easy to Use (include, library linking)
	Easy to Work With (function calls need to be simple)
	Clean (No crashes, Confirmed results)
	Easy to Read Examples (Enumerated types help with this)
	Compatibility with components on the board for use with the class
	Compatibility with both revisions of the rpi
*/

#ifndef __RPi_GPIO_H
#define __RPi_GPIO_H
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
typedef struct LED LED;
typedef struct TWS TWS;
typedef struct PHR PHR;

/* int RPi_init ()

	This function will disable any active pins. It is meant to be run at the beginning (and possibly end) of each program that uses this library

	Parameters: none
	Returns: 0 on success
*/
int RPi_init ();


/* PIN * RPi_popen(int number, enum RPi_logicType logic, enum RPi_direction direc)

	This function will create a pin struct and export a pin, setting its direction and logic type

	Parameters: int number - an integer in the range 0-16 representing a pin on the pinmap, enum RPi_logicType logic - logic type to apply to this pin, enum RPi_direction direc - intended direction of pin
	Returns: PIN, the pin struct
*/
PIN * RPi_popen(int number, enum RPi_logicType logic, enum RPi_direction direc);


/* int RPi_pclose(PIN * p)

	This function destroys a pin struct and disables the given pin

	Parameters: PIN p - the pin to be destroyed
	Returns: 0 on success
*/
int RPi_pclose(PIN * p);


/* int RPi_pread(PIN * p, bool * in)

	This function will read the value of a pin that has been created

	Parameters: PIN p - the pin to be read, bool * in - the location to store the read value
	Returns: 0 on success
*/
int RPi_pread(PIN * p, bool * in);


/* int RPi_pwrite(PIN * p, bool value)

	This function will write a value to a pin that has been created

	Parameters: PIN p - the pin to be written to, bool value - the value to put on the pin
	Returns: 0 on success
*/
int RPi_pwrite(PIN * p, bool value);


/* int RPi_pdirection(PIN * p, enum RPi_direction dire)

	This function will change the direction of a pin that has already been created

	Parameters: PIN p - the pin that is to be changed, enum RPi_direction - the new direction of the pin
	Returns: 0 on success
*/
int RPi_pdirection(PIN * p, enum RPi_direction dire);


/* int RPi_pidle(PIN * p)

	This function temporarily disables a pin, NOTE: this may not change the value that is on a pin.
	The pin will be enabled should you try to read from it or write to it.

	Parameters: PIN p - the pin that is to be made idle
	Returns: 0 on success
*/
int RPi_pidle(PIN * p);


/*  +----------------------------------------------------------------+
	|																 |
	|  END ADT Functions - Begin Device Abstracion functions 		 |
	|																 |
	+----------------------------------------------------------------+
*/


/* LED * RPi_LED_open(int pin);

	This function opens a connection to an LED

	Parameters: int pin - the pin that the led is connected to
	Returns: LED *, the created LED
*/
LED * RPi_LED_open(int pin);


/* int RPi_LED_ON(LED * l);

	This function will turn an LED on

	Parameters: LED * l - the LED to turn on
	Returns: 0 on success
*/
int RPi_LED_on(LED * l);


/* int RPi_LED_OFF(LED * l);

	This function turns an LED off

	Parameters: LED * l - the LED to turn off
	Returns: 0 on success
*/
int RPi_LED_off(LED * l);


/* int RPi_LED_toggle(LED * l);

	This function will toggle the current state of an LED

	Parameters: LED * l - the LED to toggle
	Returns: 0 on success
*/
int RPi_LED_toggle(LED * l);


/* int RPi_LED_close(LED * l);

	This function closes the connection to an LED

	Parameters: LED * l - the LED to close
	Returns: 0 on success
*/
int RPi_LED_close(LED * l);



TWS * RPi_TWS_open(int pin1, int pin2){

}

int RPi_TWS_readPosition(TWS * s, int * readto){

}

int RPi_TWS_close(TWS * s){

}

PHR * RPi_PHR_open(int pin){

}

int RPi_PHR_read(PHR * r, int * readto){

}

int RPi_PHR_close(PHR * r){
	
}


/*  +----------------------------------------------------------------+
	|																 |
	|  END Device Abstraction Functions - Begin error msg functions  |
	|																 |
	+----------------------------------------------------------------+
*/


/* int RPi_errorno()

	This function can be used to get the error number of the last funciton call (if a function does not return 0, you can use this function to see what went wrong.)

	Parameters: none.
	Returns: the last error number
*/
int RPi_errorno();


/* char * RPi_errorstr(int err)

	This function will return an english redable explanation of an errorno code.

	Parameters: int err - the error code as gotten from errorno()
	Returns" char *, a string containing information about the error
*/
char * RPi_errorstr(int err);


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

