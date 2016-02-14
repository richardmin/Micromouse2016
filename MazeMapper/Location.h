class Location {
	public:
		int getX();
		int getY();

		float getExactX();
		float getExactY();

		void update(float amtX, float amtY);
		void updateX(float amt);
		void updateY(float amt);

	private:
		float x;
		float y;
};

