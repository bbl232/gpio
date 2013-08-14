default: libRPi_GPIO.so 

libRPi_GPIO.so: GPIO.o DEVICES.o
	ar rcs libRPi_GPIO.so GPIO.o DEVICES.o


install: libRPi_GPIO.so RPi_GPIO.h RPi_GPIO_private.h
	sudo cp libRPi_GPIO.so /usr/local/lib/.
	sudo cp RPi_GPIO.h /usr/local/include/.
	sudo cp RPi_GPIO_private.h /usr/local/include/.

uninstall:
	sudo rm /usr/local/lib/libRPi_GPIO.so
	sudo rm /usr/local/include/RPi_GPIO.h
	sudo rm /usr/local/include/RPi_GPIO_private.h

clean:
	rm *.o
	rm test
	rm *.so

.SUFFIXES:
.SUFFIXES: .o .c
.c.o:
	gcc -Wall -g -fPIC -std=c99 -c $< 
