#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     infared,        sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     Left,          tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     Right,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     Lift1,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Lift2,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_1,     Sweep,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_2,     Flag,          tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    Bucket,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.

float in = 114.649682;
float deg = 17.0;
//vars used for driving
float a = 0;
int b = 0;
int c = 0;
int d = 0;
int e = 0;
int f = 0;


void initializeRobot()
{
	servo[Bucket] = 130;
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
  	if (abs(nMotorEncoder[Left]) < abs(nMotorEncoder[Right]))
  	{
  		e = 2;
  		f = 0;
  	}
  	if (abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right]))
  	{
  		e = 0;
  		f = 2;
  	}
  	motor[Left] = b*(a-e);
   	motor[Right] = b*(a-f);
  }
  nMotorEncoder[Left] = 0;
  nMotorEncoder[Right] = 0;
  while(abs(nMotorEncoder[Left]) < dist)//constant speed
  {
  	if (abs(nMotorEncoder[Left]) < abs(nMotorEncoder[Right]))
  	{
  		e = 2;
  		f = 0;
  	}
  	if (abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right]))
  	{
  		e = 0;
  		f = 2;
  	}
  	motor[Left] = b*(a-e);
   	motor[Right] = b*(a-f);
  }
  nMotorEncoder[Left] = 0;
  nMotorEncoder[Right] = 0;
  while(abs(nMotorEncoder[Left]) <dist)//deceleration
  {
  	a = 30+power-power*(abs(nMotorEncoder[Left])/dist);
  	if (abs(nMotorEncoder[Left]) < abs(nMotorEncoder[Right]))
  	{
  		e = 2;
  		f = 0;
  	}
  	if (abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right]))
  	{
  		e = 0;
  		f = 2;
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
  	if (abs(nMotorEncoder[Left]) < abs(nMotorEncoder[Right]))
  	{
  		e = 2;
  		f = 0;
  	}
  	if (abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right]))
  	{
  		e = 0;
  		f = 2;
  	}
  	motor[Left] = c*(a-e);
   	motor[Right] = d*(a-f);
  }
  nMotorEncoder[Left] = 0;
  nMotorEncoder[Right] = 0;
  while(abs(nMotorEncoder[Left]) < dist)//constant speed
  {
  	if (abs(nMotorEncoder[Left]) < abs(nMotorEncoder[Right]))
  	{
  		e = 2;
  		f = 0;
  	}
  	if (abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right]))
  	{
  		e = 0;
  		f = 2;
  	}
  	motor[Left] = c*(a-e);
   	motor[Right] = d*(a-f);
  }
  nMotorEncoder[Left] = 0;
  nMotorEncoder[Right] = 0;
  while(abs(nMotorEncoder[Left]) < dist)//deceleration
  {
  	a = 50+power-power*(abs(nMotorEncoder[Left])/dist);
  	if (abs(nMotorEncoder[Left]) < abs(nMotorEncoder[Right]))
  	{
  		e = 2;
  		f = 0;
  	}
  	if (abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right]))
  	{
  		e = 0;
  		f = 2;
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

  int ir = 0;
  int wait = 800;
  move(10, 6);
}
