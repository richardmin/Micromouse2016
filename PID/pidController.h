#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include "mbed.h"
#include "pidConstants.h"
#include "AVEncoder/AVEncoder.h"

class pidController {
    public: 
    
        // Constructor
        pidController(AVEncoder* left, AVEncoder* right,
                      PwmOut* LForward, PwmOut* LReverse,
                      PwmOut* RReverse, PwmOut* RForward,
                      AnalogIn* IR_in_left_back, AnalogIn* IR_in_left_front, AnalogIn* IR_in_right_front, AnalogIn* IR_in_right_back,
                      DigitalOut* IR_out_left_back, DigitalOut* IR_out_left_front, DigitalOut* IR_out_right_front, DigitalOut* IR_out_right_back);
    
        // Main function in charge of calculating new motor speeds
        void pid();

        // Sets both motors to zero
        void stop();
        
        // Turns PID off
        void pause();  

        // Functions to control turning
        void turnLeft();
        void turnRight();
        void turnAround();

        // Moves forward exactly one cell
        void moveForwardOneCell();
        
        // Attempts to straighten the mouse
        void realign();
        
        // Starts the PID Timer
        void start();
        
        // Helper functions to drive the wires connected to the motors
        void setRightPwm(double speed); 
        void setLeftPwm(double speed);
        
    private:

        // Helper functions to determine PID correction
        double P_controller_IR(double error);
        double I_controller_IR(double error, unsigned int& integrator, int dt);
        double D_controller_IR(double error, double& prevError, int dt);
        
        // Make sure the speeds stay within the proper bounds
        void boundSpeeds();
        
        // Sensor variables
        AVEncoder* LeftEncoder;
        AVEncoder* RightEncoder;
        
        // Motor Controllers
        PwmOut* LMotorForward;
        PwmOut* LMotorReverse;
        PwmOut* RMotorForward;
        PwmOut* RMotorReverse;
        
        // IR Readings
        AnalogIn* IR_in_left_back;
        AnalogIn* IR_in_left_front;
        AnalogIn* IR_in_right_front;
        AnalogIn* IR_in_right_back;
        
        // IR Controllers
        DigitalOut* IR_out_left_back;
        DigitalOut* IR_out_left_front;
        DigitalOut* IR_out_right_front;
        DigitalOut* IR_out_right_back;

        // Variables that hold the relative speed of each motor
        double leftSpeed;
        double rightSpeed;
        
        // Determines if we are currently turning
        bool turning;
        
        // Derivative variables
        double prevIRError;
        
        // Integral variables
        unsigned int IRIntegrator;
        
        // IR error
        float left_IR_base;
        float right_IR_base;
        
        // Interrupt controller variables
        Timer timer;
        
        // Determines if we are currently running PID
        bool running;
};

#endif