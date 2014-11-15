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

#include "joystickDriver.c"

static bool rightCorrected = false;
static bool leftCorrected = false;

void resetEncoders(){
	nMotorEncoder[Left] = 0;
	nMotorEncoder[Right] = 0;
}

void move(short speed, short errors=20){
	short direction = 1;
	if (speed<0)
		direction = -1;

	if (((motor[Left] + motor[Right])/2) < speed){
		motor[Left] = motor[Left]+5*direction;
		motor[Right] = motor[Right]+5*direction;
		motor[Right2] = motor[Right2]+5*direction;
		motor[Left2] = motor[Left2]+5*direction;
	}

	if (abs(nMotorEncoder[Left]) > abs(nMotorEncoder[Right])+50){
		if (rightCorrected){
			motor[Right] += errors;
			motor[Right2] += errors;
			rightCorrected = false;
		}

		motor[Left] -= errors*direction;
		motor[Left2] -= errors*direction;
		leftCorrected = true;
		wait1Msec(40);
	}

	if (abs(nMotorEncoder[Right]) > abs(nMotorEncoder[Left])+50){
		if (leftCorrected){
			motor[Left] += errors;
			motor[Left2] += errors;
			leftCorrected = false;
		}
		motor[Right] -= errors*direction;
		motor[Right2] -= errors*direction;
		wait1Msec(40);
	}
}

void swingLeft(short speed){
	motor[Left] = 0;
	motor[Left2] = 0;
	motor[Right] = abs(speed);
	motor[Right2] = abs(speed);
}

void swingRight(short speed){
	motor[Left] = abs(speed);
	motor[Left2] = abs(speed);
	motor[Right] = 0;
	motor[Right2] = 0;
}

void simpleTurn(short speed){
	//positive is left, negative is right in relation to the front
	motor[Left] = -speed;
	motor[Right] = speed;
	motor[Left2] = -speed;
	motor[Right2] = speed;
}

void initializeRobot(){
	servo[bucket] = 135;
	servo[pin] = 0;
	resetEncoders();
}

task main(){
	short speed = 80;
	short irSearchTime = 2000;
	short returnTime;
	short turnTime90 = 600;
	short toRamp = 1250;
	short upRamp = 1000;
	short beginFirstBucket = 650;
	short endFirstBucket = 1300;
	short beginSecondBucket = 1300;
	short endSecondBucket = 1950;
	short beginThirdBucket = 1950;
	short endThirdBucket = 2000;

	initializeRobot();

	waitForStart();

	resetEncoders();

	ClearTimer(T1);
	while (time1[T1] < irSearch){


}