#ifndef PIN_CONSTANTS_H
#define PIN_CONSTANTS_H
// P variables
double kp = 0.00043;

// Derivative variables
double kd = 0.0009;
Timer timer;
int prevError = 0;

// Integral variables
double ki = 0.00005;
int integrator = 0;
int decayFactor = 10;


// IR variables
float leftForwardLED = 0;
float rightForwardLED = 0;

float leftBackLED = 0;
float rightBackLED = 0;

int LEDOnTime = 1000;


#endif