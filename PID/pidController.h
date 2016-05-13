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
        double P_controller_translational(double error);
        double I_controller_translational(double error, unsigned int& integrator, int dt);
        double D_controller_translational(double error, double& prevError, int dt);
        double P_controller_angular(double error);
        double I_controller_angular(double error, unsigned int& integrator, int dt);
        double D_controller_angular(double error, double& prevError, int dt);
        
        // Make sure the speeds stay within the proper bounds
        void boundSpeeds();

        // Helper functions to drive the wires connected to the motors
        void setRightPwm(double speed); 
        void setLeftPwm(double speed);
        
        // Sensor variables
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
        unsigned int translationalIntegrator;
        unsigned int angularIntegrator;
        
        // Interrupt controller variables
        Timer timer;
        
        bool running;
};

#endif