#include "Gyro.h"

Gyro::Gyro()
{
    angle = 90; //90 is straight up
}


float Gyro::returnAngle()
{
    return angle;
}

void Gyro::accumulateAngle(float voltage, int milliseconds)
{
    float calculatedAngle = 0;
    calculatedAngle = (voltage-BASE_VOLTAGE)*milliseconds; //TODO: FIX THIS
    angle += calculatedAngle;
}

void Gyro::pollGyro()
{
    accumulateAngle(gyro.read(), gyroTimer.read_us());
    gyroTimer.reset();
}

void Gyro::reset()
{
    angle = 90;
}