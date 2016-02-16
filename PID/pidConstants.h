#ifndef PID_CONSTANTS_H
#define PID_CONSTANTS_H

double kp = 0.00043;

// Derivative variables
double kd = 0.0009;

int STOP = 0;

// Integral variables
double ki = 0.00005;
int decayFactor = 10;


// Interrupt controller
Ticker interrupt;
Timer timer;



#endif