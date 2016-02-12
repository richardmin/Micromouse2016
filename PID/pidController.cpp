void pid()
{
	if(turn)
		return;
	
	int error = 0;
	double change = 0.0;
	
	error = (LeftEncoder.getPulses()/2 - (RightEncoder.getPulses()-12));
		
    change = abs(P_controller(error) + D_controller(error) + I_controller(error));
    
    if(error > 0)
    {
    	leftSpeed -= change;
    	rightSpeed += change;
    }
    else
    {
    	leftSpeed += change;
    	rightSpeed -= change;
    }
    
    if(leftSpeed < 0)
    {
        leftSpeed = 0;
    }
    else if(leftSpeed > 1)
    {
        leftSpeed = 1;
    }
    
    if(rightSpeed < 0)
    {
        rightSpeed = 0;
    }
    else if(rightSpeed > 1)
    {
        rightSpeed = 1;
    }
    
    //printf("ERROR: %d\tCHANGE: %f\r\nLEFT: %d\r\nRIGHT: %d\r\n\r\n", error, change, LeftEncoder.getPulses()/2, RightEncoder.getPulses()-12);
    //printf("LEFT: %f\tRIGHT: %f\r\n", leftSpeed, rightSpeed);
	
	LeftEncoder.reset();
	RightEncoder.reset();
	timer.reset();
}

double P_controller(int error)
{
    return (kp*error);
}

double D_controller(int error)
{	
	double dError = error - prevError;
		
	int dt = timer.read_us();
	
	timer.reset();
	prevError = error;
	
	return kd*dError/dt;
}

double I_controller(int error)
{
	integrator += error;
	double correction = ki * integrator;
	integrator /= decayFactor;
	
    return correction;
}

void setLeftPwm(double speed) 
{
    if(speed == 0)
    {
        LMotorForward = 0;
        LMotorReverse = 0;
    }
    if(speed > 0)
    {
        LMotorForward = speed;
        LMotorReverse = STOP;
    }
    if(speed < 0)
    {
        LMotorForward = STOP;
        LMotorReverse = speed;
    }
}

//todo: make this have ramping
void setRightPwm(double speed) 
{
    if(speed > 0)
    {
        RMotorForward = speed;
        RMotorReverse = STOP;
    }
    if(speed < 0)
    {
        RMotorForward = STOP;
        RMotorReverse = speed;
    }
}

void stop()
{
    setLeftPwm(0);
    setRightPwm(0);
}

void turnLeft()
{
	for(int i = 0; i < 1000; i++)
	{
		setLeftPwm(leftSpeed);
		setRightPwm(rightSpeed);
	}
	
	turn = true;
	for(int i = 0; i < 100; i++)
	{
		setLeftPwm(0);
		setRightPwm(0.5);
	}
	turn = false;
	
	for(int i = 0; i < 1000; i++)
	{
		setLeftPwm(leftSpeed);
		setRightPwm(rightSpeed);
	}
}

void turnRight()
{
	for(int i = 0; i < 1000; i++)
	{
		setLeftPwm(leftSpeed);
		setRightPwm(rightSpeed);
	}
	
	turn = true;
	for(int i = 0; i < 100; i++)
	{
		setLeftPwm(0.5);
		setRightPwm(0);
	}
	turn = false;
	
	for(int i = 0; i < 100; i++)
	{
		setLeftPwm(leftSpeed);
		setRightPwm(rightSpeed);
	}
}