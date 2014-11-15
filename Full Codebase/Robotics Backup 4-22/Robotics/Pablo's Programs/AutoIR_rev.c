#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  none,     none)
#pragma config(Sensor, S3,     infared,        sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     Left,          tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     Right,         tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     Lift1,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Lift2,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_1,     Sweep,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_2,     Flag,          tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S2_C1_1,    bucket,               tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.

float in = 114.649682;
float deg = 16;
//float a = 0; //just a var, used in AutoIR_rev 2


void initializeRobot()
{
	servo[bucket] = 130;
}
/* used in AutoIR_rev2
void move(int power, float dist)//"+" =foward  "-" =reverse, dist is in inches
{
	int dir = 1;
	if (power < 0)
	{
		dir = -1;
	}
	power = abs(power);
	dist = dist*in/3;

	nMotorEncoder[Left] = 0;
  while(abs(nMotorEncoder[Left]) < dist)//acceleration
  {
  	a = 15+power*(abs(nMotorEncoder[Left])/dist);
  	motor[Left] = dir*a;
   	motor[Right] = dir*a;
  }
  nMotorEncoder[Left] = 0;
  while(abs(nMotorEncoder[Left]) < dist)//constant speed
  {
  	motor[Left] = dir*a;
   	motor[Right] = dir*a;
  }
  nMotorEncoder[Left] = 0;
  while(abs(nMotorEncoder[Left]) <dist)//deceleration
  {
  	a = 15+power-power*(abs(nMotorEncoder[Left])/dist);
  	motor[Left] = dir*a;
   	motor[Right] = dir*a;
  }
}

void turn(int power, float dist)// "+" =left  "-" =right, dist is in inches
{
	int left = 1;
	int right = -1;
	if (power < 0)
	{
		left = -1;
	  right = 1;
	}
	power = abs(power);
	dist = dist*deg/3;

	nMotorEncoder[Left] = 0;
  while(abs(nMotorEncoder[Left]) < dist)//acceleration
  {
  	a = 15+power*(abs(nMotorEncoder[Left])/dist);
  	motor[Left] = left*a;
   	motor[Right] = right*a;
  }
  nMotorEncoder[Left] = 0;
  while(abs(nMotorEncoder[Left]) < dist)//constant speed
  {
  	motor[Left] = left*a;
   	motor[Right] = right*a;
  }
  nMotorEncoder[Left] = 0;
  while(abs(nMotorEncoder[Left]) < dist)//deceleration
  {
  	a = 15+power-power*(abs(nMotorEncoder[Left])/dist);
  	motor[Left] = left*a;
   	motor[Right] = right*a;
  }
}
*/

