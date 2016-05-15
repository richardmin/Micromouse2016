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
            
            pid.moveForwardOneCellNotMoving();
            
            pid.turnLeft();
//            pid.moveForwardOneCellNotMoving();

            myled = 0;
        }
        wait(0.005);
    }

}
