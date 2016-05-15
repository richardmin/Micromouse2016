#include "mbed.h"
#include "AVEncoder.h"
#include "PINS/pinouts.h"
#include "MazeMapper/Maze.h"
#include "LED/LEDCollector.h"
#include "PID/pidController.h"
#include "PID/pidConstants.h"
#include "Gyro/Gyro.h"



DigitalOut myled(LED1);
DigitalIn mybutton(USER_BUTTON);

//RNG from https://developer.mbed.org/questions/61158/Random-number-in-C/
AnalogIn rng_read (RNG_SEED);
 
unsigned int random_generator (void)
{
    unsigned int x = 0;
    unsigned int iRandom = 0;
    
    
    
    for (x = 0; x <= 32; x += 2)
    {
        iRandom += ((rng_read.read_u16() % 3) << x);
        wait_us (10);
    }
    
    return iRandom;
}


int main()  
{           
    // Controller for IR receivers
    AnalogIn IR_receiver1(IR_LEFT_BACK);
    AnalogIn IR_receiver2(IR_LEFT_FRONT);
    AnalogIn IR_receiver3(IR_RIGHT_FRONT);
    AnalogIn IR_receiver4(IR_RIGHT_BACK);
    
    // Controller for IR emitters
    DigitalOut IR_emitter1(IR_LEFT_BACK_PWR);
    DigitalOut IR_emitter2(IR_LEFT_FRONT_PWR);
    DigitalOut IR_emitter3(IR_RIGHT_FRONT_PWR);
    DigitalOut IR_emitter4(IR_RIGHT_BACK_PWR);
    
    // Controllers for left and right encoders
    AVEncoder LeftEncoder(L_ENCODER_A, L_ENCODER_B);
    AVEncoder RightEncoder(R_ENCODER_A, R_ENCODER_B);
    
    // Controllers for right motor
    PwmOut RMotorReverse(R_MOTOR_REVERSE);
    PwmOut RMotorForward(R_MOTOR_FORWARD);
    
    // Controllers for left motor
    PwmOut LMotorForward(L_MOTOR_FORWARD);
    PwmOut LMotorReverse(L_MOTOR_REVERSE);
    
    // PID controller
    pidController pid = pidController(&LeftEncoder, &RightEncoder,
                                      &LMotorForward, &LMotorReverse,
                                      &RMotorForward, &RMotorReverse,
                                      &IR_receiver1, &IR_receiver2, &IR_receiver3, &IR_receiver4,
                                      &IR_emitter1, &IR_emitter2, &IR_emitter3, &IR_emitter4);
    // Interrupt controller
    Ticker interrupt;
    
    //interrupt.attach_us(&pid, &pidController::pid, 5000);
    
    while(mybutton);

    //Intialize final things
    pid.start(); 

    while(1)
    {
        pid.pid();
        IR_emitter1 = 1;
        float val = 0;
        for(int i = 0; i < 10; i++)
        {
            val += 1000*IR_receiver1.read();
        }
        val /= 10;
        printf("%f\r\n", val);
        if(val < 160)
        {
            myled = 1;
            pid.turnLeftFromMoving();
            
//            pid.moveForwardOneCellNotMoving();
//            
//            pid.turnLeft();
//            pid.moveForwardOneCellNotMoving();
            pid.pause();

            myled = 0;
        }
        wait(0.005);
    }


    


    //RANDOM MODE
    IR_emitter1 = 1;
    IR_emitter2 = 1;
    IR_emitter3 = 1;
    IR_emitter4 = 1;
    LEDCollector led;
    while(1)
    {
        pid.pid();
        led.pollLEDs(10);
        bool front = led.wallInFront();
        bool left = led.wallToLeft();
        bool right = led.wallToRight();
        int arr[3] = {0, 0, 0};
        int index = 0;


        //I AIN'T NO CS MAJOR
        if(front)
            arr[index++] = 1;
        if(right)
            arr[index++] = 2;
        if(left)
            arr[index++] = 3;

        if(index == 0) //dunno how you got here but that means you have a dead end
        {
            pid.turnAround();
            continue;
        }

        int rng = random_generator();

        //pick the random function
        int funcNum = rng % index;
        switch(arr[funcNum])
        {
            case 1:
            pid.moveForwardOneCell();
            continue; //pid continues forward.
            break;
            case 2:
            pid.turnRightFromMoving();
            pid.moveForwardOneCellNotMoving();
            continue;
            break;
            case 3:
            pid.turnLeftFromMoving();
            pid.moveForwardOneCellNotMoving();
            continue;
            break;
        }


    }
    

    //LEFT WALL FOLLOWER
    IR_emitter1 = 1;
    IR_emitter2 = 1;
    IR_emitter3 = 1;
    IR_emitter4 = 1;
    LEDCollector led;
    while(1)
    {
        pid.pid();
        led.pollLEDs(10);
        bool front = led.wallInFront();
        bool left = led.wallToLeft();

        if(left)
        {
            pid.turnLeftFromMoving();
            pid.moveForwardOneCellNotMoving();
        }
        else if(front)
        {
            //continue
        }
        else
        {
            pid.turnAround();
        }


    }


    //RIGHT WALL FOLLOWER
    IR_emitter1 = 1;
    IR_emitter2 = 1;
    IR_emitter3 = 1;
    IR_emitter4 = 1;
    LEDCollector led;
    while(1)
    {
        pid.pid();
        led.pollLEDs(10);
        bool front = led.wallInFront();
        bool right = led.wallToRight();

        if(right)
        {
            pid.turnRightFromMoving();
            pid.moveForwardOneCellNotMoving();
        }
        else if(front)
        {
            //continue
        }
        else
        {
            pid.turnAround(); //should continue forwards.
        }


    }

    /*while(1); //back up straight, until we give go ahead to PIDs

    //this way the LED's get initialized properly
    MazeRunner runner = MazeRunner();
    MazeMapper mapper = MazeMapper();
    
    bool hasMapped = false;

    while(!hasMapped) //do mapping mode until we decide otherwise
    {
        
    }

    //attach an interrrupt for a button to reset run (location reset to 0)
    */
}
