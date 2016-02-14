#include "Location.h"


Location::Location() {
	x = 0, y = 0;
}
int Location::getX() {
	return (int) x;
}

int Location::getY() {
	return (int) y;
}

float Location::getExactX() {
	return x;
}

float Location::getExactY() {
	return y;
}

void Location::update(float amtX, float amtY) {
	x += amtX;
	y += amtY;
}

void Location::updateX(float amt) {
	x += amt;
}

void Location::updateY(float amt) {
	y += amt;
}

