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

//Up ramp time
#define UPRAMPTIME 1750
//static const short delay = 0;//set this number to the delay in seconds ***(SET IN GUI (DANIEL))***

//initializing all procedures that are used (***(Actually called prototyping, not initializing (Daniel))***)

static void turn90(bool right);//true is right, false is left
static void turn45(bool right);//true is right, false is left

void brake(int waitTime = 0){
	move(0);
	wait1Msec(waitTime);
}





//initializing all the global floats that are used
static const float in = 360/PI;
static float distToBucket;



//initializing all the arrays that are used
static const float irDistances[4] = {14, 10, 19, 10};
static const float irBackDistances[4] = {irDistances[0]-7, irBackDistances[0]+irDistances[1], irBackDistances[1]+irDistances[2], irBackDistances[2]+irDistances[3]}; //***five inches off of the TOTAL DISTANCE TRAVELLED, !!!NOT TWO INCHES LESS THAN irDistances!!! (Daniel)***
static const short irValues[4] = {5, 5, 4, 4};

static short crateVal = 0;//initializes the value that will be set to the crate which has the IR beacon

static const int turn45 = 230;
static const int turn90 = 230;

task main()
{



}
