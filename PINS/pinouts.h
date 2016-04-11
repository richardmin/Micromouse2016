#ifndef PINOUTS_H
#define PINOUTS_H

#include "../AVEncoder/AVEncoder.h"

// Controllers for right motor
#define R_MOTOR_FORWARD PB_3
#define R_MOTOR_REVERSE PB_4

// Controllers for left motor
#define L_MOTOR_FORWARD PC_4
#define L_MOTOR_REVERSE PC_5

// Controllers for left and right encoders
#define L_ENCODER_A PA_6
#define L_ENCODER_B PA_7
#define R_ENCODER_A PB_5
#define R_ENCODER_B PB_6

// Controller for gyro
#define GYRO_PIN PB_1

// Controller for IR receivers
#define IR_LEFT_BACK PC_0
#define IR_LEFT_FRONT PC_1
#define IR_RIGHT_FRONT PC_2
#define IR_RIGHT_BACK PC_3

// Power controller for the IR's
#define IR_LEFT_BACK_PWR PC_13
#define IR_LEFT_FRONT_PWR PC_14
#define IR_RIGHT_FRONT_PWR PC_15
#define IR_RIGHT_BACK_PWR PH_0

#endif