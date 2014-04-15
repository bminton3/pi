#!/usr/bin/python
from flask import Flask, render_template, request, jsonify
import subprocess
import serial

## Connect to Arduino over USB
global ser
ser = serial.Serial('/dev/ttyACM0' , 9600)

## Start the webcam
subprocess.call(["screen", "-d", "-m", "./runCamera"])

## Route / to index.html
app = Flask(__name__)      
@app.route('/')
def index():
  	return render_template('index.html')

## Accelerate the Robot ( -1 for Backward & 1 for Forward )
@app.route('/accel')
def accel():
	a = request.args.get('a', 0, type=int)
	print "accelerating ", a
	return '\n'

## Turn the Robot ( -1 for Left & 1 for Right )
@app.route('/turn')
def turn():
        t = request.args.get('t', 0, type=int)
        print "turning ", t
        return '\n'

## Start the Flask Web Server 
if __name__ == '__main__':
  	app.run(debug=True,
		host='0.0.0.0', 
		port=5000)
