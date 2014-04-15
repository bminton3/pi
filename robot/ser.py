import serial
ser = serial.Serial('/dev/ttyACM0',9600)
while 1:
    a = raw_input()
    ser.write(a)
    while (ser.inWaiting()==0):
	break
    print ser.read()
