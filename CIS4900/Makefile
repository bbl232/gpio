default: test

test: GPIO.o GPIO.h test.o
	gcc -o test GPIO.o test.o

clean:
	rm *.o
.SUFFIXES:
.SUFFIXES: .o .c
.c.o:
	gcc -Wall -g -std=c99 -c $<
