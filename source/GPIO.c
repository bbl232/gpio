/*GPIO Library for the Raspberry Pi
Written by Bill Vandenberk for CIS4900, Summer 2013

Judi McCuaig
Bill Gardner

Description: this is a library for use with the Raspberry Pi's GPIO controller.
Intended for use with the University of Guelph's CIS students
*/

/*Uncomment the next line if you have a revision one board (the board that has no mounting holes on it)*/
/*#define RPi_board_rev_1*/

#include "RPi_GPIO.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

/*Where the last error code is stored*/
static int LASTERR;

/*Base directory for the GPIO controller fs
DEFAULT: /sys/class/gpio/
*/
static const char const * GPIODIR = "/sys/class/gpio/";
static const char const * GPIOEX = "/sys/class/gpio/export";
static const char const * GPIOUNEX = "/sys/class/gpio/unexport";

static bool exported[17] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}; /*Keeps track of which pins are exported*/

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

static int RPi__export(int pin);
static int RPi__unexport(int pin);
static int RPi__direction(int pin, enum RPi_direction dire);
static int RPi__logic(int pin, enum RPi_logicType logic);
static int RPi__getValue (int pinNum, bool * value);
static int RPi__setValue (int pinNum, bool value);

/*NON_ADT functions*/

/*
    RPi_init - this function cleans up the sysfs driver for the gpio controller for use with the library
*/
int RPi_init (){
    int i = 0;
    bool success = true; /*This is a flag to keep track of success or failure for this function*/
    for(;i<17;i++){
        if(0!=RPi__unexport(i)){
            success=false;
        }
    }
    if(success){
        return 0;
    }
    else{
        return 1;
    }
}

/*
    Low Level GPIO ADT... pin management, etc
    Functions: PinOn(Create), PinOff(Destroy), Read, Write, SetLogic, SetDirection
*/

/*
    RPi_popen - this function opens a pin, mallocs memory for it and returns it after setting up the appropriate configuration options
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

/*
    RPi_pclose - this function frees memory associated with a pin and closes that pin
*/
int RPi_pclose(PIN * p){
    if(p != NULL && 0==RPi__unexport(p->location)){
        free(p);
        return 0;
    }
    return 1;
} 

