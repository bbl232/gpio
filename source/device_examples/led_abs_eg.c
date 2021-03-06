/* This is a test file for the RPi_GPIO library written by Bill Vandenberk
Date: August 15, 2013
Purpose: Show how to use the LED abstraction to control some LEDs.

To compile this program use this command:
gcc led_abs_eg.c -o led_abs_eg -lRPi_GPIO
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

	/*These variables are how we will interact with those LEDs.*/
	LED * led1 = RPi_LED_open(pin1);
	
	/*We should check that these pins actually got created before we start using them.*/
	if(led1==NULL){
		printf("Error creating LED: %s\n",RPi_errorstr(RPi_errorno()));
		return 1;
	}
	
	LED * led2 = RPi_LED_open(pin2);
	/*Again, check to make sure the pin was created.*/
	if(led2==NULL){
		printf("Error creating LED: %s\n",RPi_errorstr(RPi_errorno()));
		return 1;
	}
	
	/*If this function does not return 0, we know something has gone wrong.
	We can look up what happened.*/
	if(0!=RPi_LED_toggle(led1)){
		printf("Error: %s\n",RPi_errorstr(RPi_errorno()));
	}

	/*This prints a message to the screen and waits for any character input, the input is ignored.*/
	printf("Press enter to toggle the LEDs\n");
	getchar();

    if(0!=RPi_LED_toggle(led2)){
        printf("Error: %s\n",RPi_errorstr(RPi_errorno()));
    }
	
    if(0!=RPi_LED_toggle(led1)){
        printf("Error: %s\n",RPi_errorstr(RPi_errorno()));
    }

	printf("Press enter to terminate\n");
	getchar();


    if(0!=RPi_LED_off(led2)){
        printf("Error: %s\n",RPi_errorstr(RPi_errorno()));
    }

	if (0!=RPi_LED_close(led1)){
		printf("Error closing LED: %s\n",RPi_errorstr(RPi_errorno()));
	}
	if (0!=RPi_LED_close(led2)){
		printf("Error closing LED: %s\n",RPi_errorstr(RPi_errorno()));
	}
	return 0;
}
