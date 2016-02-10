#include "mbed.h"
#include "AVEncoder.h"
#include "Headers/pinouts.h"
#include "Headers/pidConstants.h"

#include "PID/pidController.h"

// Interrupt controller
Ticker interrupt;

// IR variables
float leftLED = 0;
float rightLED = 0;

//Turn variables
bool turn = false;

int main() 
{    

	//set up IR
	IR_left_pwr = 1;
	for(int i = 0; i < 1000; i++)
	{
		leftLED += IR_left.read();
	}
	IR_left_pwr = 0;
	
	IR_right_pwr = 1;
	for(int i = 0; i < 1000; i++)
	{
		rightLED += IR_right.read();
	}
	IR_right_pwr = 0;
	
	printf("Initial right: %f\r\n", 1000*rightLED);
	printf("Initial left: %f\r\n", 1000*leftLED);
	


    myled = 1;
	
	//wait for button press to start
	while(mybutton == 1);
    //interrupt.attach_us(&pid, 1000);
    timer.start();
    
    leftSpeed = 0.48;
    rightSpeed = 0.5;
    
    IR_left_pwr = 1;
    IR_right_pwr = 1;
    
    while(1)
    {
        setLeftPwm(leftSpeed);
        setRightPwm(rightSpeed);
                
        if(1000*IR_right.read() < rightLED - 50)
        {
        	turnRight();
        }
        else if(1000*IR_left.read() < leftLED - 50)
        {
        	turnLeft();
        }
    }
}
