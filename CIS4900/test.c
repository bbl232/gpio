#include "GPIO.h"
#include <stdio.h>
#include <stdlib.h>
int main(){
	RPi_init();
	PIN * myP = RPi_popen(0,ACTIVE_LOW,INOUT);
	int i = 0;
	int in = 1;
	for (;i<5;i++){
		scanf("%d",&in);
		RPi_pwrite(myP,in == 1);
	}

	RPi_pclose(myP);
}
