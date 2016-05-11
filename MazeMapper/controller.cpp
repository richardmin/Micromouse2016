#include <iostream>
#include <stack>
#include <cstdlib>  // atoi

#include "MazeMapper/Maze.h"
#include "MazeMapper/MazeDefinitions.h"
#include "MazeMapper/PathFinder.h"




unsigned int Manhattan(unsigned int coordX, unsigned int coordY, unsigned int centX, unsigned int centY){
    unsigned int Distance = 0;
    
    // Absolute difference between x coordinates
    Distance =  coordX < centX ? centX - coordX : coordX - centX;
    Distance += coordY < centY ? centY - coordY : coordY - centY;
    
    return Distance;
}

struct coord{
    unsigned int x;
    unsigned int y;
};

class FloodFillFinder : public PathFinder {
public:
    // Constructor
    FloodFillFinder(): heading(NORTH){
        coords.clearAll();
        horizontal.clearAll();
        vertical.clearAll();
        infinity = ~0;
        
        
        for(int i = MazeDefinitions::MAZE_LEN/2; i < MazeDefinitions::MAZE_LEN; i++){
            for(int j = 0; j < MazeDefinitions::MAZE_LEN/2; j++)
                Distance[i][j] = Manhattan(i, j, MazeDefinitions::MAZE_LEN/2, MazeDefinitions::MAZE_LEN/2 - 1);
        }
        for(int i = MazeDefinitions::MAZE_LEN/2; i < MazeDefinitions::MAZE_LEN; i++){
            for(int j = MazeDefinitions::MAZE_LEN/2; j < MazeDefinitions::MAZE_LEN; j++)
                Distance[i][j] = Manhattan(i, j, MazeDefinitions::MAZE_LEN/2, MazeDefinitions::MAZE_LEN/2);
        }
        for(int i = 0; i < MazeDefinitions::MAZE_LEN/2; i++){
            for(int j = 0; j < MazeDefinitions::MAZE_LEN/2; j++)
                Distance[i][j] = Manhattan(i, j, MazeDefinitions::MAZE_LEN/2 - 1, MazeDefinitions::MAZE_LEN/2 - 1);
        }
        
        for(int i = 0; i < MazeDefinitions::MAZE_LEN/2; i++){
            for(int j = MazeDefinitions::MAZE_LEN/2; j < MazeDefinitions::MAZE_LEN; j++)
                Distance[i][j] = Manhattan(i, j, MazeDefinitions::MAZE_LEN/2 - 1, MazeDefinitions::MAZE_LEN/2);
        }
        for(int i = 0; i < MazeDefinitions::MAZE_LEN; i++)
            horizontal.set(0, i);
        for(int i = 0; i < MazeDefinitions::MAZE_LEN; i++)
            vertical.set(i, 0);
        
    }
    
