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
float deg = 14.35;
//vars used for driving
float a = 0;
int b = 0;
int c = 0;
int d = 0;
int e = 0;
int f = 0;
int i = 24;

void initializeRobot()
{
	servo[bucket] = 130;
	servo[pin] = 0;
}

void move(int power, float dist)//"+" =foward  "-" =reverse, dist is in inches
{
	dist -= 1;
	b = 1;
	if (power < 0)
	{
		b = -1;
	}
	power = abs(power)-30;
	dist = abs(dist*in/3);

	nMotorEncoder[Left] = 0;
  while(abs(nMotorEncoder[Left]) < dist)//acceleration
  {
  	a = 30+power*(abs(nMotorEncoder[Left])/dist);
  	if (abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right]))
  	{
  		e = i;
  		f = 0;
  	}
  	else
  	{
  		e = 0;
  		f = i;
  	}
  	motor[Left] = b*(a-e);
   	motor[Right] = b*(a-f);
  }
  nMotorEncoder[Left] = 0;
  nMotorEncoder[Right] = 0;
  while(abs(nMotorEncoder[Left]) < dist)//constant speed
  {
  	if (abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right]))
  	{
  		e = i;
  		f = 0;
  	}
  	else
  	{
  		e = 0;
  		f = i;
  	}
  	motor[Left] = b*(a-e);
   	motor[Right] = b*(a-f);
  }
  nMotorEncoder[Left] = 0;
  nMotorEncoder[Right] = 0;
  while(abs(nMotorEncoder[Left]) <dist)//deceleration
  {
  	a = 30+power-power*(abs(nMotorEncoder[Left])/dist);
  	if (abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right]))
  	{
  		e = i;
  		f = 0;
  	}
  	else
  	{
  		e = 0;
  		f = i;
  	}
  	motor[Left] = b*(a-e);
   	motor[Right] = b*(a-f);
  }
  motor[Left] = 0;
  motor[Right] = 0;
}

void turn(int power, float dist)// "-" =left  "+" =right, dist is in inches
{
	c = 1;
	d = -1;
	if (power < 0)
	{
		c = -1;
	  d = 1;
	}
	power = abs(power)-50;
	dist = abs(dist*deg/3);

	nMotorEncoder[Left] = 0;
	nMotorEncoder[Right] = 0;
  while(abs(nMotorEncoder[Left]) < dist)//acceleration
  {
  	a = 50+power*(abs(nMotorEncoder[Left])/dist);
  	if (abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right]))
  	{
  		e = i;
  		f = 0;
  	}
  	else
  	{
  		e = 0;
  		f = i;
  	}
  	motor[Left] = c*(a-e);
   	motor[Right] = d*(a-f);
  }
  nMotorEncoder[Left] = 0;
  nMotorEncoder[Right] = 0;
  while(abs(nMotorEncoder[Left]) < dist)//constant speed
  {
  	if (abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right]))
  	{
  		e = i;
  		f = 0;
  	}
  	else
  	{
  		e = 0;
  		f = i;
  	}
  	motor[Left] = c*(a-e);
   	motor[Right] = d*(a-f);
  }
  nMotorEncoder[Left] = 0;
  nMotorEncoder[Right] = 0;
  while(abs(nMotorEncoder[Left]) < dist)//deceleration
  {
  	a = 50+power-power*(abs(nMotorEncoder[Left])/dist);
  	if (abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right]))
  	{
  		e = i;
  		f = 0;
  	}
  	else
  	{
  		e = 0;
  		f = i;
  	}
  	motor[Left] = c*(a-e);
   	motor[Right] = d*(a-f);
  }
  motor[Left] = 0;
  motor[Right] = 0;
}

task main()
{
	initializeRobot();

  waitForStart();

  int wait = 75;

  //move block to Hot Wired
  move(-50,11.5);//line up block with Hot Wired
  wait1Msec(wait);

  move(50,6);

  turn(100,55);//turn right 90deg
  wait1Msec(wait);

  move(100,30);//move infront of ramp
  wait1Msec(wait);

  turn(100,45);//turn so back faces ramp
  wait1Msec(wait);

  move(100,14);//move to ramp
  wait1Msec(wait);

  turn(-100,90);//turn to ramp
  wait1Msec(wait);

  move(-100,35);//go up the ramp
  wait1Msec(wait);

}
