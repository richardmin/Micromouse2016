#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

double kp = 0.00043;

// Derivative variables
double kd = 0.0009;
Timer timer;
int prevError = 0;

// Integral variables
double ki = 0.00005;
int integrator = 0;
int decayFactor = 10;


#endif