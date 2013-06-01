#include "GPIO.h"
#include <stdio.h>

int main(){
	printPinMap();
	PIN mypin = On(1);
	if(NULL == mypin) printf("Couldn't Create Pin\n");
	printf("Pin value ");
	int read = Read(mypin));
	if(read > 0){
 		printf("TRUE\n");
 	}
 	else if(read < 0){
 		printf("FALSE\n");
 	}
 	else{
 		printf("UNDEF\n");
 	}
	printf("Set value false: %s\n", (Write(mypin,false) ? "TRUE" : "FALSE"));
	printf("Set value true: %s\n", (Write(mypin,true) ? "TRUE" : "FALSE"));
	printf("Pin off: %s\n", (Off(mypin) ? "TRUE" : "FALSE"));
}