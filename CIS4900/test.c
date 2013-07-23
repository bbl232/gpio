#include "GPIO.h"
#include <stdio.h>
#include <stdlib.h>
int main(){
	printPinMap();
	int relays[4] = {10,9,6,7};
	int i = 0;

	for(;i<4;i++){
		setValue(relays[i],true);
		sleep(1);
	}
	for(;i<4;i++){
		setValue(relays[i],false);
		sleep(1);
	}
	for(;i<4;i++){
		setValue(relays[i],true);
		sleep(1);
	}
}