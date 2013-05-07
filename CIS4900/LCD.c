#include "LCD.h"
#include <unistd.h>
#define tiny 10

void pulseE(){
	setValue(E,1);
	usleep(10);
	setValue(E,0);
	usleep(10);
	setValue(E,1);
	usleep(10);
}

void init(){
	pulseE();
	setValue(C_D,0);//Function Set
	setValue(data[0],1);
	setValue(data[1],1);
	setValue(data[2],0);
	setValue(data[3],0);
	pulseE();
	pulseE();
	usleep(tiny);

	setValue(C_D,0);//Function Set
	setValue(data[0],0);
	setValue(data[1],1);
	setValue(data[2],0);
	setValue(data[3],0);
	pulseE();
	setValue(data[0],0);
	setValue(data[1],0);
	setValue(data[2],0);
	setValue(data[3],1);
	pulseE();
	usleep(tiny);

	setValue(C_D,0);//Function Set
	setValue(data[0],0);
	setValue(data[1],1);
	setValue(data[2],0);
	setValue(data[3],0);
	pulseE();
	setValue(data[0],0);
	setValue(data[1],0);
	setValue(data[2],0);
	setValue(data[3],1);
	pulseE();
	usleep(tiny);


	setValue(C_D,0);//Display ON 38
	setValue(data[0],1);
	setValue(data[1],1);
	setValue(data[2],0);
	setValue(data[3],0);
	pulseE();
	setValue(data[0],0);
	setValue(data[1],0);
	setValue(data[2],0);
	setValue(data[3],1);
	pulseE();
	usleep(tiny);

	setValue(C_D,0);//Display ON 10
	setValue(data[0],1);
	setValue(data[1],0);
	setValue(data[2],0);
	setValue(data[3],0);
	pulseE();
	setValue(data[0],0);
	setValue(data[1],0);
	setValue(data[2],0);
	setValue(data[3],0);
	pulseE();
	usleep(tiny);

	setValue(C_D,0);//Display Clear 0c
	setValue(data[0],0);
	setValue(data[1],0);
	setValue(data[2],0);
	setValue(data[3],0);
	pulseE();
	setValue(data[0],0);
	setValue(data[1],0);
	setValue(data[2],1);
	setValue(data[3],1);
	pulseE();
	usleep(tiny);

	setValue(C_D,0);//Entry Mode 06
	setValue(data[0],0);
	setValue(data[1],0);
	setValue(data[2],0);
	setValue(data[3],0);
	pulseE();
	setValue(data[0],0);
	setValue(data[1],1);
	setValue(data[2],1);
	setValue(data[3],0);
	pulseE();
	usleep(tiny);
}

int main(){
	init();
	return 0;
}