#ifndef PID_CONSTANTS_H
#define PID_CONSTANTS_H

// Proportional variables
const static double KP_translational = 0.000002;
const static double KP_angular = 0.000008;

// Integral variables
const static double KI_translational = 0.00001;
const static double KI_angular = 0.001;
const static int DECAY_FACTOR = 10;

// Derivative variables
const static double KD_translational = 0.00001;
const static double KD_angular = 0.00001;

// Speed variables
const static int STOP = 0;
static int IDEAL_ANGULAR_SPEED = 0; // In mm/s
static int IDEAL_TRANSLATIONAL_SPEED = 500; // In mm/s


#endif