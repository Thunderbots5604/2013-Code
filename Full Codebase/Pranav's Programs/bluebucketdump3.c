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
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.



float in = 114.59155902616464175;
float deg = 14.35;
//vars used for driving
int adjust = 25;
int dir;
int leftadjust;
int rightadjust;
//other vars
static bool rightStuck = false;
static bool leftStuck = false;
static const short checkTime = 3000;
static const short checkDistance = 50;
static long moving = 0;
static long movingBackup = 0;
static const short liftUpTime = 1700;//lift up time
static const short liftDownTime = 1300;//lift down time

int distanceTravelled(){
	return (abs((nMotorEncoder[Left]+nMotorEncoder[Right])/2));
}
void initializeRobot()
{
	servo[bucket] = 130;
}

void resetEncoders()
{
	nMotorEncoder[Left] = 0;
	nMotorEncoder[Right] = 0;
}

void move(short speed)
{
	ClearTimer(T3);
	moving = abs(nMotorEncoder[Left]);
	movingBackup = abs(nMotorEncoder[Right]);
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
  if (abs(nMotorEncoder[Right]) > abs(nMotorEncoder[Left]))
  {
  	leftadjust = 0;
  	rightadjust = adjust;
  }

  if (time1[T3] >= checkTime){
  		if (moving+checkDistance > abs(nMotorEncoder[Left])){
  			motor[Left] = 0;
  			motor[Left2] = 0;
  			leftStuck = true;
  		}//left is stuck
  		if (movingBackup+checkDistance > abs(nMotorEncoder[Right])){
  			motor[Right] = 0;
  			motor[Right2] = 0;
  			rightStuck = true;
  		}//right is stuck
  		moving = abs(nMotorEncoder[Left]);
			movingBackup = abs(nMotorEncoder[Right]);
			ClearTimer(T3);
		}//safety catch

	if (!leftStuck){
  	motor[Left] = dir*(speed-leftadjust);
  	motor[Left2] = dir*(speed-leftadjust);
  }

  if (!rightStuck){
  	motor[Right] = dir*(speed-rightadjust);
  	motor[Right2] = dir*(speed-rightadjust);
  }
}

void turn(short speed)
{
	ClearTimer(T2);
	moving = abs(nMotorEncoder[Left]);
	movingBackup = abs(nMotorEncoder[Right]);
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
  if (time1[T3] >= checkTime){
  		if (moving+checkDistance > abs(nMotorEncoder[Left])){
  			motor[Left] = 0;
  			motor[Left2] = 0;
  			leftStuck = true;
  		}//left is stuck
  		if (movingBackup+checkDistance > abs(nMotorEncoder[Right])){
  			motor[Right] = 0;
  			motor[Right2] = 0;
  			rightStuck = true;
  		}//right is stuck
  		moving = abs(nMotorEncoder[Left]);
			movingBackup = abs(nMotorEncoder[Right]);
			ClearTimer(T3);
		}//safety catch
	if (!leftStuck){
  	motor[Left] = dir*(speed-leftadjust);
  	motor[Left2] = dir*(speed-leftadjust);
  }
  if (!rightStuck){
  	motor[Right] = -dir*(speed-rightadjust);
  	motor[Right2] = -dir*(speed-rightadjust);
  }
}

void swingLeft(short speed)
{
  motor[Left] = 0;
  motor[Left2] = 0;
  motor[Right] = speed;
  motor[Right2] = speed;
}
void swingRight(short speed)
{
  motor[Left] = speed;
  motor[Left2] = speed;
  motor[Right] = 0;
  motor[Right2] = 0;
}

task main()
{
	short servoMovement = 40;
	waitForStart();
	ClearTimer(T1); //This timer measures the time needed to run the lift
	ClearTimer(T2); //This timer measures the amount of time needed to get move 19 inches
	resetEncoders();
	while (abs(nMotorEncoder[Left]) < (17*in))
  {
  	move(-50);
    if (time1[T1] < 1650)
  	{
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
	while (time1[T1] < 1650)
  {
  	motor[Lift1] = 100;
  	motor[Lift2] = 100;
  }

  motor[Lift1] = 0;
  motor[Lift2] = 0;

	ClearTimer(T3);
	servo[bucket] = 130;
	wait1Msec(1000);
	servo[bucket] = 90;
	wait1Msec(3000);
	servo[bucket] = 130;

	ClearTimer(T1); //This timer measures the time needed to run the lift
	ClearTimer(T2); //This timer measures the amount of time needed to get move 19 inches
	resetEncoders();

	move(0);
	while (abs(nMotorEncoder[Left]) < (15*in))
  {
  	move(-50);
    if (time1[T1] < 1300)
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
  resetEncoders();
  while(abs(nMotorEncoder[Left]) < 88*deg)
  {
  	turn(50);
    if (time1[T1] < 1300)
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
  resetEncoders();
	while (abs(nMotorEncoder[Left]) < (30*in))
  {
  	move(-50);
    if (time1[T1] < 1300)
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
	while (time1[T1] < 1300)
  {
  	motor[Lift1] = -100;
  	motor[Lift2] = -100;
  }

  motor[Lift1] = 0;
  motor[Lift2] = 0;


}
