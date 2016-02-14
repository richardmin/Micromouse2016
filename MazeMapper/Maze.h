#include "MazeConstants.h"
#include "Location.h"

class Maze {
	public:
		int calculateDistance(wall w, int PIDValue);
	private:
		int walls[MAZE_VERTICAL_WALLS][MAZE_HORIZONTAL_WALLS];
		
		bool forwardLeftWall;
		bool forwardRightWall;

		bool closeLeftWall;
		bool closeRightWall;


		void updateWalls(Wall w, int distance, Location l);

};

