// Just help us know when code started running
DigitalOut myled(LED1);
DigitalIn mybutton(USER_BUTTON);

// Controllers for right motor
PwmOut RMotorReverse(PB_6);
PwmOut RMotorForward(PA_7);

// Controllers for left motor
PwmOut LMotorForward(PB_10);
PwmOut LMotorReverse(PB_4);

// Controllers for left and right encoders
AVEncoder LeftEncoder(PA_15, PB_3);
AVEncoder RightEncoder(PA_1, PA_2);

// Controller for gyro
AnalogIn gyro(A0);

// Controller for IRs
AnalogIn IR_left(A5);
AnalogIn IR_right(A3);

DigitalOut IR_left_pwr(D15);
DigitalOut IR_right_pwr(D7);
