#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include "mbed.h"
#include "pidConstants.h"
#include "Gyro/Gyro.h"
#include "AVEncoder/AVEncoder.h"

class pidController {
    public: 
    
        // Constructor
        pidController(Gyro* g, 
                      AVEncoder* left, AVEncoder* right,
                      PwmOut* LForward, PwmOut* LReverse,
                      PwmOut* RReverse, PwmOut* RForward);
    
        // Main function in charge of calculating new motor speeds
        void pid();

        // Sets both motors to zero
        void stop();

        // Functions to control turning
        void turnRight();
        void turnLeft();
        
        // Starts the PID Timer
        void start();
        
    private:

        // Helper functions to determine PID correction
        double P_controller(double error);
        double I_controller(double error, double& integrator);
        double D_controller(double error, double& prevError, int dt);
        
        // Make sure the speeds stay within the proper bounds
        void boundSpeeds();

        // Helper functions to drive the wires connected to the motors
        void setRightPwm(double speed); 
        void setLeftPwm(double speed);
        
        // Sensor variables
        Gyro* gyro;
        AVEncoder* LeftEncoder;
        AVEncoder* RightEncoder;
        
        // Motor Controllers
        PwmOut* LMotorForward;
        PwmOut* LMotorReverse;
        PwmOut* RMotorForward;
        PwmOut* RMotorReverse;

        // Variables that hold the relative speed of each motor
        double leftSpeed;
        double rightSpeed;
        
        // Determines if we are currently turning
        bool turning;
        
        // Derivative variables
        double prevTranslationalError;
        double prevAngularError;
        
        // Integral variables
        double translationalIntegrator;
        double angularIntegrator;
        
        // Interrupt controller variables
        Timer timer;
};


#endif