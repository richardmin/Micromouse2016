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
AnalogIn IR_left_front(A5);
AnalogIn IR_right_front(A3);
AnalogIn IR_left_back(A0);
AnalogIn IR_right_back(A0);

DigitalOut IR_left_forward_pwr(D15);
DigitalOut IR_right_forward_pwr(D7);
DigitalOut IR_left_back_pwr(D0); 
DigitalOut IR_right_back_pwr(D0);
