void Maze::updateWalls(wall w, int distance, Location l)
{
	int adjustedX, adjustedY;
	if(w == frontLeft)
	{
		adjustedX = l.x;
		adjustedY = l.y+2;
	}
	else if (w == frontRight)
	{
		adjustedX = l.x+1;
		adjustedY = l.y+2;
	}
	//for now, we assume there's no collisions
	walls[adjustedX][adjustedY] = 1;
}


Maze::Maze()
{
	for(int i = 0; i < width+1; i++)
	{
		for(int j = 0; j < height+1; j++)
		{
			walls[i][j] = 0;
		}
	}

	for(int i = 0; i < width+1; i++)
	{
		walls[i][0] = 1;
		walls[0][i] = 1;
		walls[i][width] = 1;
		walls[height][i] = 1;
	}


}