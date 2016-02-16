#ifndef MAZEMAPPER_H
#define MAZEMAPPER_H

#include "MazeMapper/Location.h"
#include "MazeMapper/Maze.h"
#include "LED/LEDCollector.h"

class Location;
class Maze;

class MazeMapper {
	public:
		MazeMapper();
		void updateLEDS();
	private:
		Maze map;
		LEDCollector led;
		Location currLoc;
		Location goals[4];

};

#endif