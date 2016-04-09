#include "LED/LEDCollector.h"
#include "PINS/pinouts.h"

LEDCollector::LEDCollector()
{
    leftForwardLED = 0;
    rightForwardLED = 0;
    leftBackLED = 0;
    rightBackLED = 0;

    initialLeftForwardLED = 0;
    initialRightForwardLED = 0;
    initialLeftBackLED = 0;
    initialRightBackLED = 0;

    currHistPosition = 0;
    
    IR_left_forward_pwr = 1;
    for(int i = 0; i < LED_ON_TIME; i++)
    {
        initialLeftForwardLED += IR_left_front.read();
    }
    IR_left_forward_pwr = 0;
    
    IR_right_forward_pwr = 1;
    for(int i = 0; i < LED_ON_TIME; i++)
    {
        initialRightForwardLED += IR_right_front.read();
    }
    IR_right_forward_pwr = 0;

    IR_left_back_pwr = 1;
    for(int i = 0; i < LED_ON_TIME; i++)
    {
        initialLeftBackLED += IR_left_back.read();
    }
    IR_left_back_pwr = 0;

    IR_right_forward_pwr = 1;
    for(int i = 0; i < LED_ON_TIME; i++)
    {
        initialRightBackLED += IR_right_back.read();
    }
    IR_right_forward_pwr = 0;

    initialRightBackLED /= LED_ON_TIME;
    initialRightForwardLED /= LED_ON_TIME;
    initialLeftBackLED /= LED_ON_TIME;
    initialRightBackLED /= LED_ON_TIME;

    for(int i = 0; i < LED_HIST_SIZE; i++)
    {
        leftForwardLEDHistory[i] = 0;
        leftBackLEDHistory[i] = 0;
        rightForwardLEDHistory[i] = 0;
        rightBackLEDHistory[i] = 0;
    }
}


void LEDCollector::pollLEDs(int milliseconds = LED_ON_TIME)
{
    leftForwardLED = 0;
    rightForwardLED = 0;
    leftBackLED = 0;
    rightBackLED = 0;

    IR_left_forward_pwr = 1;
    for(int i = 0; i < milliseconds; i++)
    {
        leftForwardLED += IR_left_front.read();
    }
    IR_left_forward_pwr = 0;
    
    IR_right_forward_pwr = 1;
    for(int i = 0; i < milliseconds; i++)
    {
        rightForwardLED += IR_right_front.read();
    }
    IR_right_forward_pwr = 0;

    IR_left_back_pwr = 1;
    for(int i = 0; i < milliseconds; i++)
    {
        leftBackLED += IR_left_back.read();
    }
    IR_left_back_pwr = 0;

    IR_right_forward_pwr = 1;
    for(int i = 0; i < milliseconds; i++)
    {
        rightBackLED += IR_right_back.read();
    }
    IR_right_forward_pwr = 0;


    
    leftForwardLEDHistory[currHistPosition] = leftForwardLED / milliseconds;
    leftBackLEDHistory[currHistPosition] = leftBackLED / milliseconds;
    rightForwardLEDHistory[currHistPosition] = rightForwardLED / milliseconds;
    rightBackLEDHistory[currHistPosition] = rightBackLED / milliseconds;

    currHistPosition++;

    if(currHistPosition >= LED_HIST_SIZE)
    	currHistPosition = 0;
    
    detectWallChanges();
}


void LEDCollector::detectWallChanges()
{
    //analyze the LED History, seeing if the walls are changing.
    //This is one way of formatively creating the walls of the maze; the other way would be to monitor our location
    //and check if there's a wall each time we enter another square.
    //In fact it's probably simpler to implement it that way.
    //This method provides the small speed benefit of immediately knowing when the walls are changing patterns. 
    //However, it does take more memory
    
}