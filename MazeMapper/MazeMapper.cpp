#include "MazeMapper/MazeMapper.h"

MazeMapper::MazeMapper()
{
    map = new Maze();
    led = new LEDCollector();
    //gyro
    currLoc = new Location(0, 0);
}

void MazeMapper::updateLEDS()
{
	
}

void MazeMapper::updateAngle()
{
	//code to read from gyro
}

void MazeMapper::updatePosition()
{
	//code to read encoders (pid dump?)
}

void MazeMapper::decideDirection()
{

}

void MazeMapper::resetRun()
{
	currLoc.updateX(0);
	currLoc.updateY(0);
}

void MazeMapper::runFloodFill()
{
	//please note that we should do this in an FSM so that we can make decisions instantaneously.
	//This means that floodfill should actually be a separate thread.
	//I'll set this up later.
	pthread_create(&floodFillThread, NULL, floodFill, NULL);

}

void MazeMapper::floodFillThread()
{
	//take position globals + LED data to map maze
	
	pthread_exit(NULL);
}

bool MazeMapper::isInGoalSquares(Location l)
{
	return l.getX() < MAZE_WIDTH/2+1 && l.getX() > MAZE_WIDTH/2-1
			&& l.getY() < MAZE_HEIGHT/2+1 && l.getY() > MAZE_HEIGHT/2-1; 
}
