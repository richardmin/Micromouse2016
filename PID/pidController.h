#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H
//http://people.ece.cornell.edu/land/courses/ece4760/FinalProjects/s2004/rd73/476finalpro.htm

#include "PID/pidConstants.h"


class pidController {
	public:	

		void pid();


		void stop();


		// Functions to control turning
		void turnRight();
		void turnLeft();
	private:
		// Variables to control speed


		double P_controller(int error);
		double D_controller(int error);
		double I_controller(int error);

		
		void setRightPwm(double speed); 
		void setLeftPwm(double speed);


		double leftSpeed;
		double rightSpeed;
		int STOP = 0;
};


#endif