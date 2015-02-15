/*GPIO library for the Raspberry Pi
Written by Bill Vandenberk for CIS4900, Summer 2013

Judi McCuaig
Bill Gardner

Description: this is a library for use with the Raspberry Pi's GPIO controller.
Intended for use with the University of Guelph's CIS students
*/

#ifndef __RPi_GPIO_H
#define __RPi_GPIO_H
#include <stdbool.h>

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
typedef struct ADC ADC;
typedef struct TWS TWS;
typedef struct PHR PHR;

/* int RPi_init ()

    This function is meant to be run at the beginning of each program that uses the library.
    It will prepare the interface required for the library to work.

    Preconditions: the function has not been called before
    Postconditions: all 17 pins are prepared for use with the library

    Parameters: none
    Returns: 0 on success, 1 on failure - use RPi_errorno and RPi_errorstr to get more details on the failure
*/
int RPi_init ();

/*  +----------------------------------------------------------------+
    |                                                                |
    |  START ADT Functions                                           |
    |                                                                |
    +----------------------------------------------------------------+
*/

/* PIN * RPi_popen(int number, enum RPi_logicType logic, enum RPi_direction direc)

    This function will make and return a PIN. 
    This needs to be called to read from or write to a pin.
    Note: This funtion mallocs memory which is freed when you call RPi_pclose.

    Preconditions: the specified pin is not opened yet
    Postconditions: the pin is created and returned

    Parameters: int number - an integer in the range 0-16 representing a pin on the pinmap
                enum RPi_logicType logic - logic type to apply to this pin
                enum RPi_direction direc - intended direction of pin
    Returns: PIN, the pin struct on success, NULL on failure - use RPi_errorno and RPi_errorstr to get more information about the failure
*/
PIN * RPi_popen(int number, enum RPi_logicType logic, enum RPi_direction direc);


/* int RPi_pclose(PIN * p)

    This function destroys a PIN and frees the memory affiliated with it.

    Preconditions: the specified pin is open
    Postconditions: the specified pin is closed

    Parameters: PIN p - the pin to be destroyed
    Returns: 0 on success, 1 on failure - use RPi_errorno and RPi_errorstr to get more details on the failure
*/
int RPi_pclose(PIN * p);


/* int RPi_pread(PIN * p, bool * in)

    This function will read the value on an input or bidirectional pin.

    Preconditions: the pin is open and either an in or inout pin
    Postconditions: the value on the pin is read and the value is placed in bool * in

    Parameters: PIN p - the pin to be read
                bool * in - the location to store the read value
    Returns: 0 on success, 1 on failure - use RPi_errorno and RPi_errorstr to get more details on the failure
*/
int RPi_pread(PIN * p, bool * in);


/* int RPi_pwrite(PIN * p, bool value)

    This function will put a value on an output or bidirectional pin.

    Preconditions: the pin is open and either an out or inout pin
    Postconditions: the given value is placed on the pin

    Parameters: PIN p - the pin to be written to
                bool value - the value to put on the pin
    Returns: 0 on success, 1 on failure - use RPi_errorno and RPi_errorstr to get more details on the failure
*/
int RPi_pwrite(PIN * p, bool value);


/* int RPi_pdirection(PIN * p, enum RPi_direction dire)

    This function will change the direction of a pin.

    Preconditions: the pin is open
    Postconditions: the pins direction is changed to the given one.

    Parameters: PIN p - the pin that is to be changed
                enum RPi_direction - the new direction of the pin
    Returns: 0 on success, 1 on failure - use RPi_errorno and RPi_errorstr to get more details on the failure
*/
int RPi_pdirection(PIN * p, enum RPi_direction dire);


/*  +----------------------------------------------------------------+
    |                                                                |
    |  END ADT Functions - Begin Device Abstracion functions         |
    |                                                                |
    +----------------------------------------------------------------+
*/


/* ADC * RPi_ADC_open()

    This function opens a connection to the ADC on the SPI bus, This needs to be calles before you can read from the ADC. Note: this function mallocs memory that is freed when you call RPi_ADC_close
*/

ADC * RPi_ADC_open();

/* int * RPi_ADC_read(ADC * adc)

    This function reads both channels of the ADC (MCP3002) and stores the values in the ADC struct.

    Preconditions: The RPi_ADC_open function has been called and adc is not null.
    Postconditions: The values read from the ADC cannels (0-1024) are placed inside the ADC struct.
    Returns: 1 on error, 0 on sucess
*/

int RPi_ADC_read(ADC * adc);

/* ADC * RPi_ADC_close(ADC * adc)

    This function closes the connection to the ADC and frees all memore associated with it.

    Preconditions: The RPi_ADC_open function has been called and adc is not null.
    Postconditions: The memory associated with the adc struct is freed and can no longer be used.
    Returns: 1 on error, 0 on sucess
*/

int RPi_ADC_close(ADC * adc);

/* LED * RPi_LED_open(int pin);

    This function opens a connection to an LED. This needs to be called before you can turn the LED on or off. Note: this function mallocs memory that is freed when you call RPi_LED_close

    Preconditions: the pin that the LED is connected to is not in use already
    Postconditions: an led is created and returned

    Parameters: int pin - the pin that the led is connected to
    Returns: LED *, the created LED
*/
LED * RPi_LED_open(int pin);


