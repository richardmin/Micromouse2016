#include "mbed.h"
#include "AVEncoder.h"
#include "PINS/pinouts.h"
#include "MazeMapper/Maze.h"
#include "LED/LEDCollector.h"
#include "LED/IRLEDConstants.h"
#include "PID/pidController.h"
#include "PID/pidConstants.h"

DigitalOut myled(LED1);
DigitalIn mybutton(USER_BUTTON);

//RNG from https://developer.mbed.org/questions/61158/Random-number-in-C/
AnalogIn rng_read (RNG_SEED);

float leftForwardLED;
float rightForwardLED;
float leftBackLED;
float rightBackLED;

float leftForwardLEDHistory[LED_HIST_SIZE];
float leftBackLEDHistory[LED_HIST_SIZE];
float rightForwardLEDHistory[LED_HIST_SIZE];
float rightBackLEDHistory[LED_HIST_SIZE];

float initialLeftForwardLED;
float initialRightForwardLED;
float initialLeftBackLED;
float initialRightBackLED;

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

int currHistPosition;

void pollLEDs(int milliseconds)
{
    leftForwardLED = 0;
    rightForwardLED = 0;
    leftBackLED = 0;
    rightBackLED = 0;

    IR_emitter2 = 1;
    for(int i = 0; i < milliseconds; i++)
    {
        leftForwardLED += IR_receiver2.read()*1000;
    }
    leftForwardLED /= milliseconds;

    IR_emitter3 = 1;
    for(int i = 0; i < milliseconds; i++)
    {
        rightForwardLED += IR_receiver3.read()*1000;
    }
    rightForwardLED /= milliseconds;

    IR_emitter1 = 1;
    for(int i = 0; i < milliseconds; i++)
    {
        leftBackLED += IR_receiver1.read()*1000;
    }
    leftBackLED /= milliseconds;

    IR_emitter4 = 1;
    for(int i = 0; i < milliseconds; i++)
    {
        rightBackLED += IR_receiver4.read()*1000;
    }
    rightBackLED /= milliseconds;
    
    leftForwardLEDHistory[currHistPosition] = leftForwardLED;
    leftBackLEDHistory[currHistPosition] = leftBackLED;
    rightForwardLEDHistory[currHistPosition] = rightForwardLED;
    rightBackLEDHistory[currHistPosition] = rightBackLED;

    currHistPosition++;

    if(currHistPosition >= LED_HIST_SIZE)
        currHistPosition = 0;
    
    // detectWallChanges();
}

void detectWallChanges()
{
    //analyze the LED History, seeing if the walls are changing.
    //This is one way of formatively creating the walls of the maze; the other way would be to monitor our location
    //and check if there's a wall each time we enter another square.
    //In fact it's probably simpler to implement it that way.
    //This method provides the small speed benefit of immediately knowing when the walls are changing patterns. 
    //However, it does take more memory
    
}

bool wallInFront()
{
    printf("wall in front: %f %f\r\n", leftForwardLED, rightForwardLED);
    return (leftForwardLED >= WALL_IN_FRONT_LEFT || rightForwardLED >= WALL_IN_FRONT_RIGHT);
}

bool wallToLeft()
{
    printf("wall in left: %f\r\n", leftBackLED);
    return (leftBackLED >= WALL_TO_LEFT);
}

bool wallToRight()
{
    printf("wall in right: %f\t%d\r\n", rightBackLED, WALL_TO_RIGHT);
    return (rightBackLED >= WALL_TO_RIGHT);
}

void rightWallFollower(pidController pid)
{
    IR_emitter1 = 1;
    IR_emitter2 = 1;
    IR_emitter3 = 1;
    IR_emitter4 = 1;
    while(1)
    {
        pid.pid();
        pollLEDs(10);
        bool front = wallInFront();
        bool right = wallToRight();

        if(right)
        {
            pid.turnRightFromMoving();
            pid.moveForwardOneCellNotMoving();
        }
        else if(!front)
        {
            //continue
        }
        else
        {
            pid.turnAround(); //should continue forwards.
        }
    }
}

