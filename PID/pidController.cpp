#include "pidController.h"
#include "AVEncoder/AVEncoder.h"
#include "LED/IRLEDConstants.h"
#include <cmath>

pidController::pidController(AVEncoder* left, AVEncoder* right, 
                             PwmOut* LForward, PwmOut* LReverse, 
                             PwmOut* RReverse, PwmOut* RForward,
                             AnalogIn* in_left_back, AnalogIn* in_left_front, AnalogIn* in_right_front, AnalogIn* in_right_back,
                            DigitalOut* out_left_back, DigitalOut* out_left_front, DigitalOut* out_right_front, DigitalOut* out_right_back)
: LeftEncoder(left), RightEncoder(right),
  LMotorForward(LForward), LMotorReverse(LReverse),
  RMotorForward(RForward), RMotorReverse(RReverse),
  IR_in_left_back(in_left_back), IR_in_left_front(in_left_front), IR_in_right_front(in_right_front), IR_in_right_back(in_right_back),
  IR_out_left_back(out_left_back), IR_out_left_front(out_left_front), IR_out_right_front(out_right_front), IR_out_right_back(out_right_back)
{
    leftSpeed = DEFAULT_LEFT_SPEED;
    rightSpeed = DEFAULT_RIGHT_SPEED;
    
    // Initialize variables for PID
    prevIRError = 0;
    IRIntegrator = 0;
    
    // Calculate left IR base value
    *IR_out_left_back = 1;
    left_IR_base = 0;
    for(int i = 0; i < 100; i++)
    {
        left_IR_base += 1000*IR_in_left_back->read();
    }
    left_IR_base /= 100;
    *IR_out_left_back = 0;
   
   // Valculate right ID base value
   *IR_out_right_back = 1;
   right_IR_base = 0;
   for(int i = 0; i < 100; i++)
   {
       right_IR_base += 1000*IR_in_right_back->read();
   }
   right_IR_base /= 100;
   *IR_out_right_back = 0;
           
   turning = false;
   running = false;
}

void pidController::start()
{
    timer.start();
    timer.reset();
    running = true;
}

// Main loop that runs the PID code and makes the necessary adjustments to 
// each motors speed
void pidController::pid()
{        
    if(!running || turning)
        return;
        
    // Get the time that has passed since the last PID update
    int dt = timer.read_us();
    
    // Read IR values
    *IR_out_left_back = 1;
    *IR_out_right_back = 1;
    float left_IR = 0;
    float right_IR = 0;
    for(int i = 0; i < 10; i++)
    {
        left_IR += 1000*IR_in_left_back->read();
        right_IR += 1000*IR_in_right_back->read();
    }
    left_IR /= 10;
    right_IR /= 10;
    *IR_out_left_back = 0;
    *IR_out_right_back = 0;
    
    // Determine the error in IR readings
    double left_IR_error = left_IR - left_IR_base;
    double right_IR_error = right_IR - right_IR_base;
    
    double IR_correction_left = 0;
    double IR_correction_right = 0;
    if(left_IR > LEFT_IR_WALL)
    {
        //Calculate IR error based on left IR
        IR_correction_left = P_controller_IR(left_IR_error) + 
                     //I_controller_IR(left_IR_error, IRIntegrator, dt) + 
                     D_controller_IR(left_IR_error, prevIRError, dt);
    }
    else if(right_IR > RIGHT_IR_WALL)
    {
        //Calculate IR error based on right IR
        IR_correction_right = P_controller_IR(right_IR_error) + 
                     //I_controller_IR(right_IR_error, IRIntegrator, dt) + 
                     D_controller_IR(right_IR_error, prevIRError, dt);
        IR_correction_right = -IR_correction_right;
    }
    
    // Calculate the total IR correction
    double IR_correction = (IR_correction_left + IR_correction_right)/2;
                                      
    // Calculate new speeds
    leftSpeed = DEFAULT_LEFT_SPEED + IR_correction;
    rightSpeed = DEFAULT_RIGHT_SPEED - IR_correction;

    // Make sure speeds stay within the proper bounds
    boundSpeeds();
    
    // We don't want the mouse to be able to move backwards
    if(leftSpeed < 0)
        leftSpeed = 0;
    if(rightSpeed < 0)
        rightSpeed = 0;
    
    // Set the new speeds
    setLeftPwm(leftSpeed);
    setRightPwm(rightSpeed);
     
    // Reset the timer for the next iteration
    timer.reset();
}

double pidController::P_controller_IR(double error)
{
    return (KP_IR*error);
}

double pidController::I_controller_IR(double error, unsigned int& integrator, int dt)
{
    integrator += (error*dt)/1000000;
    integrator /= DECAY_FACTOR;
    
    double correction = KI_IR * integrator;
        
    return correction;
}

double pidController::D_controller_IR(double error, double& prevError, int dt)
{
    double dError = error - prevError;
        
    prevError = error;
    
    return KD_IR*dError/(1000*dt);
}

void pidController::setLeftPwm(double speed) 
{
    if(speed < 0)
    {
        *LMotorForward = STOP;
        *LMotorReverse = -speed;
    }
    else if(speed > 0)
    {
        *LMotorForward = speed;
        *LMotorReverse = STOP;
    }
    else
    {
        *LMotorForward = STOP;
        *LMotorReverse = STOP;
    }
}

void pidController::setRightPwm(double speed) 
{
    if(speed > 0)
    {
        *RMotorForward = speed;
        *RMotorReverse = STOP;
    }
    else if(speed < 0)
    {
        *RMotorForward = STOP;
        *RMotorReverse = -speed;
    }
    else
    {
        *RMotorForward = STOP;
        *RMotorReverse = speed;
    }
}

