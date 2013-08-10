default: libRPi_GPIO.so test 

libRPi_GPIO.so: GPIO.o DEVICES.o
	ar rcs libRPi_GPIO.so GPIO.o DEVICES.o

test: GPIO.o GPIO.h test.o
	gcc -o test GPIO.o test.o

clean:
	rm *.o
.SUFFIXES:
.SUFFIXES: .o .c
.c.o:
	gcc -Wall -g -fPIC -std=c99 -c $< 
