

int width = 36;
int height = 36;

int walls[width+1][height+1]; //array of the walls. 

int forwardLeftWall; //do we have the forward facing left wall
int forwardRightWall; //do we have the forward facing right wall

int closeLeftWall;
int closeRightWall;

enum wall {frontLeft, frontRight, closeLeft, closeRight};

int calculateDistance(wall w, int PIDValue);
int notify(wall w, int distance);