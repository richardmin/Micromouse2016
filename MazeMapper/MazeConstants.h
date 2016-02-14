#ifndef MAZE_CONSTANTS_H
#define MAZE_CONSTANTS_H

//Wall types
enum Wall {frontLeft, frontRight, closeLeft, closeRight};

//Maze constants
int MAZE_WIDTH = 36;
int MAZE_HEIGHT = 36;

int MAZE_VERTICAL_WALLS = MAZE_WIDTH + 1;
int MAZE_HORIZONTAL_WALLS = MAZE_HEIGHT + 1;


#endif