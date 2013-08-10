default: libRPi_GPIO.so test 

libRPi_GPIO.so: GPIO.o DEVICES.o
	ar rcs libRPi_GPIO.so GPIO.o DEVICES.o

test: GPIO.o RPi_GPIO.h test.o
	gcc -o test GPIO.o test.o

install: libRPi_GPIO.so RPi_GPIO.h
	sudo cp libRPi_GPIO.so /usr/local/lib/.
	sudo cp RPi_GPIO.h /usr/local/include/.

uninstall:
	sudo rm /usr/local/lib/libRPi_GPIO.so
	sudo rm /usr/local/includes/RPi_GPIO.h

clean:
	rm *.o
	rm test
	rm *.so

.SUFFIXES:
.SUFFIXES: .o .c
.c.o:
	gcc -Wall -g -fPIC -std=c99 -c $< 
