/*
Use CAPITALIZED words with underscores for pragma config names.
Use camelCase for variables.
Capitalize the beginning of functions.
Don't use conditions like (rightDone == true).
Keep the style consistent.
*/
void SetMovePower(int power)
{
	motor[RIGHT_DRIVE1] = power;
	motor[RIGHT_DRIVE2_AND_3] = power;
	motor[LEFT_DRIVE1] = power;
	motor[LEFT_DRIVE2_AND_3] = power;
}

void LeftMove(int power)
{
	motor[LEFT_DRIVE1] = power;
	motor[LEFT_DRIVE2_AND_3] = power;
}

void RightMove(int power)
{
	motor[RIGHT_DRIVE1] = power;
	motor[RIGHT_DRIVE2_AND_3] = power;
}

void ChainBar(int power)
{
	motor[RIGHT_CHAIN_BAR] = power;
	motor[LEFT_CHAIN_BAR] = power;
}

void LineAlign(int lineThreshold, int power)
{
	bool hitLineRight = false;
	bool hitLineLeft = false;
	while(!hitLineLeft || !hitLineRight)
	{
		if(SensorValue[RIGHT_LINE_TRACKER] > lineThreshold && !hitLineRight)
		{
			RightMove(power);
		}
		else
		{
			hitLineRight = true;
			if (power > 0)
			{
				RightMove(-5);
			}
			else
			{
				RightMove(5);
			}
		}
		if(SensorValue[LEFT_LINE_TRACKER] > lineThreshold + 13 && !hitLineLeft)
		{
			LeftMove(power);
		}
		else
		{
			hitLineLeft = true;
			if (power > 0)
			{
				LeftMove(-5);
			}
			else
			{
				LeftMove(5);
			}
		}
	}
	SetMovePower(0);
}

void TrackLine(string *lineTracker, int lineThreshold, int rightPower, int leftPower)
{
	while(SensorValue[lineTracker] > lineThreshold)
	{
		RightMove(rightPower);
		LeftMove(leftPower);
	}
	SetMovePower(0);
}

void StopAtLine(int lineThreshold, int rightPower, int leftPower, int sensorPort)
{
	switch (sensorPort)
	{
		case 1:
			TrackLine("RIGHT_LINE_TRACKER", lineThreshold, rightPower, leftPower);
			break;
		case 6:
			TrackLine("FRONT_LINE_TRACKER", lineThreshold, rightPower, leftPower);
			break;
		case 8:
			TrackLine("LEFT_LINE_TRACKER", lineThreshold, rightPower, leftPower);
			break;
		default:
			break;
	}
}

void Move(int leftTurnValue,int leftPower,int rightTurnValue,int rightPower)
{
	SensorValue[RIGHT_SHAFT_ENCODER] = 0;
	SensorValue[LEFT_SHAFT_ENCODER] = 0;
	bool rightDone = false;
	bool leftDone = false;
	while(!rightDone || !leftDone)
	{
		if(abs(SensorValue[RIGHT_SHAFT_ENCODER]) < rightTurnValue && !rightDone)
		{
			RightMove(rightPower);
		}
		else
		{
			rightDone = true;
			RightMove(0);
		}
		if(abs(SensorValue[LEFT_SHAFT_ENCODER]) < leftTurnValue && !leftDone)
		{
			LeftMove(leftPower);
		}
		else
		{
			leftDone = true;
			LeftMove(0);
		}
	}
}

void TurnBySensor(int value, int leftPower, int rightPower, int sensorVal)
{
	int isClockWise;
	if (sensorVal > value)
	{
		isClockWise = 1;
	}
	else if (sensorVal < value)
	{
		isClockWise = -1;
	}
	else
	{
		isClockWise = 0;
	}

	int confirmLeftPower = (leftPower == 0) ? 0 : 1;
	int confirmRightPower = (rightPower == 0) ? 0 : 1;

	while (sensorVal > value + 200 * isClockWise)
	{
		RightMove(rightPower * isClockWise);
		LeftMove(leftPower * isClockWise);
	}
	while (sensorVal > value + 50 * isClockWise)
	{
		RightMove(-25 * isClockWise * confirmRightPower);
		LeftMove(25 * isClockWise * confirmLeftPower);
	}
	RightMove(25 * isClockWise * confirmRightPower);
	LeftMove(-25 * isClockWise * confirmLeftPower);
	wait1Msec(100);
	while (sensorVal < value - 40 * isClockWise)
	{
		RightMove(25 * isClockWise * confirmRightPower);
		LeftMove(-25 * isClockWise * confirmLeftPower);
	}
	SetMovePower(0);
}

