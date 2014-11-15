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
int adjust = 10;
int dir;
int leftadjust;
int rightadjust;
//other vars
int a = 30;//power
int b = 1700;//lift up time
int c = 1300;//lift down time
int d = 2;//a number
int dist1 = 14*in;
int pos1 = 2*in;
int pos2 = 12*in;
int pos3 = 31*in;
int pos4 = 41*in;

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
	int dir = 1;
	if (speed < 0)
		dir = -1;
	if (speed == 0)
		dir = 0;
  speed = abs(speed);
	if ( abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right]))
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
  motor[Left2] = dir*(speed-leftadjust);
  motor[Right] = dir*(speed-rightadjust);
  motor[Right2] = dir*(speed-rightadjust);
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
  motor[Left2] = dir*(speed-leftadjust);
  motor[Right] = -dir*(speed-rightadjust);
  motor[Right2] = -dir*(speed-rightadjust);
}

void swingLeft(int speed)
{
  motor[Left] = 0;
  motor[Left2] = 0;
  motor[Right] = speed;
  motor[Right2] = speed;
}
void swingRight(int speed)
{
  motor[Left] = speed;
  motor[Left2] = speed;
  motor[Right] = 0;
  motor[Right2] = 0;
}

task main()
{
  initializeRobot();
  short waitValue = 0;
  nNxtExitClicks = 3;
  while (nNxtButtonPressed != 0 && nNxtButtonPressed != 3){
  	nxtDisplayCenteredTextLine(0, "%d milliseconds", waitValue);
  	if (nNxtButtonPressed == 1){
  		waitValue += 1000;
  		wait1Msec(325);
  	}
  	if (nNxtButtonPressed == 2){
  		waitValue -= 1000;
 			wait1Msec(300);
 		}
  	if (waitValue > 15000)
  		waitValue = 0;
  	if (waitValue < 0)
  		waitValue = 15000;
  }

  nxtDisplayCenteredTextLine(0, "Wait Time Done");

  waitForStart();

  wait1Msec(waitValue);

  int ir = 0;
  resetEncoders();
// 1st search and raise lift
  ClearTimer(T1);
  while ((abs(nMotorEncoder[Left]) < (dist1+33*in)) && ir != 1)
  {
  	move(a);
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
    if (SensorValue[infared] == 3 && (abs(nMotorEncoder[Left]) > dist1))
    	ir = 2;
    if (SensorValue[infared] == 5 && ir == 2 && (abs(nMotorEncoder[Left]) > dist1))
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
// ir not found
  if (ir == 0)
  	ir = 2;
 wait1Msec(600);
// align
  if (ir == 1)
  {
  	wait1Msec(100);
  	if ( (abs(nMotorEncoder[Left]) > dist1) && (abs(nMotorEncoder[Left]) < dist1+9.5*in) )
  		d = pos1;
    if ( (abs(nMotorEncoder[Left]) > dist1+(9.5*in)) && (abs(nMotorEncoder[Left]) < dist1+(21.5*in)) )
  		d = pos2;
    if ( (abs(nMotorEncoder[Left]) > dist1+(21.5*in)) && (abs(nMotorEncoder[Left]) < dist1+(38*in)) )
  		d = pos3;
    if ( (abs(nMotorEncoder[Left]) > dist1+(38*in)) && (abs(nMotorEncoder[Left]) < dist1+(43*in)) )
  		d = pos4;


    while( ((abs(nMotorEncoder[Left])/in)+1 < (dist1+d)/in) ^ ((abs(nMotorEncoder[Left])/in)-1 > (dist1+d)/in) )//while( int(abs(nMotorEncoder[Left])) != int(dist1+d) )
    {
    	if (abs(nMotorEncoder[Left]) > dist1+d)
  	    move(-20);
    	if (abs(nMotorEncoder[Left]) < dist1+d)
  	    move(20);
    }
    move(0);
  }
  if (ir == 2)
  {
  	wait1Msec(500);
  	while (abs(nMotorEncoder[Left]) > dist1)
  	  move(-a);
  }
  move(0);
  wait1Msec(250);
// dump cube
  servo[bucket] = 245;
  wait1Msec(1500);
  servo[bucket] = 130;
  wait10Msec(100);
  turn(-20);
  wait1Msec(150);
  move(0);
// move to end of row and lower lift
  ClearTimer(T1);
  while (abs(nMotorEncoder[Left]) > (dist1-(6*in)))
  {
  	move(-a);
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
  while (abs(nMotorEncoder[Left]) < 24*in)
  {
  	swingRight(-a);
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
  while (abs(nMotorEncoder[Left]) < 21*in)
  {
  	move(-a);
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
  while (abs(nMotorEncoder[Left]) < 10*in)
  {
  	turn(a);
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
  wait1Msec(250);
  resetEncoders();
  while (abs(nMotorEncoder[Left]) < 30*in)
  	move(-100);
  move(0);


}