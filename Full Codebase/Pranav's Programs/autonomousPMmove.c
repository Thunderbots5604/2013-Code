#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  none,     none)
#pragma config(Sensor, S3,     infrared,        sensorHiTechnicIRSeeker1200)
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

//version 6.0.1 alpha

#include "JoystickDriver.c"

//Do not touch this next twelve lines of code, they are easy to mess up
#ifndef NXTBUTTONPRESS
#define NXTBUTTONPRESS 325
#endif
#ifndef PI
#define PI 3.14159265358979323846246
#endif
//#ifndef TURN90TIME
#define TURN90TIME 245
//#endif
//#ifndef TURN45TIME
#define TURN45TIME 160
//#endif

//Up ramp time
#define UPRAMPTIME 1250

//initializing all procedures that are used
static void resetEncoders();
static void initializeRobot();
static void setMovementPower(float power); //forward is positive, backward is negative
static void setTurnPower(float power); //right is positive, left is negative

//initializing all functions that are used
static float distanceTravelled();
static float encoderDifference();

//initializing all tasks that are used
task move();
task liftUp();
task liftDown();

//initializing all the global integers that are used ***this is initializing (Daniel)***
static const unsigned short liftUpTime = 1750;
static const unsigned short liftDownTime = 1300;
static const unsigned short rampLineUpDistance = 36;

//initializing all the global floats that are used
static const float in = 360/PI;

//initializing all the global booleans that are used
static bool liftedUp = false;
static bool foundCrate = false;
static bool liftIsDown = false;

//initializing all the arrays that are used
static const float irDistances[4] = {15.5, 10, 19, 10};
static const float irBackDistances[4] = {10.5, 20.5, 39.75, 50}; //***five inches off of the TOTAL DISTANCE TRAVELLED, !!!NOT TWO INCHES LESS THAN irDistances!!! (Daniel)***
static const short irValues[4] = {5, 5, 4, 4};

static short crateVal = 0;//initializes the value that will be set to the crate which has the IR beacon

//values to which each side's motors will be set
short currentLeft = 0;
short currentRight = 0;

task main(){

	//This is for the nxt GUI
  nNxtExitClicks = 3;
  static float delay = 0;

  //This next section of code is a nxt delay setting gui, please do not mess this up, also ***********************PRESS THE ORANGE BUTTON!!!!**********************
  while (nNxtButtonPressed != 3){
  	if (nNxtButtonPressed == 1)
  		delay += 0.25;
  	if (nNxtButtonPressed == 2)
  		delay -= 0.25;
  	if (delay > 15)
  		delay = 0;
  	if (delay < 0)
  		delay = 15;
  	nxtDisplayCenteredTextLine(0, "%2.2f second delay", delay);
  	wait1Msec(NXTBUTTONPRESS);
  }
  eraseDisplay();
  nxtDisplayCenteredTextLine(3, "%2.2f delay set", delay);

	resetEncoders();//sets the wheel encoders to zero
	initializeRobot();//initializes the robot


	waitForStart();//waits until the start signal

	ClearTimer(T1);//Used for the countering pushes at the end

	wait1Msec(delay*1000);//waits the number of seconds set by the program
	//wait1Msec(500);//slight delay ***This is quite literaly a waste of time, the robot is already stationary (Daniel)***



	initializeRobot();
}

task move(){
	while (true){
		if (motor[Left] != currentLeft || motor[Left2] != currentLeft){
			motor[Left] = currentLeft;
			motor[Left2] = currentLeft;
		}
		if (motor[Right] != currentRight || motor[Right2] != currentRight){
			motor[Right] = currentRight;
			motor[Right2] = currentRight;
		}
		wait10Msec(1);
	}
}

task liftUp(){
	motor[Lift1] = 100;
	motor[Lift2] = 100;
	wait1Msec(liftUpTime);

	motor[Lift1] = 0;
	motor[Lift2] = 0;
	liftedUp = true;

	EndTimeSlice();
}

task liftDown(){
	motor[Lift1] = -100;
	motor[Lift2] = -100;
	wait1Msec(liftDownTime);

	motor[Lift1] = 0;
	motor[Lift2] = 0;
	liftIsDown = true;

	EndTimeSlice();
}

void setMovementPower(float power){
	currentLeft = power;
	currentRight = power;
}

void setTurnPower(float power){
	currentLeft = power;
	currentRight = -power;
}

void resetEncoders(){
	nMotorEncoder[Left] = 0;
	nMotorEncoder[Right] = 0;
}

void initializeRobot(){
	resetEncoders();
	motor[Lift1] = 0;
	motor[Lift2] = 0;
	motor[Sweep] = 0;
	motor[Flag] = 0;
	servo[bucket] = 72;
	servo[pin] = 0;
}

float distanceTravelled(){
	return((abs(nMotorEncoder[Left]+nMotorEncoder[Right])/2)/in);
}

float encoderDifference(){
	float left = nMotorEncoder[Left];
	float right = nMotorEncoder[Right];
	return(abs(left-right));
}
