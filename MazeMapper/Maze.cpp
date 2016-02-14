#include "Maze.h"


Maze::Maze()
{
	for(int i = 0; i < MAZE_VERTICAL_WALLS; i++)
	{
		for(int j = 0; j < MAZE_HORIZONTAL_WALLS; j++)
		{
			walls[i][j] = 0;
		}
	}

	for(int i = 0; i < MAZE_VERTICAL_WALLS; i++)
	{
		walls[i][0] = 1;
		walls[i][MAZE_WIDTH] = 1;
		for(int j = 0; j < MAZE_HORIZONTAL_WALLS; j++)
		{
			walls[MAZE_HEIGHT][j] = 1;
			walls[0][j] = 1;
		}
	}


}

void Maze::updateWalls(Wall w, int distance, Location l)
{
	int adjustedX, adjustedY;
	if(w == frontLeft)
	{
		adjustedX = l.x;
		adjustedY = l.y+2;
	}
	else if (w == frontRight)
	{
		adjustedX = l.x+1;
		adjustedY = l.y+2;
	}
	//for now, we assume there's no collisions
	walls[adjustedX][adjustedY] = 1;
}

void Maze::calculateDistance(Wall w, int PIDValue)