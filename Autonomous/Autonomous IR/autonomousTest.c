#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     infrared,       sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     touch,          sensorTouch)
#pragma config(Motor,  mtr_S1_C1_1,     Left,          tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     Right,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     Lift1,         tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     Lift2,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     Left2,         tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     Right2,        tmotorTetrix, openLoop)
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
#define TURN90TIME 540
//#endif
//#ifndef TURN45TIME
#define TURN45TIME 270
//#endif

//Up ramp time
#define UPRAMPTIME 1250
//static const short delay = 0;//set this number to the delay in seconds ***(SET IN GUI (DANIEL))***

//declaring all procedures that are used
static void resetEncoders();
static void initializeRobot();
static void move(int speed);
static void brake(float waitTime = 0);
static void moveForward(float power);
static void dumpCube();
static void turn(float power); //right is positive, left is negative
static void turn90(bool right);//true is right, false is left
static void turn45(bool right);//true is right, false is left

static float proportion = 1/1440;

//initializing all functions that are used ***Prototyping, not initializing (Daniel)***
static float distanceTravelled();
static float encoderDifference();

//for the move function
int leftAdjust = 20;
int rightAdjust = 14;
int adjust = 27;
int dir;
int leftamount;
int rightamount;

//initializing all tasks that are used ***Prototyping, for the third time (Daniel)***
task liftUp();
task liftDown();

//initializing all the global integers that are used ***this is initializing (Daniel)***
static const unsigned short liftUpTime = 1750;
static const unsigned short liftDownTime = 1300;
static short power;
static const unsigned short rampLineUpDistance = 36;

//initializing all the global floats that are used
static const float in = 360/PI;
static float distToBucket;

//initializing all the global booleans that are used
bool liftedUp = false;
bool foundCrate = false;
bool liftIsDown = false;

static const float turnDifference = 7;

//initializing all the arrays that are used
static const float irDistances[4] = {14, 10, 18, 10};
//static const float irBackDistances[4] = {irDistances[0]-turnDifference, irBackDistances[0]+irDistances[1], irBackDistances[1]+irDistances[2], irBackDistances[2]+irDistances[3]}; //***five inches off of the TOTAL DISTANCE TRAVELLED, !!!NOT TWO INCHES LESS THAN irDistances!!! (Daniel)***
static const short irValues[4] = {5, 5, 4, 4};
static float irBackDistances[4] = {0, 0, 0, 0};

static short crateVal = 0;//initializes the value that will be set to the crate which has the IR beacon