/*
    RPi_pread - this function reads the logic value on a pin
*/
int RPi_pread(PIN * p, bool * in){
    if(p != NULL && in != NULL){
        if(p->dire==IN){ /*The given pin needs to be an input or bidirectional pin */
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

/*
    RPi_pwrite - this function places a logic value on a pin
*/
int RPi_pwrite(PIN * p, bool value){
    if(p != NULL){
        if(p->dire==OUT){ /*The given pin needs to be an output or bidirectional pin*/
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

/*
    RPi_pdirection - this pin updates the directionality of a pin
*/
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

/* End ADT Functions */

/*
    RPi_getValue - this function this function gets the value from a pin from any given integer
*/
static int RPi__getValue (int pinNum, bool * value){
    char * fn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+9);
    sprintf(fn,"%s%s%d/value",GPIODIR,"gpio",hardPin[pinNum]); 
    FILE * val = fopen(fn,"r"); /*Open the value file for the given pin*/
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

/*
    RPi_setValue - this function sets the value on a pin from any given integer
*/
static int RPi__setValue (int pinNum, bool value){
    char * fn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+9);
    sprintf(fn,"%s%s%d/value",GPIODIR,"gpio",hardPin[pinNum]);
    FILE * val = fopen(fn,"w"); /*Open the value file for the given pin*/
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

/*
    RPi__export - this function makes the interface for a given pin available to use
*/
static int RPi__export(int pin){
    if(pin >= 17 || pin < 0){ /*Check to make sure the pins are in the allowable range*/
        LASTERR=3;
        return 1;
    }

    if(1==exported[pin]){ /*Check to be sure that the given pin has not already been exported*/
        LASTERR=10;
        return 1;
    }

    FILE * ex = fopen(GPIOEX,"w"); /*Open the export file*/
    if(ex==NULL){
        LASTERR=2;
        return 1;
    }
    fprintf(ex,"%d",hardPin[pin]); /*Print the hardware pin number to the file*/
    fclose(ex);
    char * check = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+4);
    sprintf(check,"%s%s%d/",GPIODIR,"gpio",hardPin[pin]);
    if(0==access(check,F_OK)){ /*Check to make sure that the export worked*/
        free(check);
        exported[pin]=true;
        return 0;
    }
    free(check);
    return 1;
}

/*
    RPi__unexport - this function makes the interface for the given pin unavailable to use
*/
static int RPi__unexport(int pin){
    if(pin >= 17 || pin < 0){ /*Check pin number against allowable range*/
        LASTERR=3;
        return 1;
    }

    if(0==exported[pin]){ /*Check to make sure that the pin in exported*/
        LASTERR=11;
        return 1;
    }

    FILE * ex = fopen(GPIOUNEX,"w"); /*open the unexport file*/
    if(ex==NULL){
        LASTERR=2;
        return 1;
    }
    fprintf(ex,"%d",hardPin[pin]); /*print the hardware pin number to the unexport file*/
    fclose(ex);
    char * check = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+4);
    sprintf(check,"%s%s%d/",GPIODIR,"gpio",hardPin[pin]);
    if(0!=access(check,F_OK)){ /*check to make sure the unexport worked*/
        free(check);
        exported[pin]=false;
        return 0;
    }
    free(check);
    return 1;
}

/*
    RPi__direction - this function sets the direction for a pin mapped to any given integer
    NOTE: The direction on the pin can only be in or out. INOUT is managed by a higher level in the library
*/
static int RPi__direction(int pin, enum RPi_direction dire){
    if(pin >= 17 || pin < 0){ /*check pin against allowable range*/
        LASTERR=3;
        return 1;
    }

    char * direfn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+13);
    sprintf(direfn,"%s%s%d/direction",GPIODIR,"gpio",hardPin[pin]);
    FILE * direfh = fopen(direfn,"w"); /*Open the pin's direction file*/
    if(direfh == NULL){
        LASTERR=2;
        return 1;
    }

    if (dire == IN){ /*Write the new direction to the file*/
        fprintf(direfh,"%s","in");
    }
    else{
        fprintf(direfh,"%s","out");
    }
    fclose(direfh);
    free(direfn);

    return 0;
}

/*
    RPi_logic - this function sets the type of logic to be used on a given pin
*/
static int RPi__logic(int pin, enum RPi_logicType logic){
    if(pin >= 17 || pin < 0){ /*check pin against allowable range*/
        LASTERR=3;
        return 1;
    }

    char * lfn = malloc(sizeof(char)*strlen(GPIODIR)+sizeof(char)*strlen("gpio")+14);
    sprintf(lfn,"%s%s%d/active_low",GPIODIR,"gpio",hardPin[pin]);
    FILE * lfh = fopen(lfn,"w"); /*open active_low file associated with the pin*/
    if(lfh == NULL){
        LASTERR=2;
        return 1;
    }

    fprintf(lfh,"%d", logic == ACTIVE_LOW); /*Write the new logic type to the file*/

    fclose(lfh);
    free(lfn);
    
    return 0;
}


/*    
    Device abstractions in this file are as follows
    -LED              (on/off/toggle)
    -3-way Switch     (getPosition) (on-off-on)
    -Photoresistor    (read)
*/

/*Light emitting diode*/
struct LED{
    PIN * p;
    bool on; // This led is on, true/false
};

/*Analog to digital convertort*/
struct ADC{
    int cOne;
    int cTwo;
}

/*Three-way-switch*/
struct TWS{
    PIN * p1;
    PIN * p2;
};

/*Photoresistor*/
struct PHR{
    PIN * p;
};

/*
    RPi_LED_open - this function opens a connection to the given pin to abstract an LED device
*/
LED * RPi_LED_open(int pin){
    LED * newLED = malloc(sizeof(struct LED)); /*We need room for the LED*/
    if (newLED == NULL){
        LASTERR=7;
        return NULL;
    }
    newLED->p = RPi_popen(pin,ACTIVE_HIGH,OUT); /*Create the pin, check the result*/
    if(newLED->p == NULL){
        free(newLED);
        return NULL;
    }
    newLED->on = false;
    if(0!=RPi_pwrite(newLED->p,false)){ /*Make sure the LED starts in the off position*/
        free(newLED->p);
        free(newLED);
        return NULL;
    }
    return newLED;
}

/*
    RPi_LED_on - this function lights up an LED
*/
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

/*
    RPi_LED_off - this function turns off an LED
*/
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

/*
    RPi_LED_toggle - this function toggles the current state of an LED
*/
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

/*
    RPi_LED_close - this closes the connection to the LED, cleans up the memory
*/
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

/*
    RPi_ADC_open - this function mallocs a struct for use with an MCP3002 (eveone ADC)
*/
ADC * RPi_ADC_open(){
    ADC * newADC = malloc(sizeof(struct ADC));
    if(newADC == NULL){
        LASTERR=12;
        return NULL;
    }
    return newADC;
}

/*
    RPi_ADC_read - this function will read both channels of an MCP3002 (eveone ADC)
*/
int RPi_ADC_read(ADC * a){
    if(a == NULL){
        LASTERR=4;
        return 1;
    }
    a->cOne = RPi__spi_read(0,0);
    a->cTwo = RPi__spi_read(1,0);
    if(a->cOne == -1 || a->cTwo == -1){
        LASTERR=13;
        return 1;
    }
    return 0;
}

/*
    RPi_ADC_close - this function will free the ADC struct
*/
int RPi_ADC_close(ADC * a){
    if(a == NULL){
        LASTERR = 4;
        return 1;
    }
    free(ADC);
    return 0;
}

/*
    RPi_TWS_open - this function opens a connection to a three way switch
*/
TWS * RPi_TWS_open(int pin1, int pin2){
    TWS * newTWS = malloc(sizeof(struct TWS)); /*Make room for the three way switch*/
    if (newTWS == NULL){
        LASTERR=8;
        return NULL;
    }
    newTWS->p1 = RPi_popen(pin1,ACTIVE_HIGH,IN); /*Create pin1, check it*/
    if(newTWS->p1==NULL){
        free(newTWS);
        return NULL;
    }
    newTWS->p2 = RPi_popen(pin2,ACTIVE_HIGH,IN);/*Create pin2, check it*/
    if(newTWS->p2==NULL){
        RPi_pclose(newTWS->p1);
        free(newTWS);
        return NULL;
    }
    return newTWS;
}

/*
    RPi_TWS_readPosition - this function reads the position of a three way switch
*/
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

/*
    RPi_TWS_close - this function closes the connection to a three way switch
*/
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

/*
    RPi_PHR_open - this function opens a connection to a photoresistor
*/
PHR * RPi_PHR_open(int pin){
    PHR * newPHR = malloc(sizeof(struct PHR)); /*Make room for the PHR*/
    if(newPHR==NULL){
        LASTERR=9;
        return NULL;
    }
    newPHR->p = RPi_popen(pin,ACTIVE_HIGH,IN); /*Create the pin*/
    if(newPHR->p==NULL){
        free(newPHR);
        return NULL;
    }
    return newPHR;
}

/*
    RPi_PHR_read - thi function reads the state of a PHR
*/
int RPi_PHR_read(PHR * r, bool * readto){
    if(r==NULL || r->p==NULL){
        LASTERR=4;
        return 1;
    }
    return RPi_pread(r->p,readto);
}

/*
    RPi_PHR_close - this function closes the connection to a phr
*/
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

/*
    RPi_errorno - This function returns the exit code of the last function called
*/
int RPi_errorno(){
    return LASTERR;
}

/*
    RPi_errorstr - This function returns a string describing an error code
*/
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
        case 10:
            return "Unable to use pin. It appears that it is already in use.";
        case 11:
            return "Unable to close pin. It appears to already be closed.";
        case 12:
            return "Unable to open ADC, memory not available."
        case 13:
            return "Unable to read ADC, is SPI enabled on this Pi? Are you using sudo?"
        case 14:
            return "Unable to close ADC, it appears to already be closed."
        default:
            return "Invalid error code.";
        break;
    }
}
