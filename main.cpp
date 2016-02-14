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
                
        if(1000*IR_right_back.read() < rightLED - 50)
        {
        	turnRight();
        }
        else if(1000*IR_left_back.read() < leftLED - 50)
        {
        	turnLeft();
        }
    }
}
