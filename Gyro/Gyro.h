#ifndef GYRO_H
#define GYRO_H

#include "mbed.h"
#include "Gyro/GyroConstants.h"

class Gyro {
    public:
        
        Gyro();
        void pollGyro(int dt);
        void reset();
        
        double getAngularSpeed();
        double getAngle();
        double getTotalAngle();
        double getDistance();
        double getTotaDistance();
    
    private:
    
        void setAngle();
        void setDistance();
        
        double angularSpeed;
        double angle;
        double totalAngle;
        double distance;
        double totalDistance;
        int dt;
        Timer timer;
    
        AnalogIn gyro;
};
#endif