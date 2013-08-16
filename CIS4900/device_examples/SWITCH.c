/* This is a test file for the RPi_GPIO library written by Bill Vandenberk
Date: August 13, 2013
Purpose: Show how to use the three-way-switch abstraction (TWS).

To compile this program use this command:
gcc SWITCH.c -o switch -lRPi_GPIO
*/

/*First, we need access to the library*/
#include <RPi_GPIO.h>
/*Second, lets get some i/o access*/
#include <stdio.h>

int main (){
	/*We need to call this function at the beginning of all out programs that use the library*/
	RPi_init();
	/*These are the pins that the switch leads are hooked up to.
	These can change depending on how everything is wired up*/
	int pin1 = 5;
	int pin2 = 13;

	TWS * sw = RPi_TWS_open(pin1,pin2);
	if(sw==NULL){
		printf("error: %s\n",RPi_errorstr(RPi_errorno()));
	}

	int position=-1;
	while(position!=2){
		if(0==RPi_TWS_readPosition(sw, &position)){
			printf("Position: %d\n",position);
		}
		else{
			break;
		}
	}
	printf("error: %s\n",RPi_errorstr(RPi_errorno()));

	if(0!=RPi_TWS_close(sw)){
		printf("error: %s\n",RPi_errorstr(RPi_errorno()));
	}

	return 0;
}