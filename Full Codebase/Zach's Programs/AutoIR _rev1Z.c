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

//Based on Pablo's AutoIR_rev5_WChoose.c program

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.

float in = 114.649682;
//float deg = 14.35;
//other vars
int a = 40;//power
int b = 1700;//lift up time
int c = 1300;//lift down time
int d = 2;//a number

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

float leftEnc()
{
	return abs(nMotorEncoder[Left]);
}

float rightEnc()
{
	return abs(nMotorEncoder[Right]);
}

void move(int speed)
{
	int adjust = 0;
	/*if (leftEnc() > rightEnc())
	{
		adjust = 5;
	}
	if (rightEnc() > leftEnc())
	{
		adjust = -5;
	}*/
  motor[Left] = speed  -adjust;
  motor[Left2] = speed  +adjust;
  motor[Right] = -speed  -adjust;
  motor[Right2] = -speed  +adjust;
}

void turn(int speed)
{
	motor[Left] = speed;
  motor[Left2] = speed;
  motor[Right] = speed;
  motor[Right2] = speed;
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

void swing(int speed, string dir) //direction is either "right", "r", "left", or "l"
{
	if (dir == "right" || dir == "r")
	{
		motor[Left] = speed;
		motor[Left2] = speed;
		motor[Right] = 0;
		motor[Right2] = 0;
	}
	if (dir == "left" || dir == "l")
	{
		motor[Left] = 0;
		motor[Left2] = 0;
		motor[Right] = speed;
		motor[Right2] = speed;
	}
}

void lift(int power)
{
	motor[Lift1] = power;
  motor[Lift2] = power;
}

task main()
{
  initializeRobot();
  int waitValue = 0;
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
  	if (waitValue > 30000)
  		waitValue = 30000;
  	if (waitValue < 0)
  		waitValue = 0;
  }

  waitForStart();

  wait1Msec(waitValue);

  int ir = 0;
  resetEncoders();
  int dist1 = 16*in;
// 1st search and raise lift
  ClearTimer(T1);
  while (leftEnc() < dist1+(43*in) && ir == 0)
  {
  	move(-a);
    if (time1[T1] < b)
  	{
  		lift(100);
    }
    else
    {
    	lift(0);
    }
    if (SensorValue[infared] >= 4 && leftEnc() > dist1)
    	ir = 1;
  }
  move(0);
// finish raising lift
  while (time1[T1] < b)
  {
  	lift(100);
  }
  lift(0);
// ir not found
  if (ir == 0)
  	ir = 2;
// align
  if (ir == 1)
  {
  	wait1Msec(100);
  	if ( (abs(nMotorEncoder[Left]) > dist1) && (abs(nMotorEncoder[Left]) < dist1+9.5*in) )
  		d = 2*in;
    if ( (abs(nMotorEncoder[Left]) > dist1+(9.5*in)) && (abs(nMotorEncoder[Left]) < dist1+(21.5*in)) )
  		d = 12*in;
    if ( (abs(nMotorEncoder[Left]) > dist1+(21.5*in)) && (abs(nMotorEncoder[Left]) < dist1+(38*in)) )
  		d = 31*in;
    if ( (abs(nMotorEncoder[Left]) > dist1+(38*in)) && (abs(nMotorEncoder[Left]) < dist1+(43*in)) )
  		d = 41*in;


    while( ((abs(nMotorEncoder[Left])/in)+1 < (dist1+d)/in) ^ ((abs(nMotorEncoder[Left])/in)-1 > (dist1+d)/in) )//while( int(abs(nMotorEncoder[Left])) != int(dist1+d) )
    {
    	if (abs(nMotorEncoder[Left]) > dist1+d)
  	    move(20);
    	if (abs(nMotorEncoder[Left]) < dist1+d)
  	    move(-20);
    }
    move(0);
  }
  if (ir == 2)
  {
  	wait1Msec(500);
  	while (abs(nMotorEncoder[Left]) > dist1)
  	  move(a);
  }
  move(0);
// dump cube
  servo[bucket] = 245;
  wait1Msec(1500);
  servo[bucket] = 130;
  wait10Msec(100);
// move to end of row and lower lift
  ClearTimer(T1);
  while (abs(nMotorEncoder[Left]) > (dist1-(6*in)))
  {
  	move(a);
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
  	swingRight(a);
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
  	move(a);
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
  while (abs(nMotorEncoder[Left]) < 9.5*in)
  {
  	turn(-a);
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