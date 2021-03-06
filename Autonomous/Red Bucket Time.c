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

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth message

//Version 1.0
//Do not change this code without updating the version

//float deg = 14.35;
//vars used for driving
short adjust = 20;
short dir;
short leftadjust;
short rightadjust;
short firstMove = 710;
short secondMove = 1200;
short turnTime = 575;
short upRamp = 1500;

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

	unsigned short waitValue;
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
  	if (waitValue > 18000)
  		waitValue = 0;
  	if (waitValue < 0)
  		waitValue = 18000;
  }

  nxtDisplayCenteredTextLine(0, "Wait Time Done");

  waitForStart();

  wait1Msec(waitValue);

  ClearTimer(T1);
  ClearTimer(T2);

  resetEncoders();
  while (time1[T2] < firstMove){
  	move(-80);
  	if (time1[T1] < 1700){
  		motor[Lift1] = 100;
      motor[Lift2] = 100;
    }
    else
    {
    	motor[Lift1] = 0;
      motor[Lift2] = 0;
    }
  }
  move(0);
  while (time1[T1] < 1700){
  	motor[Lift1] = 100;
  	motor[Lift2] = 100;
  }
  motor[Lift1] = 0;
  motor[Lift2] = 0;

  //dump cube
  servo[bucket] = 170;
  wait1Msec(2000);
  servo[bucket] = 135;

  //continue to lift
  resetEncoders();
  ClearTimer(T1);
  ClearTimer(T2);
  while (time1[T2] < secondMove){
  	move(-80);
  	if (time1[T1] < 1200){
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



  wait10Msec(50);
  resetEncoders();
  ClearTimer(T2);
  while (time1[T2] < turnTime){
  	turn(-80);
 		if (time1[T1] < 1200){
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

  resetEncoders();
  while (time1[T2] < upRamp){
  	move(-80);
  	if (time1[T1] < 1200){
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

   while (time1[T1] < 1200){
  	motor[Lift1] = -100;
  	motor[Lift2] = -100;
  }
  motor[Lift1] = 0;
  motor[Lift2] = 0;
}
