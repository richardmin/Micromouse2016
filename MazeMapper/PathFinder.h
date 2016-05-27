#ifndef PathFinder_h
#define PathFinder_h

#include <string>

class Maze;

enum MouseMovement {
    MoveForward,
    MoveBackward,
    TurnClockwise,
    TurnCounterClockwise,
    TurnAround,
    Finish
};

class PathFinder {
public:
    virtual ~PathFinder() {}

    /**
     * Function that instructs the maze how to move the mouse.
     *
     * The PathFinder subclass has complete control of where the mouse
     * goes at all times, and the loop will be broken when this method
     * returns MouseMovement::Finish.
     *
     * Implement this method in your subclass (and other methods) for your path finding.
     *
     * @param x: current column of the mouse (0 is the left-most side of the maze)
     * @param y: current row of the mouse (0 is bottom of the maze)
     * @param maze: the maze object that can be queried for current wall positions
     */
    virtual MouseMovement nextMovement(unsigned x, unsigned y, const Maze &maze) = 0;
};

#endif

