#include "MazeMapper/Location.h"


Location::Location() 
{
	x = 0, y = 0;
}

Location::Location(int X, int Y)
{
	x = X, y = Y;
}
int Location::getX() 
{
	return (int) x;
}

int Location::getY() 
{
	return (int) y;
}

float Location::getExactX() 
{
	return x;
}

float Location::getExactY() 
{
	return y;
}

void Location::update(float amtX, float amtY) 
{
	x += amtX;
	y += amtY;
}

void Location::updateX(float amt) 
{
	x += amt;
}

void Location::updateY(float amt) 
{
	y += amt;
}

void Location::set(float X, float Y)
{
	x = X;
	y = Y;
}