#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<unistd.h>
#include"graphics.h"
#include"GameOfLife.h"
#include <sys/time.h>

//const int windowWidth = 1800;
//const int windowHeight = 1000;
int res;
int N;
int M;

int** grid_NewGen;
Grid* grid_PrevGen;

static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

int rand_int(int lower, int upper) {
	return (rand() % (upper - lower + 1)) + lower;
}
	
int getState(int i, int j, Grid* points) {
	int state = 0;
	int ii;
	int jj;
	//double time1 = get_wall_seconds();
	for (int ki = -1; ki < 2; ki++) {
		for (int kj = -1; kj < 2; kj++) {
			ii = (i + ki + N) % N;
			jj = (j + kj + M) % M;
			state += points->board[ii][jj];
		}
	}
	
	state -= points->board[i][j]; // subtracting own cell
	//printf("Took %7.3f Wall seconds \n",get_wall_seconds() - time1);
	return state;
}

void newGeneration(Grid* prevGen,int** nextGen) {
	int state = 0;
	int curr = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			state = getState(i,j,prevGen);
			//printf("Current state: %d \n",state);
			curr = prevGen->board[i][j];
			
			// Generates the new generation according to the rules
			// Dies by loneliness
			if (curr && state < 2)
				nextGen[i][j] = 0;
			
			// Dies by overpopulation
			else if (curr && state > 3)
				nextGen[i][j] = 0;
				
			// Rebirth
			else if (!curr && state == 3) 
				nextGen[i][j] = 1;
				
			// Nothing happens
			else
				nextGen[i][j] = curr;				
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			prevGen->board[i][j] = nextGen[i][j];
		}
	}

}

int main(int argc, char* argv[]) {
if (argc != 2) {
	fprintf(stderr,"ERROR YOU DUMDUM\n");
	return 0;
}

Display *dpy = XOpenDisplay(0);

int snum = DefaultScreen(dpy);
int windowWidth = DisplayWidth(dpy, snum) - 100;
int windowHeight = DisplayHeight(dpy, snum) - 100; 

res = atoi(argv[1]);
N = windowWidth/res; // atoi(argv[1]);
M = windowHeight/res;
grid_PrevGen = (Grid*)malloc(sizeof(Grid));

grid_PrevGen->board = (int**)malloc(N*sizeof(int*));
grid_NewGen = (int**)malloc(N*sizeof(int*));

srand(time(0));
for (int i = 0; i < N; i++) {
	grid_PrevGen->board[i] = (int*)malloc(M*sizeof(int));
	grid_NewGen[i] = (int*)malloc(M*sizeof(int));
	for (int j = 0; j < M; j++) {
		if (i == 0 || i == N-1 || j == 0 || j == M-1) {
			grid_PrevGen->board[i][j] = 0;
		}
		else {
			grid_PrevGen->board[i][j] = rand_int(0,1);
		}
		grid_NewGen[i][j] = grid_PrevGen->board[i][j];
	  // printf("grid[%d,%d] = %d\n",i,j,grid_PrevGen->board[i][j]);
	}
}

InitializeGraphics(argv[0],windowWidth,windowHeight);
 SetCAxes(0,1);
while(!CheckForQuit()) {
ClearScreen();
	newGeneration(grid_PrevGen, grid_NewGen);
	for (int i = 0; i < N; i++) {
	for (int j = 0; j < M; j++) {
		if (grid_PrevGen->board[i][j] == 0) {
		}
		else
			DrawRectangle(i*res,j*res,res,res,1);	//printf("#");
	}
}
 Refresh();
usleep(50000);
//system("clear");
}
for(int i = 0; i < N; i++) {
	free(grid_PrevGen->board[i]);
	free(grid_NewGen[i]);
}
free(grid_PrevGen->board);
free(grid_NewGen);
free(grid_PrevGen);
  
  
FlushDisplay();
CloseDisplay();
return 0;}
