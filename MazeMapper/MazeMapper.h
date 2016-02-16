#ifndef MAZEMAPPER_H
#define MAZEMAPPER_H

#include "MazeMapper/Location.h"
#include "MazeMapper/Maze.h"
#include "MazeMapper/MazeConstants.h"
#include "LED/LEDCollector.h"

class Location;
class Maze;

class MazeMapper {
	public:
		MazeMapper();

		void updateLEDS();
		void updateAngle();
		void updatePosition();

		void decideDirection();
		void runFloodFill();
		void resetRun();

		bool isInGoalSquares();

	private:
		Maze map;
		LEDCollector led;
		Location currLoc;

};

#endif