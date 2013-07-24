#include "GPIO.h"
#include <stdio.h>
#include <stdlib.h>
int main(){
	printPinMap();
	int relays[4] = {0,1,2,3};
	int i = 0;

	for(i=0;i<4;i++){
		setValue(relays[i],true);
	}
	for(i=0;i<4;i++){
		setValue(relays[i],false);
	}
	for(i=0;i<4;i++){
		setValue(relays[i],true);
	}
}