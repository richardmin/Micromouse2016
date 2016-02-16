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


    while(1); //back up straight, until we give go ahead to PIDs

    //this way the LED's get initialized properly
    MazeRunner runner = new MazeRunner();
    MazeMapper mapper = new MazeMapper();
    
    //prepare pid
    interrupt.attach_us(&pid.pid(), 1000);
    timer.start();
    
    bool hasMapped = false;

    while(!hasMapped) //do mapping mode until we decide otherwise
    {
        
    }

    //attach an interrrupt for a button to reset run (location reset to 0)
}
