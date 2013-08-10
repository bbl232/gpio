#include "GPIO.h"
#include <stdio.h>
#include <stdlib.h>
int main(){
	RPi_init();
	PIN * myP= RPi_popen(0,ACTIVE_LOW,INOUT);
	bool value;
	if(0!=RPi_pread(myP,&value)){
		printf("Read ERROR\n");
	}
	if(value){
		printf ("Read: True");
	}
	else{
		printf("Read False");
	}
	RPi_pwrite(myP,false);
	RPi_pwrite(myP,true);
}
