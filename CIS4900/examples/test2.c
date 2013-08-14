/* This is a test file for the RPi_GPIO library written by Bill Vandenberk
Date: August 13, 2013
Purpose: Show how to use the pin ADT to control some LEDs.
NOTE: This does not use the LED device abstraction.
It is meant to show the difference between the abstraction and doing it manually.

*/

/*First, we need access to the library*/
#include <RPi_GPIO.h>
/*Second, lets get some i/o access*/
#include <stdio.h>

int main (){
	/*We need to call this function at the beginning of all out programs that use the library*/
	RPi_init();
	/*These are the pins that the LEDs are hooked up to.
	These can change depending on how everything is wired up*/
	int pin1 = 9;
	int pin2 = 10;

	/*These variables are how we will interact with those pins.
	They are active high becuase when there is voltage on the pins the LEDs are on.
	They are strictly input pins so that any attempt to read from them will result in an error*/
	PIN * led1 = RPi_popen(pin1,ACTIVE_HIGH,OUT);
	
	/*We should check that these pins actually got created before we start using them.*/
	if(led1==NULL){
		printf("Error creating pin: %s\n",RPi_errorstr(RPi_errorno()));
		return 1;
	}
	
	PIN * led2 = RPi_popen(pin2,ACTIVE_HIGH,OUT);
	/*Again, check to make sure the pin was created.*/
	if(led2==NULL){
		printf("Error creating pin: %s\n",RPi_errorstr(RPi_errorno()));
		return 1;
	}
	
	/*If this function does not return 0, we know something has gone wrong.
	We can look up what happened.*/
	if(0!=RPi_pwrite(led1,true)){
		printf("Error writing to pin: %s\n",RPi_errorstr(RPi_errorno()));
	}

	/*This prints a message to the screen and waits for any character input, the input is ignored.*/
	printf("Press enter to toggle the LEDs\n");
	getchar();

    if(0!=RPi_pwrite(led2,true)){
        printf("Error writing to pin: %s\n",RPi_errorstr(RPi_errorno()));
    }
	
    if(0!=RPi_pwrite(led1,false)){
        printf("Error writing to pin: %s\n",RPi_errorstr(RPi_errorno()));
    }

	printf("Press enter to terminate\n");
	getchar();


    if(0!=RPi_pwrite(led2,false)){
        printf("Error writing to pin: %s\n",RPi_errorstr(RPi_errorno()));
    }

	if (0!=RPi_pclose(led1)){
		printf("Error closing pin: %s\n",RPi_errorstr(RPi_errorno()));
	}
	if (0!=RPi_pclose(led2)){
		printf("Error closing pin: %s\n",RPi_errorstr(RPi_errorno()));
	}
	return 0;
}
