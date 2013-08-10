#include "GPIO.h"
#include <stdio.h>
#include <stdlib.h>
int main(){
	RPi_init();
	PIN * myP= RPi_popen(0,ACTIVE_LOW,INOUT);
	bool value;
	RPi_pread(myP,&value);
	if(value){
		printf ("Read: True");
	}
	else{
		printf("Read False");
	}
	RPi_pwrite(myP,false);
}