void pidController::boundSpeeds()
{
    if(leftSpeed < -1)
    {
        leftSpeed = -1;
    }
    else if(leftSpeed > 1)
    {
        leftSpeed = 1;
    }
    
    if(rightSpeed < -1)
    {
        rightSpeed = -1;
    }
    else if(rightSpeed > 1)
    {
        rightSpeed = 1;
    }
}

void pidController::stop()
{
    running = false;
    leftSpeed = STOP;
    rightSpeed = STOP;
    
    setLeftPwm(STOP);
    setRightPwm(STOP);
    
    while(LeftEncoder->getPulses() > 0 || RightEncoder->getPulses() > 0)
    {
        LeftEncoder->reset();
        RightEncoder->reset();
        wait(0.01);
    }
}

void pidController::moveForwardOneCellNotMoving()
{
    turning = false;
    setLeftPwm(leftSpeed);
    setRightPwm(rightSpeed);
    
    LeftEncoder->reset();
    RightEncoder->reset();
    while((LeftEncoder->getPulses() + RightEncoder->getPulses())/2 < 590)
        ;
}

void pidController::turnLeft()
{
    turning = true;
    
    LeftEncoder->reset();
    RightEncoder->reset();
   
    setRightPwm(.15);
    setLeftPwm(-.15);
    
    while((LeftEncoder->getPulses() + RightEncoder->getPulses())/2 < LEFT_TURN_ENCODER_COUNT)
        ;
        
    stop();
    
    turning = false;
}

void pidController::turnRight()
{
    turning = true;
    
    LeftEncoder->reset();
    RightEncoder->reset();
   
    setRightPwm(-.15);
    setLeftPwm(.15);
    
    while((LeftEncoder->getPulses() + RightEncoder->getPulses())/2 < RIGHT_TURN_ENCODER_COUNT)
        ;
        
    stop();
    
    turning = false;
}

void pidController::turnAround()
{
    turning = true;

    LeftEncoder->reset();
    RightEncoder->reset();
   
    setRightPwm(.15);
    setLeftPwm(-.15);
    
    while((LeftEncoder->getPulses() + RightEncoder->getPulses())/2 < TURN_AROUND_ENCODER_COUNT)
        ;
        
    stop();
    
    turning = false;
}

void pidController::moveForwardOneCell()
{
    // Reset encoder readings
    LeftEncoder->reset();
    RightEncoder->reset();

    // Turn motors on
    setLeftPwm(leftSpeed);
    setRightPwm(rightSpeed);
    
    // Wait for encoders to tell us we've moved one square
    // Because PID is on interrupts, PID will continue running
    while((LeftEncoder->getPulses() + RightEncoder->getPulses())/2 < ONE_CELL_PULSES)
        ;
        
    stop();
    
    // Reset encoder readings
    LeftEncoder->reset();
    RightEncoder->reset();
}

void pidController::turnLeftFromMoving()
{
    LeftEncoder->reset();
    RightEncoder->reset();
    while((LeftEncoder->getPulses() + RightEncoder->getPulses())/2 < 420)
        ;
    // TODO: turning should be curved
    turning = true;
    
    stop();
    
    int i = 0;
    
    int leftPulses, rightPulses;
    while(i < 50)
    {
        leftPulses = LeftEncoder->getPulses();
        rightPulses = RightEncoder->getPulses();
        LeftEncoder->reset();
        RightEncoder->reset();
        
        if( leftPulses == 0 && rightPulses == 0)
            i++;
        else
            i = 0;
    }

    //Now the mouse should have stopped appropriately.
    turnLeft();
}
        
void pidController::turnRightFromMoving()
{
    LeftEncoder->reset();
    RightEncoder->reset();
    while((LeftEncoder->getPulses() + RightEncoder->getPulses())/2 < 420)
        ;
    // TODO: turning should be curved
    turning = true;
    
    stop();
    
    int i = 0;
    
    int leftPulses, rightPulses;
    while(i < 50)
    {
        leftPulses = LeftEncoder->getPulses();
        rightPulses = RightEncoder->getPulses();
        LeftEncoder->reset();
        RightEncoder->reset();
        
        if( leftPulses == 0 && rightPulses == 0)
            i++;
        else
            i = 0;
    }

    //Now the mouse should have stopped appropriately.
    turnRight();
}

void pidController::moveForward()
{
    int front_left_LED = 0, front_right_LED = 0, back_left_LED = 0, back_right_LED = 0;
    *IR_out_left_front = 1;
    for(int i = 0; i < 10; i++)
    {
        front_left_LED += 1000*IR_in_left_front->read();
    }
    front_left_LED /= 10;
    *IR_out_left_front = 0;
   
    *IR_out_right_front = 1;
    for(int i = 0; i < 10; i++)
    {
        front_right_LED += 1000*IR_in_right_front->read();
    }
    front_right_LED /= 10;
    *IR_out_right_front = 0;

    *IR_out_left_back = 1;
    for(int i = 0; i < 10; i++)
    {
        back_left_LED += 1000*IR_in_left_back->read();
    }
    back_left_LED /= 10;
    *IR_out_left_back = 0;
   
    *IR_out_right_back = 1;
    for(int i = 0; i < 10; i++)
    {
        back_right_LED += 1000*IR_in_right_back->read();
    }
    back_right_LED /= 10;
    *IR_out_right_back = 0;

    turning = false;

    if(front_left_LED > IR_FRONT_WALL || front_right_LED > IR_FRONT_WALL)
    {
        turnLeft();
    }
}

void pidController::pause()
{
    stop();
    running = false;
}