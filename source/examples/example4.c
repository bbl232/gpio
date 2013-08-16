/* This is a test file for the RPi_GPIO library written by Bill Vandenberk
Date: August 15, 2013
Purpose: Show how to use the three-way-switch abstraction (TWS).

To compile this program use this command:
gcc example4.c -o example4 -lRPi_GPIO
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

	/*We need a name for this 3-way-switch*/
	TWS * sw = RPi_TWS_open(pin1,pin2);
	/*If it was not created, we really should exit*/
	if(sw==NULL){
		printf("error: %s\n",RPi_errorstr(RPi_errorno()));
		return 1;
	}

	/*This is the variable that we're going to read the switch position into. Switch it to 2 to stop the loop*/
	int position=-1;
	/*This will repeat until the switch hits position 2*/
	while(position!=2){
		/*If this function does not return 0, we know something has gone wrong*/
		if(0==RPi_TWS_readPosition(sw, &position)){
			/*Print the poition to the screen*/
			printf("Position: %d\n",position);
		}
		else{
			printf("error: %s\n",RPi_errorstr(RPi_errorno()));
			break;
		}
	}

	/*Close the connection to the switch.*/
	if(0!=RPi_TWS_close(sw)){
		printf("error: %s\n",RPi_errorstr(RPi_errorno()));
	}

	return 0;
}