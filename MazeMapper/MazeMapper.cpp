#include "MazeMapper/MazeMapper.h"

MazeMapper::MazeMapper()
{
    map = new Maze();
    led = new LEDCollector();
    currLoc = new Location();
    goals[1] = new Location();
    goals[2] = new Location();
    goals[3] = new Location();
    goals[4] = new Location();
}

void MazeMapper::updateLEDS()
{

}