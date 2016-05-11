#include "Gyro.h"
#include "PINS/pinouts.h"

Gyro::Gyro()
: gyro(GYRO_PIN)
{
    angularSpeed = 0;
    angle = 0;
    totalAngle = 0;
    distance = 0;
    totalDistance = 0;
    dt = 0;

    timer.start();
}

void Gyro::pollGyro(int dt)
{
    this->dt = dt;
    
    angularSpeed = gyro.read();
    angularSpeed *= 3.4;
    angularSpeed = 1.5 - angularSpeed;
    angularSpeed *= 1492.5; 
    
    setAngle();
    setDistance();
    
    timer.reset();
}

void Gyro::setAngle()
{
    angle = angularSpeed * (dt / 1E-6);
    totalAngle += angle;
}


void Gyro::setDistance()
{
    distance = WHEEL_CIRCUMFERENCE * (angle / 360);
    totalDistance += distance;
}

void Gyro::reset()
{
    angularSpeed = 0;
    angle = 0;
    totalAngle = 0;
    distance = 0;
    totalDistance = 0;
    dt = 0;
    timer.reset();
}

double Gyro::getAngularSpeed()
{
    return angularSpeed;
}

double Gyro::getAngle()
{
    return angle;
}

double Gyro::getTotalAngle()
{
    return totalAngle;
}

double Gyro::getDistance()
{
    return distance;
}

double Gyro::getTotaDistance()
{
    return totalDistance;
}