void leftWallFollower(pidController pid)
{
    IR_emitter1 = 1;
    IR_emitter2 = 1;
    IR_emitter3 = 1;
    IR_emitter4 = 1;
    while(1)
    {
        pid.pid();
        pollLEDs(10);
        bool front = wallInFront();
        bool left = wallToLeft();

        if(left)
        {
            pid.turnLeftFromMoving();
            pid.moveForwardOneCellNotMoving();
        }
        else if(!front)
        {
            //continue
        }
        else
        {
            pid.turnAround();
        }
    }
}

void FourIRRandom(pidController pid)
{
    IR_emitter1 = 1;
    IR_emitter2 = 1;
    IR_emitter3 = 1;
    IR_emitter4 = 1;
    while(1)
    {
        pid.pid();
        pollLEDs(10);
        bool front = wallInFront();
        bool left = wallToLeft();
        bool right = wallToRight();
        int arr[3] = {0, 0, 0};
        int index = 0;

        // I AIN'T NO CS MAJOR
        if(!front)
            arr[index++] = 1;
        if(!right)
            arr[index++] = 2;
        if(!left)
            arr[index++] = 3;

        if(index == 0) // dunno how you got here but that means you have a dead end
        {
            printf("TURN AROUND\r\n");
            pid.turnAround();
            continue;
        }

        //pick the random function
        int funcNum = rand() % index;
        switch(arr[funcNum])
        {
            case 1:
                printf("STRAIGHT\r\n");
                break;
            case 2:
                printf("RIGHT\r\n");
                pid.turnRightFromMoving();
                pid.moveForwardOneCellNotMoving();
                break;
            case 3:
                printf("LEFT\r\n");
                pid.turnLeftFromMoving();
                pid.moveForwardOneCellNotMoving();
                break;
        }


    }
}

void TwoIRRandom(pidController pid)
{
    IR_emitter2 = 1;
    IR_emitter3 = 1;
    while(1)
    {
        pollLEDs(10);
        int i = 0;
        int j;
        while(!wallInFront())
        {
            i++;
            pollLEDs(10);
            pid.moveForwardOneCellNotMoving();
            if(i > 1000)
            {
                for(j = 0; j < 500; j++)
                {
                    pid.setLeftPwm(-0.1);
                    pid.setRightPwm(-0.12);
                }
                i = 0;
            }
        }
        
        while(IR_receiver2.read()*1000 > 920 || IR_receiver3.read()*1000 > 920)
        {
            pid.setLeftPwm(-0.1);
            pid.setRightPwm(-0.12);
        }
        pid.stop();
        
        switch(rand()%2)
        {
            case 0:
                pid.turnRight();
                break;
            case 1:
                pid.turnLeft();
                break;
        }
    }
}
 
int main()  
{           
    uint32_t seed =0;
    const int loops = 1; // increase if LSB has a bias. 
     
    for (int i=0;i<(32*loops);i++) {
      seed ^= rng_read.read_u16();
      if (seed & 1<31) { // shift left and wrap.
        seed <<= 1;
        seed |= 1;
      } else
        seed <<= 1;
    }
    srand(seed);

    // PID controller
    pidController pid = pidController(&LeftEncoder, &RightEncoder,
                                      &LMotorForward, &LMotorReverse,
                                      &RMotorForward, &RMotorReverse,
                                      &IR_receiver1, &IR_receiver2, &IR_receiver3, &IR_receiver4,
                                      &IR_emitter1, &IR_emitter2, &IR_emitter3, &IR_emitter4);
    // Interrupt controller
    Ticker interrupt;
    
    interrupt.attach_us(&pid, &pidController::pid, 5000);
    
    while(mybutton)
        ;

    //Intialize final things
    pid.start(); 

    while(1)
    {
        ;
    }
}
