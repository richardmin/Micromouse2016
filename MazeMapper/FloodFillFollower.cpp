#include "MazeMapper/FloodFillFollower.h"

// FloodFillFollower::FloodFillFollower(bool shouldPause) : pause (shouldPause)
// {
// 	shouldGoForward = false;
// 	visitedStart = false;
// 	for(int i = 0; i < MazeDefinitions::MAZE_LEN; i++)
// 		for(int j = 0; j < MazeDefinitions::MAZE_LEN; j++)
// 			explored[i][j] = false;
// }

MouseMovement FloodFillFollower::nextMovement(unsigned x, unsigned y, const Maze &maze)
{
	if(pause) 
	{
		std::cout << "Hit enter to continue..." << std::endl;
		std::cin.ignore(10000, '\n');
		std::cin.clear();
	}

	std::cout << maze.draw(5) << std::endl << std::endl;

	MouseMovement next;

		//TODO: implement the maze fill algorithm here
	if(currQueue.empty())
	{
		if (FloodFillFollower::getExplored(x, y) == false)
         {
            FloodFillFollower::checkWalls(x, y, maze);
            FloodFillFollower::setExplored(x, y);
         }
         if (FloodFillFollower::isAtCenter())
         {
			//TODO: go back
   			std::cout << "Found center!" << std::endl;
   			return Finish;
         }
         next = FloodFillFollower::getBestDirection(x, y, maze);
         if (nextDirection == TurnCounterClockwise || nextDirection == TurnClockwise)
         {
            currQueue.push(MoveForward);
         }
	}
	else
	{
         next = currQueue.pop();
	}	
	return next;
}

bool FloodFillFollower::getExplored(unsigned x, unsigned y) const
{
	return explored[x][y];
}

void FloodFillFollower::setExplored(unsigned x, unsigned y)
{
	explored[x][y] = true;
}

void FloodFillFollower::checkWalls(unsigned x, unsigned y, const Maze &maze)
{
      if (maze.wallInFront())
      {
         maze.setWall(cell.getX(), cell.getY(), direction.getIndex());
         if (hasSameDirection(cell, Direction2.getDirection(direction)))
         {
            setDirection(cell, getDirection(cell) ^ Direction2.getDirection(direction).getIndex());
         }
      }
      if (maze.wallOnLeft())
      {
         maze.setWall(cell.getX(), cell.getY(), direction.getLeft().getIndex());
         setDirection(cell,
                      getDirection(cell) &
                            (Direction2.Mask.getIndex() ^ Direction2.getDirection(direction.getLeft()).getIndex()));
      }
      if (maze.wallOnRight())
      {
         maze.setWall(cell.getX(), cell.getY(), direction.getRight().getIndex());
         setDirection(cell,
                      getDirection(cell) &
                            (Direction2.Mask.getIndex() ^ Direction2.getDirection(direction.getRight()).getIndex()));
      }
}

MouseMovement FloodFillFollower::getBestDirection(unsigned x, unsigned y, const Maze &maze)
{

      Dir current = robotLocation.getDirection();
      MouseMovement bestDirection = null;

      if (hasSameDirection(here, Direction2.getDirection(current)))
      //&& (robotLocation.isWallFront() == false))
      {
         bestDirection = current;
      }
      else if (hasSameDirection(here, Direction2.getDirection(Direction.North)))
      //&& (maze.getWall(here, Direction.North).isSet() == false))
      {
         bestDirection = Direction.North;
      }
      else if (hasSameDirection(here, Direction2.getDirection(Direction.East)))
      //&& (maze.getWall(here, Direction.East).isSet() == false))
      {
         bestDirection = Direction.East;
      }
      else if (hasSameDirection(here, Direction2.getDirection(Direction.South)))
      //&& (maze.getWall(here, Direction.South).isSet() == false))
      {
         bestDirection = Direction.South;
      }
      else if (hasSameDirection(here, Direction2.getDirection(Direction.West)))
      //&& (maze.getWall(here, Direction.West).isSet() == false))
      {
         bestDirection = Direction.West;
      }

      if (bestDirection == null)
      {
         modifiedFloodfill();
         return getBestDirection();
      }
      else
      {
         return bestDirection;
      }
}


