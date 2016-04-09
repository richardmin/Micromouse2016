#ifndef MAZE_CONSTANTS_H
#define MAZE_CONSTANTS_H

//Wall types
enum Wall {frontLeft, frontRight, closeLeft, closeRight};

//Maze constants
const int MAZE_WIDTH = 36;
const int MAZE_HEIGHT = 36;

const int MAZE_VERTICAL_WALLS = MAZE_WIDTH + 1;
const int MAZE_HORIZONTAL_WALLS = MAZE_HEIGHT + 1;


#endif