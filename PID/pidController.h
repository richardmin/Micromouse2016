#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H
//http://people.ece.cornell.edu/land/courses/ece4760/FinalProjects/s2004/rd73/476finalpro.htm

#include "pidConstants.h"

class pidController {
	public:	
	
		// Constructor
		pidController();
	
		// Main function in charge of calculating new motor speeds
		void pid();

		// Sets both motors to zero
		void stop();

		// Functions to control turning
		void turnRight();
		void turnLeft();
		
	private:

		// Helper functions to determine PID correction
		double P_controller(int error);
		double D_controller(int error);
		double I_controller(int error);


		// Helper functions to drive the wires connected to the motors
		void setRightPwm(double speed); 
		void setLeftPwm(double speed);


		// Variables that hold the relative speed of each motor
		double leftSpeed;
		double rightSpeed;
		
		// Variables to assist with PID calculations
		int integrator;
		int prevError;
		
		// Determines if we are currently turning
		int turn;
		
		// Interrupt controller variables
		Ticker interrupt;
		Timer timer;
};


#endif