#include "LED/IRLEDConstants.h"
#include "PINS/pinouts.h" 

#ifndef LEDCOLLECTOR_H
#define LEDCOLLECTOR_H


class LEDCollector 
{
    public:
        void pollLEDs(int milliseconds);
        bool wallInFront();
        bool wallToLeft();
        bool wallToRight();
        LEDCollector();
    private:
        float leftForwardLED;
        float rightForwardLED;
        float leftBackLED;
        float rightBackLED;

        float leftForwardLEDHistory[LED_HIST_SIZE];
        float leftBackLEDHistory[LED_HIST_SIZE];
        float rightForwardLEDHistory[LED_HIST_SIZE];
        float rightBackLEDHistory[LED_HIST_SIZE];

        float initialLeftForwardLED;
        float initialRightForwardLED;
        float initialLeftBackLED;
        float initialRightBackLED;

        int currHistPosition;
        
        void detectWallChanges();

};
#endif
