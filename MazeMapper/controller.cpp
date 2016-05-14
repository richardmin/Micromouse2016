#include <iostream>
#include <stack>
#include <cstdlib>  // atoi

#include "Maze.h"
#include "MazeDefinitions.h"
#include "PathFinder.h"




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
    FloodFillFinder(bool shouldPause = false) : pause(shouldPause) {
        coords.clearAll();
        horizontal.clearAll();
        vertical.clearAll();
        heading = NORTH;
        
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
        
        
        coord block;
        block.x = x;
        block.y = y;

        //todo: CREATE IR (so we can tell if there's walls in front or not)
        bool front = maze.wallInFront();
        bool left = maze.wallOnLeft();
        bool right = maze.wallOnRight();
        
        //todo: reverse the goal and reset the values. 
        if(isAtCenter(x, y))
            return Finish;
        
        
        switch(heading)
        {
            case NORTH:
            {
                if(front)
                    horizontal.set(y+1,x);
                if(left)
                    vertical.set(y,x);
                if(right)
                    vertical.set(y,x+1);
                break;
            }
            case EAST:
            {
                if(front)
                    vertical.set(y,x+1);
                if(left)
                    horizontal.set(y+1,x);
                if(right)
                    horizontal.set(y,x);
                break;
            }
            case WEST:
            {
                
                if(front)
                    vertical.set(y,x);
                if(left)
                    horizontal.set(y,x);
                if(right)
                    horizontal.set(y+1,x);
                break;
            }
            case SOUTH:
            {
                if(front)
                    horizontal.set(y,x);
                if(left)
                    vertical.set(y,x+1);
                if(right)
                    vertical.set(y,x);
                break;
            }
            case INVALID:
            //cry
            {
            }
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
    
    const bool pause;
private:
    BitVector256 coords;
    BitVector256 vertical;
    BitVector256 horizontal;
    Dir heading;
    unsigned int Distance[MazeDefinitions::MAZE_LEN][MazeDefinitions::MAZE_LEN];
    const unsigned int infinity = ~0;
    
    
};

void begin_controller() {
    MazeDefinitions::MazeEncodingName mazeName = MazeDefinitions::MAZE_CAMM_2012;
    bool pause = true;
    
    FloodFillFinder floodFill(pause);
    Maze maze(mazeName, &floodFill);
    
    maze.start();
}





