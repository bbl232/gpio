#include "GPIO.h"
#include <stdio.h>
#include <stdlib.h>
int main(){
	printPinMap();
	int relays[4] = {0,1,2,3};
	int i = 0;

	PIN ap1 = PinOn(0);
	PIN ap2 = PinOn(1);
	PIN ap3 = PinOn(2);
	PIN ap4 = PinOn(3);
	SetLogic(ap1,ACTIVE_LOW);
	SetLogic(ap2,ACTIVE_LOW);
	SetLogic(ap3,ACTIVE_LOW);
	SetLogic(ap4,ACTIVE_LOW);
	SetDirection(ap1,OUT);
	SetDirection(ap2,OUT);
	SetDirection(ap3,OUT);
	SetDirection(ap4,OUT);
	Write(ap1,true);
	Write(ap2,true);
	Write(ap3,true);
	Write(ap4,true);
	sleep(1);
	Write(ap1,false);
	Write(ap2,false);
	Write(ap3,false);
	Write(ap4,false);
	PinOff(ap1);
	PinOff(ap2);
	PinOff(ap3);
	PinOff(ap4);

}
