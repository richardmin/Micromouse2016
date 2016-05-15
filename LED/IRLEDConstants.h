#ifndef IR_LED_CONSTANTS_H
#define IR_LED_CONSTANTS_H

//the angles of the IR LEDs on our PCB
static float angle_front_left;
static float angle_front_right = 360 - angle_front_left;
static float angle_back_right;
static float angle_back_left = 360 - angle_back_right;

//the LED measured variables (should probably not be global)
static float measured_left_forward;
static float measured_right_forward;

static float measured_left_back;
static float measured_right_back;

const static int IR_FRONT_WALL = 250;

// Default amount of time we're polling our IR LEDs
const int LED_ON_TIME = 10;

const int LED_HIST_SIZE = 50;

const int WALL_IN_FRONT_LEFT = 880;
const int WALL_IN_FRONT_RIGHT = 880;
const int WALL_TO_LEFT = 450;
const int WALL_TO_RIGHT = 425;

#endif