    unsigned int getDistance(unsigned int x, unsigned int y){
        if(((int)x < 0 || x >= MazeDefinitions::MAZE_LEN)
           || ((int)y < 0 || y >= MazeDefinitions::MAZE_LEN))
            return infinity;
        return Distance[x][y];
    }
    
    
    void floodFillFinder(coord block, unsigned int Distance[MazeDefinitions::MAZE_LEN][MazeDefinitions::MAZE_LEN],
                         BitVector256 horizontal, BitVector256 vertical){
        std::stack<coord> coords;
        BitVector256 visited;
        
        visited.clearAll();
        coords.push(block);
        while(!coords.empty())
        {
            
            unsigned int row = (coords.top()).y;
            unsigned int col = (coords.top()).x;
            coords.pop();
            
            visited.set(row,col);
            
            if(Distance[col][row] == 0 || Distance[col][row] == infinity)
                continue;
            
            unsigned int shortest = ~0;
            
            Dir face = NORTH;
            for(int i = 0; i < 4; i++, face = clockwise(face))
            {
                unsigned int nextX = col;
                unsigned int nextY = row;
                bool hasWall = false;
                
                switch(face){
                    case NORTH:
                        nextY += 1;
                        hasWall = horizontal.get(nextY, nextX);
                        break;
                    case EAST:
                        nextX += 1;
                        hasWall = vertical.get(nextY, nextX);
                        break;
                    case SOUTH:
                        nextY -= 1;
                        hasWall = horizontal.get(row,col);
                        break;
                    case WEST:
                        nextX -= 1;
                        hasWall = vertical.get(row,col);
                        break;
                }
                
                if(((int)nextX < 0 || nextX >= MazeDefinitions::MAZE_LEN)
                   || ((int)nextY < 0 || nextY >= MazeDefinitions::MAZE_LEN))
                    continue;
                
                
                if(!hasWall){
                    if(Distance[nextX][nextY] < shortest)
                        shortest = Distance[nextX][nextY];
                    if(!visited.get(nextY, nextX)){
                        coord n;
                        n.x = nextX;
                        n.y = nextY;
                        coords.push(n);
                    }
                }
            }
            
            if(shortest == infinity || Distance[col][row] == (shortest + 1))
                continue;
            
            Distance[col][row] = shortest + 1;
            
            
            coord n;
            n.x = col;
            n.y = row+1;
            coords.push(n);
            
            n.x = col+1;
            n.y = row;
            coords.push(n);
            
            n.x = col;
            n.y = row-1;
            if(row != 0)
                coords.push(n);
            
            n.x = col-1;
            n.y = row;
            if(col != 0)
                coords.push(n);
            
        }
    }
    
    
    MouseMovement nextMovement(unsigned x, unsigned y, const Maze& maze)
    {
        
        std::cout << "Hit enter to continue..." << std::endl;
        std::cin.ignore(10000, '\n');
        std::cin.clear();
        std::cout << maze.draw(5) << std::endl << std::endl;
        
        coord block;
        block.x = x;
        block.y = y;
        bool front = maze.wallInFront();
        bool left = maze.wallOnLeft();
        bool right = maze.wallOnRight();
        
        if(isAtCenter(x, y))
            return Finish;
        
        
        if(heading == NORTH){
            if(front)
                horizontal.set(y+1,x);
            if(left)
                vertical.set(y,x);
            if(right)
                vertical.set(y,x+1);
        }
        else if(heading == EAST){
            if(front)
                vertical.set(y,x+1);
            if(left)
                horizontal.set(y+1,x);
            if(right)
                horizontal.set(y,x);
        }
        else if(heading == SOUTH){
            if(front)
                horizontal.set(y,x);
            if(left)
                vertical.set(y,x+1);
            if(right)
                vertical.set(y,x);
        }
        else{
            if(front)
                vertical.set(y,x);
            if(left)
                horizontal.set(y,x);
            if(right)
                horizontal.set(y+1,x);
        }
        
        
        if((getDistance(x,y) <= getDistance(x,y+1) || horizontal.get(y+1,x))
           && (getDistance(x,y) <= getDistance(x,y-1) || horizontal.get(y,x))
           && (getDistance(x,y) <= getDistance(x+1,y) || vertical.get(y,x+1))
           && (getDistance(x,y) <= getDistance(x-1,y) || vertical.get(y,x)) ){
            floodFillFinder(block, Distance, horizontal, vertical);
        }
        else if(front && left && right)
            floodFillFinder(block, Distance, horizontal, vertical);
        
        
        if(maze.wallInFront()){
            if(left && right){
                heading = opposite(heading);
                return TurnAround;
            }
            else if(left){
                heading = clockwise(heading);
                return TurnClockwise;
            }
            else{
                heading = counterClockwise(heading);
                return TurnCounterClockwise;
            }
        }
        
        
        if(!coords.get(y,x)){
            coords.set(y,x);
            return MoveForward;
        }
        
        left = maze.wallOnLeft();
        right = maze.wallOnRight();
        unsigned int shortest;
        MouseMovement dir = MoveForward;
        switch(heading){
            case NORTH:
                shortest = Distance[x][y+1];
                if(!left){
                    if(shortest > Distance[x-1][y]){
                        shortest = Distance[x-1][y];
                        heading = counterClockwise(heading);
                        dir = TurnCounterClockwise;
                    }
                }
                if(!right){
                    if(shortest > Distance[x+1][y]){
                        shortest = Distance[x+1][y];
                        heading = clockwise(heading);
                        dir = TurnClockwise;
                    }
                }
                break;
            case EAST:
                shortest = Distance[x+1][y];
                if(!left){
                    if(shortest > Distance[x][y+1]){
                        shortest = Distance[x][y+1];
                        heading = counterClockwise(heading);
                        dir = TurnCounterClockwise;
                    }
                }
                if(!right){
                    if(shortest > Distance[x][y-1]){
                        shortest = Distance[x][y-1];
                        heading = clockwise(heading);
                        dir = TurnClockwise;
                    }
                }
                break;
            case SOUTH:
                shortest = Distance[x][y-1];
                if(!left){
                    if(shortest > Distance[x+1][y]){
                        shortest = Distance[x+1][y];
                        heading = counterClockwise(heading);
                        dir = TurnCounterClockwise;
                    }
                }
                if(!right){
                    if(shortest > Distance[x-1][y]){
                        shortest = Distance[x-1][y];
                        heading = clockwise(heading);
                        dir = TurnClockwise;
                    }
                }
                break;
            case WEST:
                shortest = Distance[x-1][y];
                if(!left){
                    if(shortest > Distance[x][y-1]){
                        shortest = Distance[x][y-1];
                        heading = counterClockwise(heading);
                        dir = TurnCounterClockwise;
                    }
                }
                if(!right){
                    if(shortest > Distance[x][y+1]){
                        shortest = Distance[x][y+1];
                        heading = clockwise(heading);
                        dir = TurnClockwise;
                    }
                }
                break;
        }
        
        return dir;
        
    }
protected:
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
    BitVector256 coords;
    BitVector256 vertical;
    BitVector256 horizontal;
    Dir heading;
    unsigned int Distance[MazeDefinitions::MAZE_LEN][MazeDefinitions::MAZE_LEN];
    unsigned int infinity;
};

