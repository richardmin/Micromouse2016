
#include <iostream>
#include <queue>

#include "MazeMapper/PathFinder.h"
#include "MazeMapper/Maze.h"
#include "MazeMapper/Dir.h"
#include "MazeMapper/MazeDefinitions.h"

class FloodFillFollower : public PathFinder {
	public:
		FloodFillFollower(bool shouldPause = false) : pause(shouldPause) {
			shouldGoForward = false;
			visitedStart = false;
			for(int i = 0; i < MazeDefinitions::MAZE_LEN; i++)
			{
				for(int j = 0; j < MazeDefinitions::MAZE_LEN; j++)
				{
					explored[i][j] = false;
					distance[i][k] = -1;
				}
			}
		}
		MouseMovement nextMovement(unsigned x, unsigned y, const Maze &maze);
		~FloodFillFollower() {
			//blank.
		}
	protected:
		bool shouldGoForward;
		bool visitedStart;
		const bool pause;
		  bool isAtCenter(unsigned x, unsigned y) const {
        unsigned midpoint = MazeDefinitions::MAZE_LEN / 2;

        if(MazeDefinitions::MAZE_LEN % 2 != 0) {
            return x == midpoint && y == midpoint;
        }

        return  (x == midpoint     && y == midpoint    ) ||
        (x == midpoint - 1 && y == midpoint    ) ||
        (x == midpoint     && y == midpoint - 1) ||
        (x == midpoint - 1 && y == midpoint - 1);
    }
	private:
		bool getExplored(unsigned x, unsigned y) const;
		void setExplored(unsigned x, unsigned y);
		
		void floodfill();

		void checkWalls(unsigned x, unsigned y, const Maze &maze);
		MouseMovement getBestDirection(unsigned x, unsigned y, const Maze &maze);

		std::queue<MouseMovement> currQueue;	
		bool explored[MazeDefinitions::MAZE_LEN][MazeDefinitions::MAZE_LEN];
		int distance[MazeDefinitions::MAZE_LEN][MazeDefinitions::MAZE_LEN]
	
		
};