void GyroTurn(int value, int power, const string oneWheel = "both")
{
	SensorValue[GYRO_SENSOR] = 0;
	if (oneWheel == "both")
	{
		TurnBySensor(value, power, -power, SensorValue[GYRO_SENSOR]);
	}
	else if (oneWheel == "left")
	{
		TurnBySensor(value, power, 0, SensorValue[GYRO_SENSOR]);
	}
	else if (oneWheel == "right")
	{
		TurnBySensor(value, 0, -power, SensorValue[GYRO_SENSOR]);
	}
}


void SetMogoLiftPower(int power)
{
	motor[LEFT_MOBILE_LIFT] = power;
	motor[RIGHT_MOBILE_LIFT] = power;
}

void MobileLift(int rightValue, int leftValue, int power)
{
	bool leftMobileDone = false;
	bool rightMobileDone = false;

	//Raising Mobile Goal--------------------------------------
	if (power > 0)
	{
		while (!leftMobileDone || !rightMobileDone)
		{
			//Right Side of Mobile Lift
			if (SensorValue[RIGHT_POTENTIOMETER] < rightValue)
			{
				motor[RIGHT_MOBILE_LIFT] = power;
			}
			else if (SensorValue[RIGHT_POTENTIOMETER] >= rightValue)
			{
				motor[RIGHT_MOBILE_LIFT] = 0;
				rightMobileDone = true;
			}

			//Left Side of Mobile Lift
			if (SensorValue[LEFT_POTENTIOMETER] < leftValue)
			{
				motor[LEFT_MOBILE_LIFT] = power;
			}
			else if (SensorValue[LEFT_POTENTIOMETER] >= leftValue)
			{
				motor[LEFT_MOBILE_LIFT] = 0;
				leftMobileDone = true;
			}
		}
	}
	//Lowering Mobile Goal------------------------------------
	else if (power < 0)
	{
		while (!leftMobileDone || !rightMobileDone)
		{
			//Right Side of Mobile Lift
			if (SensorValue[RIGHT_POTENTIOMETER] > rightValue)
			{
				motor[RIGHT_MOBILE_LIFT] = power;
			}
			else if (SensorValue[RIGHT_POTENTIOMETER] <= rightValue)
			{
				motor[RIGHT_MOBILE_LIFT] = 0;
				rightMobileDone = true;
			}

			//Left Side of Mobile Lift
			if (SensorValue[LEFT_POTENTIOMETER] > leftValue)
			{
				motor[LEFT_MOBILE_LIFT] = power;
			}
			else if (SensorValue[LEFT_POTENTIOMETER] <= leftValue)
			{
				motor[LEFT_MOBILE_LIFT] = 0;
				leftMobileDone = true;
			}
		}
	}
	else
	{
		motor[RIGHT_MOBILE_LIFT] = 0;
		motor[LEFT_MOBILE_LIFT] = 0;
	}
}