/**
 * Demo of a PathFinder implementation.
 *
 * Do not use a left/right wall following algorithm, as most
 * Micromouse mazes are designed for such algorithms to fail.
 */
class LeftWallFollower : public PathFinder {
public:
    LeftWallFollower(bool shouldPause = false) : pause(shouldPause) {
        shouldGoForward = false;
        visitedStart = false;
    }
    
    MouseMovement nextMovement(unsigned x, unsigned y, const Maze &maze) {
        const bool frontWall = maze.wallInFront();
        const bool leftWall  = maze.wallOnLeft();
        
        // Pause at each coord if the user requests it.
        // It allows for better viewing on command line.
        if(pause) {
            std::cout << "Hit enter to continue..." << std::endl;
            std::cin.ignore(10000, '\n');
            std::cin.clear();
        }
        
        std::cout << maze.draw(5) << std::endl << std::endl;
        
        // If we somehow miraculously hit the center
        // of the maze, just terminate and celebrate!
        if(isAtCenter(x, y)) {
            std::cout << "Found center! Good enough for the demo, won't try to get back." << std::endl;
            return Finish;
        }
        
        // If we hit the start of the maze a second time, then
        // we couldn't find the center and never will...
        if(x == 0 && y == 0) {
            if(visitedStart) {
                std::cout << "Unable to find center, giving up." << std::endl;
                return Finish;
            } else {
                visitedStart = true;
            }
        }
        
        // If we have just turned left, we should take that path!
        if(!frontWall && shouldGoForward) {
            shouldGoForward = false;
            return MoveForward;
        }
        
        // As long as nothing is in front and we have
        // a wall to our left, keep going forward!
        if(!frontWall && leftWall) {
            shouldGoForward = false;
            return MoveForward;
        }
        
        // If our forward and left paths are blocked
        // we should try going to the right!
        if(frontWall && leftWall) {
            shouldGoForward = false;
            return TurnClockwise;
        }
        
        // Lastly, if there is no left wall we should take that path!
        if(!leftWall) {
            shouldGoForward = true;
            return TurnCounterClockwise;
        }
        
        // If we get stuck somehow, just terminate.
        std::cout << "Got stuck..." << std::endl;
        return Finish;
    }
    
protected:
    // Helps us determine that we should go forward if we have just turned left.
    bool shouldGoForward;
    
    // Helps us determine if we've made a loop around the maze without finding the center.
    bool visitedStart;
    
    // Indicates we should pause before moving to next coord.
    // Useful for command line usage.
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
};

int main(int argc, char * argv[]) {
    MazeDefinitions::MazeEncodingName mazeName = MazeDefinitions::MAZE_CAMM_2012;
    bool pause = true;
    
    // Since Windows does not support getopt directly, we will
    // have to parse the command line arguments ourselves.
    
    // Skip the program name, start with argument index 1
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-m") == 0 && i+1 < argc) {
            int mazeOption = atoi(argv[++i]);
            if(mazeOption < MazeDefinitions::MAZE_NAME_MAX && mazeOption > 0) {
                mazeName = (MazeDefinitions::MazeEncodingName)mazeOption;
            }
        } else if(strcmp(argv[i], "-p") == 0) {
            pause = true;
        } else {
            std::cout << "Usage: " << argv[0] << " [-m N] [-p]" << std::endl;
            std::cout << "\t-m N will load the maze corresponding to N, or 0 if invalid N or missing option" << std::endl;
            std::cout << "\t-p will wait for a newline in between coord traversals" << std::endl;
            return -1;
        }
    }
    FloodFillFinder testRun;
    LeftWallFollower leftWallFollower(pause);
    Maze maze(mazeName, &testRun);
    std::cout << maze.draw(5) << std::endl << std::endl;
    
    maze.start();
}




