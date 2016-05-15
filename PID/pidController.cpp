#include "pidController.h"
#include "AVEncoder/AVEncoder.h"
#include "Gyro/GyroConstants.h"
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
    
    prevTranslationalError = 0;
    prevAngularError = 0;
    prevIRError = 0;
        
    translationalIntegrator = 0;
    angularIntegrator = 0;
    IRIntegrator = 0;
    
    *IR_out_left_back = 1;
    left_IR_base = 0;
    for(int i = 0; i < 100; i++)
    {
        left_IR_base += 1000*IR_in_left_back->read();
    }
    left_IR_base /= 100;
    *IR_out_left_back = 0;
   
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
    
    // Determine the error in everything
    double translational_error = IDEAL_TRANSLATIONAL_SPEED - actual_translational_speed;
    double angular_error = IDEAL_ANGULAR_SPEED - actual_angular_speed;
    double left_IR_error = left_IR - left_IR_base;
    double right_IR_error = right_IR - right_IR_base;
    
    double IR_correction;
    double IR_correction_left = 0;
    double IR_correction_right = 0;
    if(left_IR > LEFT_IR_WALL)
    {
        //////printf("LEFT WALL\t");
        //Calculate IR error based on left IR
        IR_correction_left = P_controller_IR(left_IR_error) + 
                     //I_controller_IR(left_IR_error, IRIntegrator, dt) + 
                     D_controller_IR(left_IR_error, prevIRError, dt);
    }
    if(right_IR > RIGHT_IR_WALL)
    {
        //////printf("RIGHT WALL\t");
        //Calculate IR error based on right IR
        IR_correction_right = P_controller_IR(right_IR_error) + 
                     //I_controller_IR(right_IR_error, IRIntegrator, dt) + 
                     D_controller_IR(right_IR_error, prevIRError, dt);
        IR_correction_right = -IR_correction_right;
    }
    
    IR_correction = (IR_correction_left + IR_correction_right)/2;
    
    double translational_correction = P_controller_translational(translational_error) +
                                      //I_controller_translational(translational_error, translationalIntegrator, dt) +
                                      D_controller_translational(translational_error, prevTranslationalError, dt);
                                      
    double angular_correction = P_controller_angular(angular_error) + 
                                //I_controller_angular(angular_error, angularIntegrator, dt) + 
                                D_controller_angular(angular_error, prevAngularError, dt);
    angular_correction = 0;
    translational_correction = 0;
                                      
    // Calculate new speeds
    leftSpeed = DEFAULT_LEFT_SPEED + (translational_correction + angular_correction + (IR_correction/2));
    rightSpeed = DEFAULT_RIGHT_SPEED + (translational_correction - angular_correction - (IR_correction/2)); 
    //////////printf("LEFT: %f (%f)\tRIGHT: %f (%f) CORRECTION: %f\r\n", leftSpeed, left_IR, rightSpeed, right_IR, IR_correction);
    //////printf("%f\t%f\t%f\r\n", translational_correction, IR_correction, (translational_correction + angular_correction - IR_correction));

    // Make sure speeds stay within the proper bounds
    boundSpeeds();
    
    if(leftSpeed < 0)
        leftSpeed = 0;
    if(rightSpeed < 0)
        rightSpeed = 0;
    
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
    setLeftPwm(STOP);
    setRightPwm(STOP);
}

void pidController::turnLeft()
{
    LeftEncoder->reset();
    RightEncoder->reset();
    while((LeftEncoder->getPulses() + RightEncoder->getPulses())/2 < 572)
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
        
//        wait(.01);
        if( leftPulses == 0 && rightPulses == 0)
            i++;
        else
            i = 0;
    }
    setRightPwm(.15);
    setLeftPwm(-.15);
    
    while(LeftEncoder->getPulses() < LEFT_TURN_ENCODER_COUNT ||  RightEncoder->getPulses() < LEFT_TURN_ENCODER_COUNT)
    {
        if(LeftEncoder->getPulses() >= LEFT_TURN_ENCODER_COUNT)
        {
            setLeftPwm(0.0);
        }
        
        if(RightEncoder->getPulses() >= LEFT_TURN_ENCODER_COUNT)
        {
            setRightPwm(0.0);    
        }
    }
        
    stop();
    turning = false;
    setLeftPwm(leftSpeed);
    setRightPwm(rightSpeed);
    
    LeftEncoder->reset();
    RightEncoder->reset();
    while((LeftEncoder->getPulses() + RightEncoder->getPulses())/2 < 372)
        ;
        
    stop();
    while(1);
}

void pidController::turnRight()
{
    // TODO: turning should be curved
    turning = true;
    
    *LMotorForward = 1;
    *LMotorReverse = 1;
    *RMotorForward = 1;
    *RMotorReverse = 1;
    
    int i = 0;
    while(LeftEncoder->getPulses() != 0 &&  RightEncoder->getPulses() != 0)
    {
        ////////////printf("Looped: %d\r\n", i);
        LeftEncoder->reset();
        RightEncoder->reset();
        
        wait(.01);
        i++;
    }
    
    setRightPwm(-.15);
    setLeftPwm(.15);
    
    while(LeftEncoder->getPulses() < LEFT_TURN_ENCODER_COUNT ||  RightEncoder->getPulses() < LEFT_TURN_ENCODER_COUNT)
    {
        ////////////printf(" Left Encoder: %d Right Encoder: %d \r\n", LeftEncoder->getPulses(), RightEncoder->getPulses());
        
        if(LeftEncoder->getPulses() >= LEFT_TURN_ENCODER_COUNT)
        {
            setLeftPwm(0.0);    
        }
        
        if(RightEncoder->getPulses() >= LEFT_TURN_ENCODER_COUNT)
        {
            setRightPwm(0.0);    
        }
    }
    
    stop();
    
    LeftEncoder->reset();
    RightEncoder->reset();
    
    //setLeftPwm(leftSpeed);
    //setRightPwm(rightSpeed);
    turning = false;
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

//    if(back_left_LED > 70)
//        //////////printf("LEFT WALL %d\r\n", back_left_LED);
//
//    if(back_right_LED > 350)
//        //////////printf("RIGHT WALL\r\n");
    if(front_left_LED > IR_FRONT_WALL || front_right_LED > IR_FRONT_WALL)
    {
        turnLeft();
    }
}