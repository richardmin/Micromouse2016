//http://people.ece.cornell.edu/land/courses/ece4760/FinalProjects/s2004/rd73/476finalpro.htm
#include <cmath>
#include "Motor/MotorController.h"

void pid();

double P_controller(int error);
double D_controller(int error);
double I_controller(int error);


void setRightPwm(double speed); 
void setLeftPwm(double speed);
void stop();


// Functions to control turning
void turnRight();
void turnLeft();

// Variables to control speed
double leftSpeed;
double rightSpeed;
int STOP = 0;
