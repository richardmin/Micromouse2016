#ifndef PID_CONSTANTS_H
#define PID_CONSTANTS_H

// Proportional variables
const static double KP_IR = 0.001;

// Integral variables
const static double KI_IR = 0.0001;
const static int DECAY_FACTOR = 10;

// Derivative variables
const static double KD_IR = 0.015;

// Speed variables
const static int STOP = 0;
static int IDEAL_ANGULAR_SPEED = -67; // In mm/s
static int IDEAL_TRANSLATIONAL_SPEED = 500; // In mm/s

static int LEFT_TURN_ENCODER_COUNT = 280;
static int RIGHT_TURN_ENCODER_COUNT = 320;
static int TURN_AROUND_ENCODER_COUNT = 560;
static int ONE_CELL_PULSES = 600;

const static int LEFT_IR_WALL = 170;
const static int RIGHT_IR_WALL = 350;

const static double DEFAULT_RIGHT_SPEED = 250/1885.0;
const static double DEFAULT_LEFT_SPEED = 225/1885.0;
#endif