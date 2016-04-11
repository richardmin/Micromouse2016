#include "mbed.h"
#include "AVEncoder.h"
#include "PINS/pinouts.h"
#include "MazeMapper/Maze.h"
#include "MazeMapper/Location.h"
#include "LED/LEDCollector.h"
#include "PID/pidController.h"
#include "PID/pidConstants.h"
#include "Gyro/Gyro.h"


int main()  
{        
    Gyro gyro;
    
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
    pidController pid = pidController(&gyro,
                                      &LeftEncoder, &RightEncoder,
                                      &LMotorForward, &LMotorReverse,
                                      &RMotorForward, &RMotorReverse);
                                      
    // Interrupt controller
    Ticker interrupt;
    
    Location curLoc = Location(0, 0);
    
    //Intialize final things
    pid.start();
    interrupt.attach_us(&pid, &pidController::pid, 1000);
    
    while(1)
    {
        wait(2.0);
        pid.turnLeft();
        wait(2.0);
        pid.turnLeft();
        wait(2.0);
        pid.turnLeft();
        wait(2.0);
        pid.turnLeft();
        wait(2.0);
        
        
        wait(2.0);
        pid.turnRight();
        wait(2.0);
        pid.turnRight();
        wait(2.0);
        pid.turnRight();
        wait(2.0);
        pid.turnRight();
        wait(2.0);
    }

    /*while(1); //back up straight, until we give go ahead to PIDs

    //this way the LED's get initialized properly
    MazeRunner runner = MazeRunner();
    MazeMapper mapper = MazeMapper();
    
    //prepare pid
    interrupt.attach_us(&pid.pid(), 1000);
    timer.start();
    gyroTimer.start();
    
    bool hasMapped = false;

    while(!hasMapped) //do mapping mode until we decide otherwise
    {
        
    }

    //attach an interrrupt for a button to reset run (location reset to 0)
    */
}