task main(){

	irBackDistances[0] = irDistances[1] - turnDifference;
	for (short i = 1; i <= 3; i ++){
		irBackDistances[i] = irBackDistances[i-1]+irDistances[i];
	}

	//This is for the nxt GUI
  nNxtExitClicks = 3;
  static float delay = 0;
	static int tooLong = 20*1000;
  ClearTimer(T2);//automatically runs if the orange button has not been
  while (nNxtButtonPressed != 3 && time1[T2] <= tooLong){
  	if (nNxtButtonPressed == 1)
  		delay += 1;
  	if (nNxtButtonPressed == 2)
  		delay -= 1;
  	if (delay > 15)
  		delay = 0;
  	if (delay < 0)
  		delay = 15;
  	nxtDisplayCenteredTextLine(0, "%2.2f second delay", delay);
  	wait1Msec(NXTBUTTONPRESS);
  }
  eraseDisplay();
  nxtDisplayCenteredTextLine(0, "%2.2f delay set", delay);

	resetEncoders();//sets the wheel encoders to zero
	initializeRobot();//initializes the robot

	waitForStart();//waits until the start signal

	ClearTimer(T1);//Used for the countering pushes at the end

	wait1Msec(delay*1000);//waits the number of seconds set by the program

	move(-30);//starts moving backwards

	StartTask(liftUp);//start moving the lift up while the program runs

	for (short currentCrate = 0; currentCrate < 4; currentCrate++){//runs through the next block four times or until it finds the beacon
		while (distanceTravelled() <= irDistances[currentCrate]){//waits until the robot is at each bucket
			move(-30);
		}
		brake();
		while (nNxtButtonPressed != 3){
			nxtDisplayCenteredTextLine(0, "%d", SensorValue[infrared]);
		}
			if (SensorValue(infrared) == irValues[currentCrate]){//checks to see if the robot is at the bucket with the beacon
				foundCrate = true;
				crateVal = currentCrate+1;//sets the global variable that shows the rest of the program where the beacon is
				brake();//stops the robot
				wait1Msec(250);//slight delay (to lose momentum and completely stop)
				distToBucket = distanceTravelled();
				break;
			wait10Msec(1);
		}

		resetEncoders();//resets the encoders after every basket
	}



	resetEncoders();//resets the encoders again (to confirm that they were reset when the for loop broke)
	brake();//stops the robot in case the first one didn't work

	if (!foundCrate || crateVal == 0)//runs through if the sensor did not find the beacon
		crateVal = 4;//the robot is at the fourth bucket if it did not find the beacon anywhere else

	while (!liftedUp){}//waits if the lift is not all the way up yet
	StopTask(liftUp);//stops the task after the lift is up

	dumpCube();//dumps the cube
	StartTask(liftDown);//starts dropping the lift

	move(60);//moves forward towards the starting position

	while (distanceTravelled() <= irBackDistances[crateVal-1]){move(60);}//waits until the robot is five inches away from the wall

	brake(0.5);//stops five inches away from the wall

	turn45(true);//turns right at half power

	resetEncoders();
	move(60);
	while (distanceTravelled() < 12){move(60);}

	brake(0.5);
	turn45(true);

	resetEncoders();
	move(60);
	while (distanceTravelled() < rampLineUpDistance){move(60);}
	brake(0.5);

	while (!liftIsDown){}
	StopTask(liftDown);

	turn90(false);
	ClearTimer(T2);//for the time needed to go up the ramp
	move(-80);
	while (time1[T2] <= UPRAMPTIME){move(-80);}

	brake();
	wait1Msec(100);

	resetEncoders();
	//int currentCheck = 0;
	while (time1[T1] < 29500){//This  following loop counters any pushes that the robot may encounter
		if ((nMotorEncoder[Left]+nMotorEncoder[Right])/2 <= -0.5*in && (nMotorEncoder[Left]+nMotorEncoder[Right])/2 > -2*in){
			moveForward(30);
		}
		if ((nMotorEncoder[Left]+nMotorEncoder[Right])/2 >= 0.5*in && (nMotorEncoder[Left]+nMotorEncoder[Right])/2 < 2*in){
			moveForward(-30);
		}
		if ((nMotorEncoder[Left]+nMotorEncoder[Right])/2 >= 2*in){
			moveForward(-100);
		}
		if ((nMotorEncoder[Left]+nMotorEncoder[Right])/2 <= -2*in){
			moveForward(100);
		}
		if ((nMotorEncoder[Left]+nMotorEncoder[Right])/2 > -0.5*in){
			if ((nMotorEncoder[Left]+nMotorEncoder[Right])/2 < 0.5*in){
				brake();
			}
		}
	}
	brake();
	initializeRobot();
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

void pauseForTest(){
	while(!nNxtButtonPressed == 3){
		wait1Msec(10);
	}
}

void resetEncoders(){
	nMotorEncoder[Left] = 0;
	nMotorEncoder[Right] = 0;
}

void initializeRobot(){
	resetEncoders();
	brake();
	while (!SensorValue[touch]){
		motor[Lift1] = -50;
		motor[Lift2] = -50;
	}
	motor[Lift1] = 0;
	motor[Lift2] = 0;
	motor[Sweep] = 0;
	motor[Flag] = 0;
	servo[bucket] = 72;
	servo[pin] = 0;
}

void move(int speed)
{
	dir = 1;
	if (speed < 0)
		dir = -1;
	if (speed == 0)
		dir = 0;
  speed = abs(speed);
  int difference = abs(nMotorEncoder[Left]) - abs(nMotorEncoder[Right]);
	if (abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right]))
  {
  	leftamount = leftAdjust;
  	rightamount = 0;
  }
  else if (abs(nMotorEncoder[Left]) < abs(nMotorEncoder[Right]))
  {
  	leftamount = 0;
  	rightamount = rightAdjust;
  }
  motor[Left] = dir*(speed-leftamount);
  motor[Left2] = dir*(speed-leftamount);
  motor[Right] = dir*(speed-rightamount);
  motor[Right2] = dir*(speed-rightamount);
}

void brake(float waitTime){
	move(0);
	nxtDisplayCenteredTextLine(0, "%d", SensorValue[infrared]);
	wait1Msec(waitTime*1000);
}

void moveForward(float power){
	motor[Left] = power;
	motor[Left2] = power;
	motor[Right] = power;
	motor[Right2] = power;
}

void adjust12(){
	moveForward(25);
	while (distanceTravelled() <= 2){}
	brake();
}

void adjust34(){
	moveForward(-25);
	while (distanceTravelled() <= 2){}
	brake();
}

void dumpCube(){
	int servoMovement = 40;
	wait1Msec(50);
	servo[bucket] = 72;
	wait1Msec(50);
	servo[bucket] = 72 + (servoMovement);
	wait1Msec(2000);
	servo[bucket] = 72;
	wait1Msec(50);
}

void turn(float power){ //right is positive, left is negative
	motor[Left] = power;
	motor[Left2] = power;
	motor[Right] = -power;
	motor[Right2] = -power;
}

void turn90(bool right){ //true is right, false is left
	if (right)
		power = 100;
	if (!right)
		power = -100;
	motor[Left] = power;
	motor[Left2] = power;
	motor[Right] = -power;
	motor[Right2] = -power;
	wait1Msec(TURN90TIME);
	brake();
}

void turn45(bool right){ //true is right, false is left
	if (right)
		power = 100;
	if (!right)
		power = -100;
	motor[Left] = power;
	motor[Left2] = power;
	motor[Right] = -power;
	motor[Right2] = -power;
	wait1Msec(TURN45TIME);
	brake();
}

float distanceTravelled(){
	return((abs(nMotorEncoder[Left]+nMotorEncoder[Right])/2)/in);
}

float encoderDifference(){
	float left = nMotorEncoder[Left];
	float right = nMotorEncoder[Right];
	return(abs(left-right));
}