#include "MazeMapper/Maze.h"

Maze::Maze()
{
    //initialize the walls to not be existent: 0 means they dont' exist
    for(int i = 0; i < MAZE_VERTICAL_WALLS; i++)
    {
        for(int j = 0; j < MAZE_HORIZONTAL_WALLS; j++)
        {
            walls[i][j] = 0;
        }
    }

    //initialize the edge walls
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
        adjustedX = l.getX();
        adjustedY = l.getY()+2;
    }
    else if (w == frontRight)
    {
        adjustedX = l.getX()+1;
        adjustedY = l.getY()+2;
    }
    //for now, we assume there's no collisions
    walls[adjustedX][adjustedY] = 1;
}

int Maze::calculateDistance(Wall w, int PIDValue)
{
    //this may not be the right place for this function
    return -1;
}

bool Maze::hasNorthWall(Location l)
{
    return walls[l.getX()][l.getY()];
}

bool Maze::hasSouthWall(Location l)
{
    return walls[l.getX()][l.getY()+1];
}

bool Maze::hasEastWall(Location l)
{
    return walls[l.getX()+1][l.getY()];
}

bool Maze::hasWestWall(Location l)
{
    return walls[l.getX()][l.getY()];
}