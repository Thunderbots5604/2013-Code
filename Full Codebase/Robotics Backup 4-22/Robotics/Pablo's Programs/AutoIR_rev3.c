#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  none,     none)
#pragma config(Sensor, S3,     infared,        sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     touch,          sensorTouch)
#pragma config(Motor,  mtr_S1_C1_1,     Left,          tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     Right,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     Left2,         tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     Right2,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     Lift1,         tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     Lift2,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_1,     Sweep,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_2,     Flag,          tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    bucket,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    pin,                  tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.

float in = 114.649682;
//float deg = 14.35;
//vars used for driving
int adjust = 20;
int dir;
int leftadjust;
int rightadjust;
//other vars
int a;//used for distance
int b = 1650;//lift up time
int c = 1300;//lift down time

void initializeRobot()
{
	servo[bucket] = 130;
	servo[pin] = 0;
}

void resetEncoders()
{
	nMotorEncoder[Left] = 0;
	nMotorEncoder[Right] = 0;
}

void move(int speed)
{
	dir = 1;
	if (speed < 0)
		dir = -1;
	if (speed == 0)
		dir = 0;
  speed = abs(speed);
	if (abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right]))
  {
  	leftadjust = adjust;
  	rightadjust = 0;
  }
  else
  {
  	leftadjust = 0;
  	rightadjust = adjust;
  }
  motor[Left] = dir*(speed-leftadjust);
  motor[Right] = dir*(speed-rightadjust);
}

void turn(int speed)
{
	dir = 1;
	if (speed < 0)
		dir = -1;
	if (speed == 0)
		dir = 0;
  speed = abs(speed);
	if (abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right]))
  {
  	leftadjust = adjust;
  	rightadjust = 0;
  }
  else
  {
  	leftadjust = 0;
  	rightadjust = adjust;
  }
  motor[Left] = dir*(speed-leftadjust);
  motor[Right] = -dir*(speed-rightadjust);
}

void swingLeft(int speed)
{
  motor[Left] = 0;
  motor[Right] = speed;
}
void swingRight(int speed)
{
  motor[Left] = speed;
  motor[Right] = 0;
}

task main()
{
  initializeRobot();

  waitForStart();

  int ir = 0;
  resetEncoders();
  int dist1 = 16*in;
// 1st search and raise lift
  ClearTimer(T1);
  while ((abs(nMotorEncoder[Left]) < (dist1+43*in)) && ir == 0)
  {
  	move(-60);
    if (time1[T1] < b)
  	{
  		motor[Lift1] = 100;
      motor[Lift2] = 100;
    }
    else
    {
    	motor[Lift1] = 0;
      motor[Lift2] = 0;
    }
    if (SensorValue[infared] == 5)
    	ir = 1;
  }
  move(0);
// finish raising lift
  while (time1[T1] < b)
  {
  	motor[Lift1] = 100;
  	motor[Lift2] = 100;
  }
  motor[Lift1] = 0;
  motor[Lift2] = 0;
// 2nd search
  if (ir == 0)
  wait1Msec(500);
  while ((abs(nMotorEncoder[Left]) > (dist1+2*in)) && ir == 0)
  {
  	move(80);
    if (SensorValue[infared] == 5)
    	ir = 2;
  }
  move(0);
// ir not found
  if (ir == 0)
  	ir = 3;
// align
  if (ir == 2)
  {
  	wait1Msec(500);
  	a = abs(nMotorEncoder[Left])+2*in;
    while (abs(nMotorEncoder[Left]) < a)
  	  move(30);
  }
  move(0);
// dump cube
  move(-80);
  wait1Msec(500);
  move(0);
  servo[bucket] = 245;
  wait1Msec(3000);
  servo[bucket] = 130;
  wait10Msec(100);
// move to end of row and lower lift
  ClearTimer(T1);
  while (abs(nMotorEncoder[Left]) > (dist1-(4*in)))
  {
  	move(80);
    if (time1[T1] < 1400)
  	{
  		motor[Lift1] = -100;
      motor[Lift2] = -100;
    }
    else
    {
    	motor[Lift1] = 0;
      motor[Lift2] = 0;
    }
  }
  move(0);
// swing 90 deg right and lower lift
  resetEncoders();
  while (abs(nMotorEncoder[Left]) < 25*in)
  {
  	swingRight(80);
    if (time1[T1] < c)
  	{
  		motor[Lift1] = -100;
      motor[Lift2] = -100;
    }
    else
    {
    	motor[Lift1] = 0;
      motor[Lift2] = 0;
    }
  }
  move(0);
// move infront of ramp
  resetEncoders();
  while (abs(nMotorEncoder[Left]) < 24*in)
  {
  	move(80);
  	if (time1[T1] < c)
  	{
  		motor[Lift1] = -100;
      motor[Lift2] = -100;
    }
    else
    {
    	motor[Lift1] = 0;
      motor[Lift2] = 0;
    }
  }
  move(0);
// turn 90deg left
  resetEncoders();
  while (abs(nMotorEncoder[Left]) < 8.5*in)
  {
  	turn(-80);
  	if (time1[T1] < c)
  	{
  		motor[Lift1] = -100;
      motor[Lift2] = -100;
    }
    else
    {
    	motor[Lift1] = 0;
      motor[Lift2] = 0;
    }
  }
  move(0);
//finish lowering lift
  while (time1[T1] < c)
  {
  	motor[Lift1] = -100;
  	motor[Lift2] = -100;
  }
  motor[Lift1] = 0;
  motor[Lift2] = 0;
// move up ramp
  wait1Msec(500);
  resetEncoders();
  while (abs(nMotorEncoder[Left]) < 30*in)
  	move(-80);
  move(0);


}
