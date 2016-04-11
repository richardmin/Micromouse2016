#ifndef PID_CONSTANTS_H
#define PID_CONSTANTS_H

// Proportional variables
const static double KP = 0.00043;

// Integral variables
const static double KI = 0.00005;
const static int DECAY_FACTOR = 10;

// Derivative variables
const static double KD = 0.0009;

// Speed variables
const static int STOP = 0;
static int IDEAL_ANGULAR_SPEED = 0; // In mm/s
static int IDEAL_TRANSLATIONAL_SPEED = 25; // In mm/s


#endif