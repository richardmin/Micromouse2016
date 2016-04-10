#ifndef PINOUTS_H
#define PINOUTS_H

#include "../AVEncoder/AVEncoder.h"

// Controllers for right motor
#define R_MOTOR_FORWARD PB_3
#define R_MOTOR_REVERSE PB_4
static PwmOut RMotorReverse(R_MOTOR_REVERSE);
static PwmOut RMotorForward(R_MOTOR_FORWARD);

// Controllers for left motor
#define L_MOTOR_FORWARD PC_4
#define L_MOTOR_REVERSE PC_5
static PwmOut LMotorForward(L_MOTOR_FORWARD);
static PwmOut LMotorReverse(L_MOTOR_REVERSE);

// Controllers for left and right encoders
#define L_ENCODER_A PA_6
#define L_ENCODER_B PA_7
static AVEncoder LeftEncoder(L_ENCODER_A, L_ENCODER_B);

#define R_ENCODER_A PB_5
#define R_ENCODER_B PB_6
static AVEncoder RightEncoder(R_ENCODER_A, R_ENCODER_B);

// Controller for gyro
#define GYRO_PIN PB_1
static AnalogIn gyro(GYRO_PIN);

// Controller for IRs
#define IR_LEFT_BACK PC_0
#define IR_LEFT_FRONT PC_1
#define IR_RIGHT_FRONT PC_2
#define IR_RIGHT_BACK PC_3
static AnalogIn IR_left_front(IR_LEFT_BACK);
static AnalogIn IR_right_front(IR_LEFT_FRONT);
static AnalogIn IR_left_back(IR_RIGHT_FRONT);
static AnalogIn IR_right_back(IR_RIGHT_BACK);

// Power controller for the IR's
#define IR_LEFT_BACK_PWR PC_13
#define IR_LEFT_FRONT_PWR PC_14
#define IR_RIGHT_FRONT_PWR PC_15
#define IR_RIGHT_BACK_PWR PH_0
static DigitalOut IR_left_forward_pwr(D15);
static DigitalOut IR_right_forward_pwr(D7);
static DigitalOut IR_left_back_pwr(D0); 
static DigitalOut IR_right_back_pwr(D0);

#endif