/* int RPi_LED_on(LED * l);

    This function will turn an LED on.

    Preconditions: the led is open
    Postconditions: the led is turned on

    Parameters: LED * l - the LED to turn on
    Returns: 0 on success, 1 on failure - use RPi_errorno and RPi_errorstr to get more details on the failure
*/
int RPi_LED_on(LED * l);


/* int RPi_LED_off(LED * l);

    This function turns an LED off.

    Preconditions: the led is open
    Postconditions: the led is turned off

    Parameters: LED * l - the LED to turn off
    Returns: 0 on success, 1 on failure - use RPi_errorno and RPi_errorstr to get more details on the failure
*/
int RPi_LED_off(LED * l);


/* int RPi_LED_toggle(LED * l);

    This function will toggle the current state of an LED.

    Preconditions: the led is open
    Postconditions: the led is turned on if it was previously off and off if it was previously on

    Parameters: LED * l - the LED to toggle
    Returns: 0 on success, 1 on failure - use RPi_errorno and RPi_errorstr to get more details on the failure
*/
int RPi_LED_toggle(LED * l);


/* int RPi_LED_close(LED * l);

    This function closes the connection to an LED. This also frees all memory associated with the LED.

    Preconditions: the led is open
    Postconditions: the led is closed and the memory is freed

    Parameters: LED * l - the LED to close
    Returns: 0 on success, 1 on failure - use RPi_errorno and RPi_errorstr to get more details on the failure
*/
int RPi_LED_close(LED * l);


/* TWS * RPi_TWS_open(int pin1, int pin2);

    This function creates a connection to a three way switch. Note: this function mallocs memory that is freed when you call RPi_TWS_close

    Preconditions: the pins that are being used for the 3-way-switch are not already in use
    Postconditions: the 3-way-switch is created and returned

    Parameters: int pin1 - the first input pin (also connected to ground via a resistor),
                int pin2 - the second input pin (also connected to ground via a resistor)
    Returns: a three-way-switch on success, NULL on failure
*/
TWS * RPi_TWS_open(int pin1, int pin2);


/* int RPi_TWS_readPosition(TWS * s, int * readto);

    This function reads the position of a 3-way-switch

    Preconditions: the tws is open
    Postconditions: the position of the tws is placed in int * readto

    Parameters: TWS * s - the switch to get the position for, 
                                int * readto - the int to read the position into
    Returns: 0 on success, 1 on failure - use RPi_errorno and RPi_errorstr to get more details on the failure
*/
int RPi_TWS_readPosition(TWS * s, int * readto);


/* int RPi_TWS_close(TWS * s);

    This function will close a three way switch connection

    Preconditions: the tws is open
    Postconditions: the tws is closed and the memory related to it is freed

    Parameters: TWS * s - the switch to close
    Returns: 0 on success, 1 on failure - use RPi_errorno and RPi_errorstr to get more details on the failure
*/
int RPi_TWS_close(TWS * s);


/* PHR * RPi_PHR_open(int pin);

    This function creates a photoresistor

    Preconditions: the pins for the photoresistor are not already in use
    Postconditions: the photoresistor is created and returned

    Parameters: int pin - the pin number that the photo resistor is hooked up to
    Returns: PHR *, the photoresistor
*/
PHR * RPi_PHR_open(int pin);


/* int RPi_PHR_read(PHR * r, bool * readto);

    This function reads the photoresistor. False value means it is dark, true value means it is bright

    Preconditions: the phr is open
    Postconditions: the value on the phr is placed in bool * readto

    Parameters: PHR * r - the resistor to read from, bool * readto - the boolean to put the value into
    Returns: 0 on success
*/
int RPi_PHR_read(PHR * r, bool * readto);


/* int RPi_PHR_close(PHR * r);

    This function closes the connection to a photoresistor

    Preconditions: the phr is open
    Postconditions: the phr is closed and memory related to it is freed

    Parameters: PHR * r - the photoresistor you are no longer using
    Returns: 0 on success, 1 on failure - use RPi_errorno and RPi_errorstr to get more details on the failure
*/
int RPi_PHR_close(PHR * r);


/*  +----------------------------------------------------------------+
    |                                                                |
    |  END Device Abstraction Functions - Begin error msg functions  |
    |                                                                |
    +----------------------------------------------------------------+
*/


/* int RPi_errorno()

    This function can be used to get the error number of the last funciton call (if a function does not return 0, you can use this function to see what went wrong.)

    Preconditions: there was an error within the library
    Postconditions: the last error code is returned

    Parameters: none.
    Returns: the last error number
*/
int RPi_errorno();


/* char * RPi_errorstr(int err)

    This function will return an english readable explanation of an errorno code.

    Preconditions: none
    Postconditions: the string affiliated with the error code is returned

    Parameters: int err - the error code as gotten from errorno()
    Returns" char *, a string containing information about the error
*/
char * RPi_errorstr(int err);
#endif

