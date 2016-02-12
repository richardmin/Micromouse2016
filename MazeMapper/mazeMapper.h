

enum wall {frontLeft, frontRight, closeLeft, closeRight};



class Maze {
	public:
		void updateWalls(wall w, int distance, Location l);
		int calculateDistance(wall w, int PIDValue);
	private:
		int width = 36;
		int height = 36;
		int walls[height+1][width+1];
		
		bool forwardLeftWall;
		bool forwardRightWall;

		bool closeLeftWall;
		bool closeRightWall;
}
