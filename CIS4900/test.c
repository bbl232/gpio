#include "GPIO.h"
#include <stdio.h>

int main(){
	printPinMap();
	PIN mypin = On(1);
	if(NULL == mypin) printf("Couldn't Create Pin\n");
	printf("Pin value %s\n", (Read(mypin) ? "TRUE" : "FALSE"));
	printf("Set value false: %s\n", (Write(mypin,false) ? "TRUE" : "FALSE"));
	printf("Set value true: %s\n", (Write(mypin,true) ? "TRUE" : "FALSE"));
	printf("Pin off: %s\n", (Off(mypin) ? "TRUE" : "FALSE"));
}