task main()
{
  initializeRobot();

  waitForStart();

  int ir = 0;
  int wait = 500;
  int speed = 25;//speed of robot while moving, changes after dump cube
  int tspeed = 100;// speed of robot while turning

  //check beacon 1
  //line up with beacon 1
  nMotorEncoder[Right] = 0;
  while(abs(nMotorEncoder[Right]) < 18*in)//HAS TO BE A ADJUSTED!
  {
  	motor[Left] = -speed;
   	motor[Right] = -speed;
  }
  motor[Left] = 0;
  motor[Right] = 0;
  wait1Msec(wait);
  //check for beacon 1
  if (SensorValue[infared] == 5)
  {
    ir = 1;//beacon is on goal 1
  }
  if (ir == 0)//check beacon 2
  {
    //line up with beacon 2
    nMotorEncoder[Right] = 0;
    while(abs(nMotorEncoder[Right]) < 10*in)
    {
    	motor[Left] = -speed;
    	motor[Right] = -speed;
    }
    motor[Left] = 0;
    motor[Right] = 0;
    wait1Msec(wait);
    //check for beacon 2
    if (SensorValue[infared] == 5)
    {
      ir = 2;//beacon is on goal 2
    }
  }
  if (ir == 0)// check beacon 3
  {
    //line up with beacon 3
    nMotorEncoder[Right] = 0;
    while(abs(nMotorEncoder[Left]) < 20*in)
    {
    	motor[Left] = -speed;
    	motor[Right] = -speed;
    }
    motor[Left] = 0;
    motor[Right] = 0;
    wait1Msec(wait);
    //check for beacon 3
    if (SensorValue[infared] == 5)
    {
      ir = 3;//beacon is on goal 3
    }
  }
  if (ir == 0)//check beacon 4
  {
    //line up with beacon 4
    nMotorEncoder[Right] = 0;
    while(abs(nMotorEncoder[Left]) < 10*in)
    {
    	motor[Left] = -speed;
    	motor[Right] = -speed;
    }
    motor[Left] = 0;
    motor[Right] = 0;
    wait1Msec(wait);
    //check for beacon 4
    if (SensorValue[infared] == 5)
    {
      ir = 4;//beacon is on goal 4
    }
  }

  //align with goal
  //goal 3 and 4 are already aligned
  if (ir == 1 || ir == 2)//align 1 or 2
  {
  	nMotorEncoder[Right] = 0;
    while(abs(nMotorEncoder[Right]) < 1.5*in)
    {
      motor[Left] = -speed;
      motor[Right] = -speed;
    }
    motor[Left] = 0;
    motor[Right] = 0;
    wait1Msec(wait);
  }
  if (ir == 0)//if ir was not found, align with goal 1
  {
    nMotorEncoder[Right] = 0;
    while(abs(nMotorEncoder[Right]) < 39*in)
    {
      motor[Left] = -speed;
      motor[Right] = -speed;
    }
    motor[Left] = 0;
    motor[Right] = 0;
    wait1Msec(wait);
  }

  //dump cube
  motor[Lift1] = 100;
  motor[Lift2] = 100;
  wait1Msec(1850);
  motor[Lift1] = 0;
  motor[Lift2] = 0;
  wait10Msec(5);
  servo[bucket] = 230;
  wait1Msec(1000);
  servo[bucket] = 125;
  wait10Msec(100);
  motor[Lift1] = -100;
  motor[Lift2] = -100;
  wait1Msec(1400);
  motor[Lift1] = 0;
  motor[Lift2] = 0;

  speed=40;

  //back up, center 18in away from end of pendulum, starting side
  if (ir == 0 || ir == 1)
  {
    nMotorEncoder[Right] = 0;
    while(abs(nMotorEncoder[Right]) < 22.5*in)
    {
   	  motor[Left] = speed;
      motor[Right] = speed;
    }
    motor[Left] = 0;
    motor[Right] = 0;
    wait1Msec(wait);
  }
  if (ir == 2)
  {
    nMotorEncoder[Right] = 0;
    while(abs(nMotorEncoder[Right]) < 32.5*in)
    {
      motor[Left] = speed;
      motor[Right] = speed;
    }
    motor[Left] = 0;
    motor[Right] = 0;
    wait1Msec(wait);
  }
  if (ir == 3)
  {
    nMotorEncoder[Right] = 0;
    while(abs(nMotorEncoder[Right]) < 51.5*in)
    {
      motor[Left] = speed;
      motor[Right] = speed;
    }
    motor[Left] = 0;
    motor[Right] = 0;
    wait1Msec(wait);
  }
  if (ir == 4)
  {
    nMotorEncoder[Right] = 0;
    while(abs(nMotorEncoder[Right]) < 61.5*in)
    {
      motor[Left] = speed;
      motor[Right] = speed;
    }
    motor[Left] = 0;
    motor[Right] = 0;
    wait1Msec(wait);
  }

  //turn right 90deg
  nMotorEncoder[Right] = 0;
  while(abs(nMotorEncoder[Right]) < 90*deg)
  {
    motor[Left] = tspeed;
    motor[Right] = -tspeed;
  }
  motor[Left] = 0;
  motor[Right] = 0;
  wait1Msec(wait);

  //move infront of ramp
  nMotorEncoder[Left] = 0;
  while(abs(nMotorEncoder[Right]) < 32*in)
  {
    motor[Left] = speed;
    motor[Right] = speed;
  }
  motor[Left] = 0;
  motor[Right] = 0;
  wait1Msec(wait);

  //turn so back faces ramp
  nMotorEncoder[Right] = 0;
  while(abs(nMotorEncoder[Right]) < 90*deg)
  {
    motor[Left] = -speed;
    motor[Right] = speed;
  }
  motor[Left] = 0;
  motor[Right] = 0;
  wait1Msec(wait);

  //drive up ramp
  nMotorEncoder[Left] = 0;
  while(abs(nMotorEncoder[Right]) < 34*in)
  {
    motor[Left] = -speed;
    motor[Right] = -speed;
  }
  motor[Left] = 0;
  motor[Right] = 0;
  wait1Msec(wait);

}
// this is now 320 lines long.
