#include "mbed.h"
#include "AVEncoder.h"
#include "Headers/pinouts.h"
#include "Headers/pidConstants.h"
#include "MazeMapper/mazeMapper.h"
#include "PID/pidController.h"

// Interrupt controller
Ticker interrupt;


//Turn variables
bool turn = false;

int main()  //
{    

	//set up IR
	IR_left_forward_pwr = 1;
	for(int i = 0; i < LEDOnTime; i++)
	{
		leftForwardLED += IR_left_front.read();
	}
	IR_left_forward_pwr = 0;
	
	IR_right_forward_pwr = 1;
	for(int i = 0; i < LEDOnTime; i++)
	{
		rightForwardLED += IR_right_front.read();
	}
	IR_right_forward_pwr = 0;

	IR_left_back_pwr = 1;
	for(int i = 0; i < LEDOnTime; i++)
	{
		leftBackLED += IR_left_back.read();
	}
	IR_left_back_pwr = 0;

	IR_right_front_pwr = 1;
	for(int i = 0; i < LEDOnTime; i++)
	{
		rightBackLED += IR_right_back.read();
	}
	IR_right_front_pwr = 0;
	
	printf("Initial right forward: %f\r\n", 1000*rightForwardLED);
	printf("Initial left forward: %f\r\n", 1000*leftForwardLED);
	printf("Initial right back: %f\r\n", 1000*rightBackLED);
	printf("Initial left back: %f\r\n", 1000*leftBackLED);
	//TODO: Use this to calibrate the IR Leds


    myled = 1;
	
	//wait for button press to start
	while(mybutton == 1);

    interrupt.attach_us(&pid, 1000);
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
