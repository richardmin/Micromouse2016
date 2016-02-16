#include "LED/IRLEDConstants.h"
#include "PINS/pinouts.h" 

#ifndef LEDCOLLECTOR_H
#define LEDCOLLECTOR_H


class LEDCollector 
{
	public:
		void pollLEDs(int milliseconds);
		LEDCollector();
	private:
		float leftForwardLED = 0;
		float rightForwardLED = 0;
		float leftBackLED = 0;
		float rightBackLED = 0;

		float leftForwardLEDHistory[LED_HIST_SIZE];
		float leftBackLEDHistory[LED_HIST_SIZE];
		float rightForwardLEDHistory[LED_HIST_SIZE];
		float rightBackLEDHistory[LED_HIST_SIZE];

		float initialLeftForwardLED = 0;
		float initialRightForwardLED = 0;
		float initialLeftBackLED = 0;
		float initialRightBackLED = 0;

		void detectWallChanges();

};
#endif