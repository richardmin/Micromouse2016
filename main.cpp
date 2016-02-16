#include "mbed.h"
#include "AVEncoder.h"
#include "PINS/pinouts.h"
#include "MazeMapper/Maze.h"
#include "MazeMapper/Location.h"
#include "LED/LEDCollector.h"
#include "PID/pidController.h"
#include "PID/pidConstants.h"


int main()  
{    
    Location curLoc = new Location(0, 0);
    pidController pid = new pidController();
    

    //prepare pid
    interrupt.attach_us(&pid.pid(), 1000);
    timer.start();
    
    while(1)
    {
        //do maze mapping code here
    }
}
