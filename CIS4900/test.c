#include "GPIO.h"
#include <stdio.h>
#include <stdlib.h>
int main(){
	RPi_init();
	PIN * myP = RPi_popen(0,ACTIVE_LOW,INOUT);
	if(myP == NULL){
		printf("ERROR! %s\n", errorstr(errorno()));
	}
	int i = 0;
	int in = 1;
	for (;i<5;i++){
		scanf("%d",&in);
		if(0!=RPi_pwrite(myP,in == 1)){
			printf("ERROR! %s\n", errorstr(errorno()));
		}
	}
	RPi_pclose(myP);
}
