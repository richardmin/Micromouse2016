#ifndef PINOUTS_H
#define PINOUTS_H

#include "../AVEncoder/AVEncoder.h"

// Controllers for right motor
#define R_MOTOR_FORWARD PB_6
#define R_MOTOR_REVERSE PA_7

// Controllers for left motor
#define L_MOTOR_FORWARD PB_8
#define L_MOTOR_REVERSE PC_9

// Controllers for left and right encoders
#define L_ENCODER_A PC_6
#define L_ENCODER_B PC_8
#define R_ENCODER_A PC_10
#define R_ENCODER_B PC_12

// Controller for gyro
#define GYRO_PIN PB_1

// Controller for IR receivers
#define IR_LEFT_BACK A0
#define IR_LEFT_FRONT A1
#define IR_RIGHT_FRONT A2
#define IR_RIGHT_BACK A3

// Power controller for the IR's
#define IR_LEFT_BACK_PWR D1
#define IR_LEFT_FRONT_PWR D2
#define IR_RIGHT_FRONT_PWR D3
#define IR_RIGHT_BACK_PWR D4

#endif