void PIMove(float targetL, float powerLimitL, float targetR, float powerLimitR,
						const string moveType = "straight", float kP = 0.1, float kI = 0.0002)
{
	SensorValue[LEFT_SHAFT_ENCODER] = 0;
	SensorValue[RIGHT_SHAFT_ENCODER] = 0;
	float errorL = 0;
	float errorR = 0;
	float errorTotalL = 0;
	float errorTotalR = 0;
	float proportionL = 0;
	float proportionR = 0;
	float integralL = 0;
	float integralR = 0;
	int powerL = 0;
	int powerR = 0;
	bool leftDone = false;
	bool rightDone = false;
	if (moveType == "turn")
	{
		kP = 0.6;//0.47 , 0.35
		kI = 0.0006;//0.0004 , 0.0011
	}
	while (!leftDone || !rightDone)
	{
		if (!leftDone)
	 	{
	 		errorL = targetL - (-SensorValue[LEFT_SHAFT_ENCODER]);
			proportionL = errorL * kP;
			errorTotalL += errorL;
			integralL = errorTotalL * kI;
			powerL = proportionL + integralL;
	    if ((powerL > powerLimitL && powerLimitL > 0) || (powerL < powerLimitL && powerLimitL < 0))
	    {
	    	powerL = powerLimitL;
	    }
	    LeftMove(powerL);
	    if ((-SensorValue[LEFT_SHAFT_ENCODER] < targetL + 3) && (-SensorValue[LEFT_SHAFT_ENCODER] > targetL - 3))
			{
				LeftMove(0);
				leftDone = true;
			}
	 	}
	 	if (!rightDone)
	 	{
	 		errorR = targetR - SensorValue[RIGHT_SHAFT_ENCODER];
			proportionR = errorR * kP;
			errorTotalR += errorR;
			integralR = errorTotalR * kI;
			powerR = proportionR + integralR;
	    if ((powerR > powerLimitR && powerLimitR > 0) || (powerR < powerLimitR && powerLimitR < 0))
	    {
	    	powerR = powerLimitR;
	    }
	    RightMove(powerR);
	    if ((SensorValue[RIGHT_SHAFT_ENCODER] < targetR + 3) && (SensorValue[RIGHT_SHAFT_ENCODER] > targetR - 3))
			{
				RightMove(0);
				rightDone = true;
			}
	 	}
	  wait1Msec(25);
	}
}

void PIGyroTurn(int target, int powerLimit, int threshold = 0,
								bool mogo = false, float kP = 0.1097, float kI = 0.0003)
{
	int absTarget = abs(target);
	float proportion = 0;
	float integral = 0;
	float error = 0;
	float errorTotal = 0;
	int power;
	powerLimit *= (target/absTarget);
	threshold = abs(threshold);

	if (mogo == true && kP == 0.1097 && kI == 0.0003)
	{
		kP = 0.17;//0.15
		kI = 0.002;//0.0072
	}

	SensorValue[GYRO_SENSOR] = 0;
	while (abs(SensorValue[GYRO_SENSOR]) < (absTarget - threshold))
	{
		error = target - SensorValue[GYRO_SENSOR];
		proportion = error * kP;
		power = proportion;
		if ((power > powerLimit && powerLimit > 0) || (power < powerLimit && powerLimit < 0))
		{
			power = powerLimit;
		}
		else
		{
			errorTotal += error;
			integral = errorTotal * kI;
			power += integral;
		}
		LeftMove(-power);
		RightMove(power);
		wait1Msec(25);
	}
	SetMovePower(0);
}

//const string type is not actually used...
void PIDGyroTurn(int target, bool mogo = false, bool reset = false, const string type = "both", int range = 4)
{
	if (reset)
	{
		SensorValue[GYRO_SENSOR] = 0;
	}
	float kP = 0.2;
 	float kI = 0.222;//0.22
	float kD = 1.6;
	int Lmode = 1;
	int Rmode = 1;
	int PowerLimit = 60;//60
	float error;
	float integral = 0;
	float derivative;
	float previous_error = 0;
	float position;
	float iteration_time = 25;
	float output;
	float test;
	int counter = 0;

	if (mogo)
	{
		kP = 0.25;
		kI = 0.26;//0.26
		kD = 1.6;//1.6
	}

	while (counter < 1)
	{
		position = SensorValue[GYRO_SENSOR];
		error = target - position;
		derivative = error - previous_error;
		test = kP*error + kD*derivative;
		integral += error;
		test = integral;
		if (abs(test) > PowerLimit)
		{
			integral = (integral/abs(test)) * PowerLimit;
		}
    output = kP*error + kI*integral + kD*derivative;
    previous_error = error;
    test = output;
    if (abs(test) > PowerLimit)
    {
    	output = (output/abs(test)) * PowerLimit;
    }
    datalogAddValue(1, target);
    datalogAddValue(2, kP * error);
    datalogAddValue(3, integral);
    datalogAddValue(4, derivative);
    datalogAddValue(5, counter);
  	LeftMove(-output * Lmode);
  	RightMove(output * Rmode);
    if (SensorValue[GYRO_SENSOR] > target - range && SensorValue[GYRO_SENSOR] < target + range)
    {
    	counter += 1;
    }
    wait1Msec(iteration_time);
	}
	SetMovePower(0);
}
