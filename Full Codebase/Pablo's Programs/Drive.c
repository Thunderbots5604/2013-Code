#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S3,     infared,        sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     Left,          tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     Right,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     Lift1,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Lift2,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     Sweep,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     Flag,          tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C4_1,    Bucket,               tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.

float in = 114.649682;
float deg = 16;

void initializeRobot()
{
	servo[Bucket] = 130;
}

task main()
{
	initializeRobot();
	waitForStart();
	float speed = 0;
	float turn = 0;
	float accelspeed = 0.3;
	float accelturn = 0.5;

	while(true)
	{
		getJoystickSettings(joystick);
		int targetspeed = joystick.joy1_y1*4/5;
		int targetturn = joystick.joy1_x2*4/5;
		if (abs(targetspeed) < 25)
			targetspeed = 0;
    if (abs(targetturn) < 25)
    	targetturn = 0;

    if (targetspeed > speed)
    	speed = speed+accelspeed;
    if (targetspeed < speed)
    	speed = speed-accelspeed;

    if (targetturn > turn)
    	turn = turn+accelturn;
    if (targetturn < turn)
    	turn = turn-accelturn;

    motor[Left] = speed +turn;
    motor[Right] = speed -turn;
  }
}