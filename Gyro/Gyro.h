#include "PINS/pinouts.h"
#include "Gyro/GyroConstants.h"
#ifndef GYRO_H
#define GYRO_H


class Gyro {
public:
    float returnAngle();
    void pollGyro();
    void reset();
    Gyro();

private:
    float angle;
    Timer gyroTimer;

    void accumulateAngle(float voltage, int milliseconds);

};
#endif