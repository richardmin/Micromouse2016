#include "pidController.h"
#include "../PINS/pinouts.h"
#include <cmath>


pidController::pidController() {
    leftSpeed = 0;
    rightSpeed = 0;
    turn = false;
}

// Main loop that runs the PID code and makes the necessary adjustments to 
// each motors speed
void pidController::pid()
{
    // TODO: right now we don't run PID while turning
    // Eventually, we want to use the gyro to make sure we
    // are turning correctly.
    if(turn)
        return;
        
    // Read the encoder for the left and right motor
    int left_encoder_pulses = LeftEncoder.getPulses();
    int right_encoder_pulses = RightEncoder.getPulses();
    
    // Get the time that has passed since the last PID update
    int dt = timer.read_us();
    
    // Calculate the actual speeds we are traveling at
    // TODO: The angular speed should be based off of the gyro, not the encoders
    int actual_translational_speed = (left_encoder_pulses + right_encoder_pulses)/dt;
    int actual_angular_speed = (right_encoder_pulses - left_encoder_pulses)/dt;
    
    // Determine the error in the translational speed and the angular speed
    // TODO: the angular error should also include the gyro and IRs
    int translational_error = IDEAL_TRANSLATIONAL_SPEED - actual_translational_speed;
    int angular_error = IDEAL_ANGULAR_SPEED - actual_angular_speed;
    
    double translational_correction = P_controller(translational_error) +
                                      I_controller(translational_error) +
                                      D_controller(translational_error);
                                      
    double angular_correction = P_controller(angular_error) +
                                I_controller(angular_error) +
                                D_controller(angular_error);
                                      
    // Calculate new speeds
    leftSpeed += (translational_correction - angular_correction);
    rightSpeed += (translational_correction + angular_correction);
    
    // Make sure speeds stay within the proper bounds
    boundSpeeds();
    
    // Set new speeds
    setLeftPwm(leftSpeed);
    setRightPwm(rightSpeed);
     
    // Reset the sensors for the next iteration
    LeftEncoder.reset();
    RightEncoder.reset();
    timer.reset();
}

double pidController::P_controller(int error)
{
    // TODO: this function
    return -1;
    //return (KP*error);
}

double pidController::D_controller(int error)
{   
    // TODO: this function
    return -1;
    /*double dError = error - prevError;
        
    int dt = timer.read_us();
    
    timer.reset();
    prevError = error;
    
    return KD*dError/dt;*/
}

double pidController::I_controller(int error)
{
    //  TODO: this function
    return -1;
    /*integrator += error;
    double correction = KI * integrator;
    integrator /= DECAY_FACTOR;
    
    return correction;*/
}

void pidController::setLeftPwm(double speed) 
{
    if(speed < 0)
    {
        LMotorForward = STOP;
        LMotorReverse = speed;
    }
    else if(speed > 0)
    {
        LMotorForward = speed;
        LMotorReverse = STOP;
    }
    else
    {
        LMotorForward = STOP;
        LMotorReverse = STOP;
    }
}

void pidController::setRightPwm(double speed) 
{
    if(speed > 0)
    {
        RMotorForward = speed;
        RMotorReverse = STOP;
    }
    else if(speed < 0)
    {
        RMotorForward = STOP;
        RMotorReverse = speed;
    }
    else
    {
        RMotorForward = STOP;
        RMotorReverse = speed;
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
    // TODO: add stop code
}

void pidController::turnLeft()
{
    // TODO: add turn code
}

void pidController::turnRight()
{
    // TODO: add turn code
}