#ifndef IR_LED_CONSTANTS_H
#define IR_LED_CONSTANTS_H

//the angles of the IR LEDs on our PCB
float angle_front_left;
float angle_front_right = 360 - angle_front_left;
float angle_back_right;
float angle_back_left = 360 - angle_back_right;

//the LED measured variables (should probably not be global)
float measured_left_forward;
float measured_right_forward;

float measured_left_back;
float measured_right_back;



// Default amount of time we're polling our IR LEDs
const int LED_ON_TIME = 1000;

const int LED_HIST_SIZE = 10;

#endif