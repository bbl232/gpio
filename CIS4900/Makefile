default: GPIO.c GPIO.h LCD.c LCD.h
	@ echo -n "(re)building library "
	@ gcc -Wall -c LCD.c
	@ echo -n "."
	@ gcc -o LCD LCD.o GPIO.o
