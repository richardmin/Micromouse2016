#include "pidController.h"
#include "AVEncoder/AVEncoder.h"
#include "Gyro/GyroConstants.h"

pidController::pidController(AVEncoder* left, AVEncoder* right, 
                             PwmOut* LForward, PwmOut* LReverse, 
                             PwmOut* RReverse, PwmOut* RForward)
: LeftEncoder(left), RightEncoder(right),
  LMotorForward(LForward), LMotorReverse(LReverse),
  RMotorForward(RForward), RMotorReverse(RReverse)
{
    leftSpeed = 500/1885.0;
    rightSpeed = 500/1885.0;
    
    prevTranslationalError = 0;
    prevAngularError = 0;
        
    translationalIntegrator = 0;
    angularIntegrator = 0;
        
    turning = false;
    running = false;
}

void pidController::start()
{
    timer.start();
    timer.reset();
    LeftEncoder->reset();
    RightEncoder->reset();
    running = true;
}

// Main loop that runs the PID code and makes the necessary adjustments to 
// each motors speed
void pidController::pid()
{        
    if(!running)
    {
        LeftEncoder->reset();
        RightEncoder->reset();
        timer.reset();
        return;
    }

    if(turning)
        return;
        
    // Get the time that has passed since the last PID update
    int dt = timer.read_us();

    // Read the encoder for the left and right motor
    int left_encoder_pulses = LeftEncoder->getPulses();
    int right_encoder_pulses = RightEncoder->getPulses();
    
    // Calculate actual translational speed
    double actual_translational_speed = (left_encoder_pulses + right_encoder_pulses); // pulses
    actual_translational_speed /= dt; // pulses/us
    actual_translational_speed *= 1000000;
    actual_translational_speed /= 360; // rotations/s      
    actual_translational_speed *= WHEEL_CIRCUMFERENCE; // mm/s
    actual_translational_speed /= 2; // To account for the fact that we added at the beginning
    
        // Calculate actual angular speed
    double actual_angular_speed = (left_encoder_pulses - right_encoder_pulses); // pulses
    actual_angular_speed /= dt; // pulses/us
    actual_angular_speed *= 1000000;
    actual_angular_speed /= 360; // rotations/s
    actual_angular_speed  *= WHEEL_CIRCUMFERENCE; // mm/s
    
    // Determine the error in the translational speed and the angular speed
    double translational_error = IDEAL_TRANSLATIONAL_SPEED - actual_translational_speed;
    double angular_error = IDEAL_ANGULAR_SPEED - actual_angular_speed;
    
    double translational_correction = P_controller_translational(translational_error) +
                                      //I_controller_translational(translational_error, translationalIntegrator, dt) +
                                      D_controller_translational(translational_error, prevTranslationalError, dt);
                                      
    double angular_correction = P_controller_angular(angular_error) + 
                                //I_controller_angular(angular_error, angularIntegrator, dt) + 
                                D_controller_angular(angular_error, prevAngularError, dt);
                                      
    // Calculate new speeds
    leftSpeed += (translational_correction + angular_correction);
    rightSpeed += (translational_correction - angular_correction);
    
    // Make sure speeds stay within the proper bounds
    boundSpeeds();
    
    setLeftPwm(leftSpeed);
    setRightPwm(rightSpeed);
     
    // Reset the sensors for the next iteration
    LeftEncoder->reset();
    RightEncoder->reset();
    timer.reset();
}

double pidController::P_controller_translational(double error)
{
    return (KP_translational*error);
}

double pidController::I_controller_translational(double error, unsigned int& integrator, int dt)
{
    integrator += (error*dt)/1000000;
    if(integrator > 0.01/KI_translational)
            integrator /= DECAY_FACTOR;
                
    double correction = KI_translational * integrator;
        
    return correction;
}

double pidController::D_controller_translational(double error, double& prevError, int dt)
{   
    double dError = error - prevError;
        
    prevError = error;
    
    return KD_translational*dError/(1000*dt);
}

double pidController::P_controller_angular(double error)
{
    return (KP_angular*error);
}

double pidController::I_controller_angular(double error, unsigned int& integrator, int dt)
{
    integrator += (error*dt)/1000000;
        integrator /= DECAY_FACTOR;
    
    double correction = KI_angular * integrator;
        
    return correction;
}

double pidController::D_controller_angular(double error, double& prevError, int dt)
{   
    double dError = error - prevError;
        
    prevError = error;
    
    return KD_angular*dError/(1000*dt);
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
    setLeftPwm(STOP);
    setRightPwm(STOP);
}

void pidController::turnLeft()
{
    // TODO: turning should be curved
    turning = true;
    stop();
    setRightPwm(0.25);
    
    stop();
    
    LeftEncoder->reset();
    RightEncoder->reset();
    
    setLeftPwm(leftSpeed);
    setRightPwm(rightSpeed);
    turning = false;
}

void pidController::turnRight()
{
    // TODO: turning should be curved
    turning = true;
    stop();
    setLeftPwm(0.25);
    
    stop();
    
    LeftEncoder->reset();
    RightEncoder->reset();
    
    setLeftPwm(leftSpeed);
    setRightPwm(rightSpeed);
    turning = false;
}