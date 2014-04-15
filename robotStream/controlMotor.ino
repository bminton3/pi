#include "NewPing.h"
#include "AFMotor.h"
#include "TimedAction.h"

#define TRIGGER_PIN  2   // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     13  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 20 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

/// HARDWARE ///
// Ultrasonic sensor
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
// DC motor 2
AF_DCMotor motor2(2, MOTOR12_2KHZ);
AF_DCMotor motor1(1, MOTOR12_2KHZ);

int MAX_SPEED =     255;   // 
int MIN_SPEED =     100;   // desired_speeds start at 100
int SPEED_STEP =    10;    // how much to increase speed by per click
int TURN_DELAY =    1000;  // how long to delay while turning

int speed =         0;     // current speed
int desired_speed = 0;     // accelerate to this
int direction =       0;   // 0 - not moving or moving forward, 1 - left, 2 - right

void setup() 
{
   // Open serial monitor at 9600 baud to see ping results.
  Serial.begin(9600);
  
  // turn on motor
  motor1.setSpeed(100);
  motor2.setSpeed(100);

  while (!Serial)
  {// WAIT to connect
  }
}

int accelerate(int v_o, int v_f)
{  
  if (v_o < v_f)
    {v_o += SPEED_STEP;}
  else if (v_o > v_f)
    {v_o -= SPEED_STEP;}
  return v_o
}

void turn()
{
  //   /|___________
  //  /  TURN LEFT  |
  //  \  ___________|
  //   \|
  if (direction == 1)
  { 
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
  }
  //   ___________|\
  //  | TURN RIGHT  \
  //  |___________  /
  //              |/
  else if (direction == 2)
  {
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
  }
    
  // ACCELERATE
  for (i = 0; i < MIN_SPEED; i+=SPEED_STEP)
  {
    motor1.setSpeed(i);
    motor2.setSpeed(i);
  }
  
  //TURN
  delay(TURN_DELAY);
  
  // DECELERATE
  for (i = i; i > 0; i-=SPEED_STEP)
  {
    motor1.setSpeed(i);
    motor2.setSpeed(i);
  }
  
  // STOP
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  desired_speed = 0;
  direction = 0;
}

void checkSerial()
{
  if (Serial.available())
    {
    
    // (0-ACCELERATE, 1-TURN)
    byte val = Serial.read();
    
    // ACCELERATE
    else if (val == '0')
    {
      // (0-BACKWARD, 1-TURN, 2-STOP)
      val = Serial.read();

      //      /\
      //     /  \
      //   FORWARD
      //  ---------
      if (val == '1')
      {
        // IF MOVING BACKWARD SET MIN_SPEED
        if (desired_speed < 0)
        {
          desired_speed = MIN_SPEED;
        }
        // ELSE ACCELERATE UNTIL MAX_SPEED
        else
        {
          desired_speed += 10;
          if (desired_speed > MAX_SPEED)
          {
            desired_speed = MAX_SPEED;
          }
        }
      }
      
      //  ----------
      //   BACKWARD
      //     \  /
      //      \/
      if (val == '0')
      {
        // IF MOVING FORWARD SET MIN_SPEED
        if (desired_speed > 0)
        {
          desired_speed = -1*MIN_SPEED;
        }
        // ELSE DECELERATE UNTIL MAX_SPEED
        else
        { 
          desired_speed -= SPEED_STEP;
          if (abs(desired_speed) > MAX_SPEED)
          {
            desired_speed = -1*MAX_SPEED;
          }
        }
      }
      //   ____
      //  /    \
      // | STOP |
      //  \____/
      else
      {
        desired_speed = 0;
      }
    }
    // TURN
    else if (val == '1')
    {
      // STOP
      desired_speed = 0;
      
      // (1-LEFT, 2-RIGHT)
      direction = int(Serial.read());
    }
  }
}

// TimedAction initialization
// checks serial connection
TimedAction serialAction = TimedAction(100, checkSerial);

void loop() 
{
  delay(50);
  
  int i;
  serialAction.check();
  
  // GET DISTANCE IN FRONT
  unsigned int uS = sonar.ping();
  unsigned int distance = uS / US_ROUNDTRIP_CM;
  
  //  DON'T RUN INTO THINGS
  if (distance < MAX_DISTANCE && distance > 0 && desired_speed > 0)
  {
    desired_speed == 0;
  }
  
  // SWITCH DIRECTIONS
  if (speed == 0)
  {
    if (desired_speed > 0)
    {
      motor1.set(FORWARD);
      motor2.set(FORWARD);
    }
    else if (desired_speed < 0)
    {
      motor1.set(BACKWARD);
      motor2.set(BACKWARD);
    }
  }
  
  // ACCELERATE
  speed = accelerate(speed, desired_speed);
  motor1.setSpeed(speed);
  motor2.setSpeed(speed);
  
  
  // TURN
  if (desired_speed == 0 & direction > 0)
  {
    turn();
  }
}

