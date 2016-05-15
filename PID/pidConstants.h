#ifndef PID_CONSTANTS_H
#define PID_CONSTANTS_H

// Proportional variables
const static double KP_translational = 0.0000005;
const static double KP_angular = 0.00021;
const static double KP_IR = 0.0005;

// Integral variables
const static double KI_translational = 0.0000000001;
const static double KI_angular = 0.0001;
const static double KI_IR = 0.0001;
const static int DECAY_FACTOR = 10;

// Derivative variables
const static double KD_translational = 0.0001;
const static double KD_angular = 0.00001;
const static double KD_IR = 0.01;

// Speed variables
const static int STOP = 0;
static int IDEAL_ANGULAR_SPEED = -67; // In mm/s
static int IDEAL_TRANSLATIONAL_SPEED = 500; // In mm/s

static int LEFT_TURN_ENCODER_COUNT = 295;

const static int LEFT_IR_WALL = 170;
const static int RIGHT_IR_WALL = 350;


const static double DEFAULT_RIGHT_SPEED = 250/1885.0;
const static double DEFAULT_LEFT_SPEED = 200/1885.0;
#endif