void floodfill()
{
	  Dimension size = maze.getSize();
      Vector<MazeCell> queue = new Vector<MazeCell>();
      MazeCell cell;
      int currentDistance;
      boolean speedy;

      for (int i = 1; i <= size.width; i++)
      {
         for (int j = 1; j <= size.height; j++)
         {
            MazeCell here = MazeCell.valueOf(i, j);
            setDistance(here, USELESS);
            setDirection(here, 0);
         }
      }

      if (goal == TO_START)
      {
         cell = MazeCell.valueOf(1, size.height);
         setDistance(cell, 0);
         queue.add(cell);
         speedy = false;
      }
      else
      {
         int targetX = size.width / 2;
         int targetY = size.height / 2;
         cell = MazeCell.valueOf(targetX, targetY);
         setDistance(cell, 0);
         queue.add(cell);
         cell = MazeCell.valueOf(targetX + 1, targetY);
         setDistance(cell, 0);
         queue.add(cell);
         cell = MazeCell.valueOf(targetX, targetY + 1);
         setDistance(cell, 0);
         queue.add(cell);
         cell = MazeCell.valueOf(targetX + 1, targetY + 1);
         setDistance(cell, 0);
         queue.add(cell);
         if ( (speedRun == true) && (speedRunCapable == true))
         {
            speedy = true;
         }
         else
         {
            speedy = false;
         }
      }

      while (queue.isEmpty() == false)
      {
         cell = queue.get(0);
         queue.remove(0);
         currentDistance = getDistance(cell);

         //Check to see if accessible
         if (maze.getWall(cell, Direction.North).isSet() == false)
         { //Check to see if it should be added to queue
            if ( ( (currentDistance + 1) < getNeighborDistance(cell, Direction.North)) &&
                ( (speedy == false) || (getNeighborExplored(cell, Direction.North) == true)))
            {
               queue.add(cell.plusY(-1));
               setDistance(cell.plusY(-1), currentDistance + 1);
               setDirection(cell.plusY(-1), Direction2.South.getIndex());
            }
            else if ( ( (currentDistance + 1) == getNeighborDistance(cell, Direction.North)) &&
                     ( (speedy == false) || (getNeighborExplored(cell, Direction.North) == true)))
            {
               setDirection(cell.plusY(-1), Direction2.South.getIndex() +
                                            getDirection(cell.plusY(-1)));
            }
         }

         //Check to see if accessible
         if (maze.getWall(cell, Direction.South).isSet() == false)
         { //Check to see if it should be added to queue
            if ( ( (currentDistance + 1) < getNeighborDistance(cell, Direction.South)) &&
                ( (speedy == false) || (getNeighborExplored(cell, Direction.South))))
            {
               queue.add(cell.plusY(1));
               setDistance(cell.plusY(1), currentDistance + 1);
               setDirection(cell.plusY(1), Direction2.North.getIndex());
            }
            else if ( ( (currentDistance + 1) == getNeighborDistance(cell, Direction.South)) &&
                     ( (speedy == false) || (getNeighborExplored(cell, Direction.South) == true)))
            {
               setDirection(cell.plusY(1), Direction2.North.getIndex() +
                                           getDirection(cell.plusY(1)));
            }
         }

         //Check to see if accessible
         if (maze.getWall(cell, Direction.West).isSet() == false)
         { //Check to see if it should be added to queue
            if ( ( (currentDistance + 1) < getNeighborDistance(cell, Direction.West)) &&
                ( (speedy == false) || (getNeighborExplored(cell, Direction.West))))
            {
               queue.add(cell.plusX(-1));
               setDistance(cell.plusX(-1), currentDistance + 1);
               setDirection(cell.plusX(-1), Direction2.East.getIndex());
            }
            else if ( ( (currentDistance + 1) == getNeighborDistance(cell, Direction.West)) &&
                     ( (speedy == false) || (getNeighborExplored(cell, Direction.West) == true)))
            {
               setDirection(cell.plusX(-1), Direction2.East.getIndex() +
                                            getDirection(cell.plusX(-1)));
            }

         }

         //Check to see if accessible
         if (maze.getWall(cell, Direction.East).isSet() == false)
         { //Check to see if it should be added to queue
            if ( ( (currentDistance + 1) < getNeighborDistance(cell, Direction.East)) &&
                ( (speedy == false) || (getNeighborExplored(cell, Direction.East))))
            {
               queue.add(cell.plusX(1));
               setDistance(cell.plusX(1), currentDistance + 1);
               setDirection(cell.plusX(1), Direction2.West.getIndex());
            }
            else if ( ( (currentDistance + 1) == getNeighborDistance(cell, Direction.East)) &&
                     ( (speedy == false) || (getNeighborExplored(cell, Direction.East) == true)))
            {
               setDirection(cell.plusX(1), Direction2.West.getIndex() + getDirection(cell.plusX(1)));
            }
         }
      }

      MazeCell here = robotLocation.getCurrentLocation();
      if (getDistance(here) == USELESS)
      {
         System.out.println("Purging Knowledge");
         maze.clearMaze();
         speedRunCapable = false;
         for (int i = 0; i < size.width; i++)
         {
            for (int j = 0; j < size.height; j++)
            {
               explored[i][j] = false;
            }
         }
         explored[here.getX() - 1][here.getY() - 1] = true;
         checkWalls();
         floodfill();
      }
}