#ifndef PINOUTS_H
#define PINOUTS_H

#include "../AVEncoder/AVEncoder.h"

// Controllers for right motor
static PwmOut RMotorReverse(PB_6);
static PwmOut RMotorForward(PA_7);

// Controllers for left motor
static PwmOut LMotorForward(PB_10);
static PwmOut LMotorReverse(PB_4);

// Controllers for left and right encoders
static AVEncoder LeftEncoder(PA_15, PB_3);
static AVEncoder RightEncoder(PA_1, PA_2);

// Controller for gyro
static AnalogIn gyro(A0);

// Controller for IRs
static AnalogIn IR_left_front(A5);
static AnalogIn IR_right_front(A3);
static AnalogIn IR_left_back(A0);
static AnalogIn IR_right_back(A0);

static DigitalOut IR_left_forward_pwr(D15);
static DigitalOut IR_right_forward_pwr(D7);
static DigitalOut IR_left_back_pwr(D0); 
static DigitalOut IR_right_back_pwr(D0);

#endif