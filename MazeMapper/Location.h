#ifndef LOCATION_H
#define LOCATION_H

class Location {
	public:
		int getX();
		int getY();

		float getExactX();
		float getExactY();

		void update(float amtX, float amtY);
		void updateX(float amt);
		void updateY(float amt);

		void set(float X, float Y);

	private:
		float x;
		float y;
};


#endif