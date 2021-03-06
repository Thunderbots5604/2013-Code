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
void initializeRobot()
{
	motor[Lift1] = 0;
	motor[Lift2] = 0;
	motor[Left] = 0;
	motor[Right] = 0;
	servo[Bucket] = 135;
}

void accelerate(float maxSpeed, float timeToSpeed, float startSpeed, float maxDistance)
{
	maxDistance = maxDistance*in;
	int i = 0;//this will be used for the time to full speed in msec)
	int currentSpeed = startSpeed;
	motor[Left] = startSpeed;
	motor[Right] = startSpeed;
	while (i < timeToSpeed){
		if ((currentSpeed + (maxSpeed/100)) >= maxSpeed){
			motor[Left] = maxSpeed-6;
			motor[Right] = maxSpeed;
			i = timeToSpeed;
		}
		if ((currentSpeed + (maxSpeed/100)) < maxSpeed){
			currentSpeed += (maxSpeed/100);
			motor[Left] = currentSpeed-6;
			motor[Right] = currentSpeed;
			wait1Msec(timeToSpeed/100);
			i += (timeToSpeed/100);
		}
		if (maxDistance <= abs(nMotorEncoder[Right]))
			i = timeToSpeed;
	}
	while (maxDistance > abs(nMotorEncoder[Right])){
		motor[Left] = maxSpeed-6;
		motor[Right] = maxSpeed;
	}
}
void decelerate(float minSpeed, float timeToSpeed, float startSpeed, float maxDistance)
{
	maxDistance = maxDistance*in;
	int i = 0;//this is the time to speed as a variable
	int currentSpeed = startSpeed;
	motor[Left] = startSpeed-6;
	motor[Right] = startSpeed;
	while (i < timeToSpeed){
		if ((currentSpeed - ((100 - minSpeed)/100)) >= (100 - minSpeed)){
			motor[Left] = minSpeed+6;
			motor[Right] = minSpeed;
			i = timeToSpeed;
		}
		if ((currentSpeed + ((100 - minSpeed/100))) < (100 - minSpeed)){
			currentSpeed += (100 - minSpeed);
			motor[Left] = currentSpeed+6;
			motor[Right] = currentSpeed;
			wait1Msec(timeToSpeed);
			i += (timeToSpeed/100);
		}
		if (maxDistance <= abs(nMotorEncoder[Right])){
			motor[Left] = minSpeed+6;
			motor[Right] = minSpeed;
			i = timeToSpeed;
		}
	}
	while (maxDistance > abs(nMotorEncoder[Right])){
		motor[Left] = ((startSpeed/abs(startSpeed)*40)+6);
		motor[Right] = ((startSpeed/abs(startSpeed)*40));
	}
	motor[Left] = minSpeed;
	motor[Right] = minSpeed;
}

void zeroEncoders()
{
	nMotorEncoder[Left] = 0;
	nMotorEncoder[Right] = 0;
}

task main()
{
	waitForStart();
	initializeRobot();
	zeroEncoders();
	accelerate(100, 2000, 0, 50);
	zeroEncoders();
	decelerate(0, 250, 100, 10);
	wait10Msec(100);
}
