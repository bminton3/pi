#!/usr/bin/env python
from nanpy import Arduino
from nanpy import serial_manager
serial_manager.connect('/dev/ttyACM0')        # serial connection to Arduino
from time import sleep

# Import Key Stroke
import termios, fcntl, sys, os

print "finished importing "

fd = sys.stdin.fileno()

oldterm = termios.tcgetattr(fd)
newattr = termios.tcgetattr(fd)
newattr[3] = newattr[3] & ~termios.ICANON & ~termios.ECHO
termios.tcsetattr(fd, termios.TCSANOW, newattr)

oldflags = fcntl.fcntl(fd, fcntl.F_GETFL)
fcntl.fcntl(fd, fcntl.F_SETFL, oldflags | os.O_NONBLOCK)

#Set the pin to the on Board LED
LED =13  
Arduino.pinMode(LED, Arduino.OUTPUT)

try:
    while 1:
        try:
            c = sys.stdin.read(1)
	    print "===="
            print "Got character", `c`
        except IOError: pass
finally:
    termios.tcsetattr(fd, termios.TCSAFLUSH, oldterm)
    fcntl.fcntl(fd, fcntl.F_SETFL, oldflags)

while 1:
    c = raw_input('enter a key')
    Arduino.digitalWrite(LED, Arduino.LOW)
    sleep(1.5)

print"Changing brightness of LED"
bright = 128                           # Mid brightness
Arduino.analogWrite(LED, bright)
Arduino.digitalWrite(LED,Arduino.HIGH)          # Turn on LED

for i in range(0,200):
    bright = bright + 8
    if (bright > 200):          # LED already full on at this point
        bright = 0          # Minimum power to LED
    Arduino.analogWrite(LED, bright)           # Change PWM setting/brightness
    sleep(0.05)

Arduino.digitalWrite(LED,Arduino.LOW)          # Turn off LED
print"Finished"

