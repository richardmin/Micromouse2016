#ifndef MAZE_H
#define MAZE_H

#include "MazeConstants.h"
#include "Location.h"

class Maze {
	public:
		int calculateDistance(Wall w, int PIDValue);
	private:
		int walls[MAZE_VERTICAL_WALLS][MAZE_HORIZONTAL_WALLS]; //array of walls
		
		bool forwardLeftWall;
		bool forwardRightWall;

		bool closeLeftWall;
		bool closeRightWall;

		void updateWalls(Wall w, int distance, Location l);
};

#endif