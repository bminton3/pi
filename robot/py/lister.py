import serial
ser = serial.Serial('/dev/ttyACM0' , 9600) 
a = ''
while 1:
	if len(a)>0:
		a = ''
	for i in ser.read():
		if (i=='1' or i=='0'):
			ser.write('0')
			ser.read()
			for j in range(1, len('led blinking')):
				a += ser.read()
	print a 
