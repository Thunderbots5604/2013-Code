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


//version 5.0.0 alpha

#include "JoystickDriver.c"

void setMovementPower_normal(int power, int turn){
	int leftBreak = 1;
	int rightBreak = 1;
	if (joy1Btn(7))
		leftBreak = 0;
	if (joy1Btn(8))
		rightBreak = 0;
	motor[Left2] = leftBreak*(power + turn);
	motor[Right2] = rightBreak*(power - turn);
	motor[Left] = leftBreak*(power + turn);
	motor[Right] = rightBreak*(power - turn);
}

int correctInputs(int input){
	if (abs(input) < 30)
		input = 0;
	return input;
}

void initializeRobot(){
	servo[bucket] = 135;
  setMovementPower_normal(0,0);
  servo[pin] = 0;
}

task main()
{

	initializeRobot();
	short servoMovement = 40; //might be 63


  while(true){

  	getJoystickSettings(joystick);


  	setMovementPower_normal(correctInputs(joystick.joy1_y1) * 4/5, correctInputs(joystick.joy1_x2) * 4/5);

  	//time-out (for lines of code that have been naughty)
  	/*bucketServoPos = bucketServoPos + (correctInputs(joystick.joy2_x1) / 25.5);

  	if (bucketServoPos > 230) //controls the servo that buckets the blocky thingy (not Minecraft-related)
  		bucketServoPos = 230;
  	if (bucketServoPos < 40)
  		bucketServoPos = 40;
  	if (servo[bucket] != bucketServoPos)
  		servo[bucket] = bucketServoPos;*/
  	if (joy2Btn(1))//this controls the bucket servo
  		servo[bucket] = 135-servoMovement;
  	if (joy2Btn(3))
  		servo[bucket] = 135+servoMovement;
  	if (joy2Btn(2))
  		servo[bucket] = 135;

  	if (!joy2Btn(8) && !joy2Btn(6) && !joy2Btn(4)) //controls sweeping motor
  		motor[Sweep] = 0;
 	 	if (joy2Btn(8))
  		motor[Sweep] = 100;
  	if (joy2Btn(6))
  		motor[Sweep] = -100;
  	if (joy2Btn(4))
  		motor[Sweep] = -5;

		if (!joy2Btn(7) && !joy2Btn(5)) //controls flag motor
  		motor[Flag] = 0;
  	if (joy2Btn(7))
  		motor[Flag] = 6;
  	if (joy2Btn(5))
  		motor[Flag] = 100;

  	if (!joy1Btn(5) && !joy1Btn(6)){//controls bucket lift motor
  		motor[Lift1] = 0;
  		motor[Lift2] = 0;
  	}
		if (joy1Btn(5)){
			motor[Lift1] = -100;
			motor[Lift2] = -100;
		}

  	if (SensorValue[touch]){
  		if (joy1Btn(5)){
  			motor[Lift1] = 0;
  			motor[Lift2] = 0;
  		}
  	}

  	if (joy1Btn(6)){
  		motor[Lift1] = 100;
  		motor[Lift2] = 100;
  	}
  	if (joy1Btn(2))
  		servo[pin] = 28;
  	if (joy1Btn(3))
  		servo[pin] = 0;

  }

}
