/*First, we need access to the library*/
#include <RPi_GPIO.h>
/*Second, lets get some i/o access*/
#include <stdio.h>

/*This is what gets run when the program is called up*/
int main (){
	
	/*These are the pins that the LEDs are hooked up to*/
	int pin1 = 9;
	int pin2 = 10;

	/*These variables are how we will interact with those pins*/
	PIN * led1 = RPi_popen(pin1,ACTIVE_HIGH,OUT);
	PIN * led2 = RPi_popen(pin2,ACTIVE_HIGH,OUT);
	
	/*If this function does not return 0, we know something has gone wrong*/
	if(0!=RPi_pwrite(led1,true)){
		printf("ERROR %s\n",RPi_errorstr(RPi_errorno()));
	}
	printf("Press any key to toggle the LEDs\n");
	getchar();
        /*If this function does not return 0, we know something has gone wrong*/
        if(0!=RPi_pwrite(led2,true)){
                printf("ERROR %s\n",RPi_errorstr(RPi_errorno()));
        }
	
	/*If this function does not return 0, we know something has gone wrong*/
        if(0!=RPi_pwrite(led1,false)){
                printf("ERROR %s\n",RPi_errorstr(RPi_errorno()));
        }
	printf("Press any key to terminate\n");
	getchar();
	/*If this function does not return 0, we know something has gone wrong*/
        if(0!=RPi_pwrite(led2,false)){
                printf("ERROR %s\n",RPi_errorstr(RPi_errorno()));
        }



	if (0!=RPi_pclose(led1)){
		printf("Error closing pin: %s\n",RPi_errorstr(RPi_errorno()));
	}
	RPi_pclose(led2);
	return 0;
}
