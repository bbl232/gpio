gpio
====

RaspberryPi GPIO Library (CIS4900)
see http://rpigpio.blogspot.com/ for project updates

Concepts
  "gpio"     - Stands for General Purpose Input/Output.

  "pin"      - A physical pin on the Raspberry Pi board.
               There are 26 of them, 17 of which can be used for gpio.
               All 17 pins are mapped in software to the phsical pins on the board.
               Changing the state of a pin in the software changes the state of the physical pin.
          
  "export"   - Each pin sits dormant until it is exported for use.
               There is no way of controlling a pin on the board without having first exported it.
               Exporting a pin creates the interface neccesary to interact with it.
               Users of this library do not have to export pins as the software will export as required.
             
  "unexport" - Each exported pin's interface remains active until unexported.
               Once unexported, a pin may not be used again until it is first exported.
               
  "read"     - To read from a pin is to get the current value on the pin as represented by a 0 or 1.
  
  "write"    - To write to a pin is to set the value on that pin to either high or low voltage.


API Usage
  To use the API you need to include GPIO.h and compile with the library included
