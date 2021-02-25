/*
 * Example of how to use graphics routines.
 *
 * Based on code by Oliver Fringer, Stanford University.
 * 
 * Modified by Elias Rudberg, Uppsala University.
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include "graphics.h"
#include <math.h>
#include <time.h>

const int N = 21;
const float circleRadius=0.025, circleColor=0;
const float rectWidth = circleRadius;
const int windowWidth=1392;
const int windowLength = 1856;
const float res = 10;
const int cols = (int)(windowWidth/res) + 1;
const int rows = (int)(windowLength/res) + 1;


float float_rand(float min, float max)
{
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}

void keep_within_box(float* xA, float* yA) {
  if(*xA > 1)
    *xA = 0;
  if(*yA > 1)
    *yA = 0;
}

float calcWeight(float x, float y, circle_t** circArray, int N) {
	float sum = 0;
	for (int i = 0; i < N; i++) {
		float sq = (x-circArray[i]->x)*(x-circArray[i]->x) + (y-circArray[i]->y)*(y-circArray[i]->y);
			sum += (circArray[i]->r)*(circArray[i]->r)/sq;
		}
		return sum;
		/*
	if (sum >= 1) {
		return 1;
	}
	else {
		return 0;
	} */
}

void bounce(circle_t* circ, float L, float W) {
		if (circ->x < circ->r && circ->vx < 0){
			circ->x = circ->r;
			circ->vx = -circ->vx; 
		}

		if (circ->y < circ->r && circ->vy < 0){		
			circ->y = circ->r;
			circ->vy = -circ->vy;
		}

		if (circ->x > L - circ->r && circ->vx > 0){		
			circ->x = L - circ->r;
			circ->vx = -circ->vx;
		}

		if (circ->y > W - circ->r && circ->vy > 0){		
			circ->y = W - circ->r;
			circ->vy = -circ->vy;
		}
}

int main(int argc, char *argv[]) {
  float L=1, W=1;
  point_t* points[cols][rows];
  point_t* a = (point_t*)malloc(sizeof(point_t));
  point_t* b = (point_t*)malloc(sizeof(point_t));
  point_t* c = (point_t*)malloc(sizeof(point_t));
  point_t* d = (point_t*)malloc(sizeof(point_t));
  circle_t** circArray = (circle_t**)malloc(N*sizeof(circle_t));
  //printf("Cols: %d, Rows: %d\n",cols, rows);
  srand(time(0));
  for (int i = 0; i < N; i++) {
  	circArray[i] = (circle_t*)malloc(sizeof(circle_t));
  	circArray[i]->x = float_rand(0,1);
	  circArray[i]->y = float_rand(0,1);
	  circArray[i]->r = float_rand(rectWidth,3*rectWidth);
	  circArray[i]->vx = float_rand(-0.001,0.001);//0.15;
	  circArray[i]->vy = float_rand(-0.001,0.001);// 0.07;
  } 
	
	//printf("x: %f, y: %f, vx: %f, vy: %f\n",circ->x,circ->y,circ->vx,circ->vy);
  
  InitializeGraphics(argv[0],windowLength,windowWidth);
  SetCAxes(0,1);

  printf("Hit q to quit.\n");
  while(!CheckForQuit()) {
    /* Move Circles. */
    for (int i = 0; i < N; i++) {
		  circArray[i]->x += circArray[i]->vx;
		  circArray[i]->y += circArray[i]->vy;
    	bounce(circArray[i],L,W);
    }
    
    /* Call graphics routines. */
    ClearScreen();

	for (int i=0; i < cols; i++) {
		for (int j=0; j < rows; j++) {
			points[i][j] = (point_t*)malloc(sizeof(point_t));
			points[i][j]->x = i*res;
			points[i][j]->y = j*res;
			points[i][j]->val = calcWeight(i*res/windowWidth,j*res/windowLength,circArray,N);
			//printf("points[%d][%d] = %d\n",i, j, points[i][j]);
		}
	}
   
	for (int i = 0; i < cols-1; i++) {
	  	for (int j = 0; j < rows-1; j++) {
	  		int state = getState(points[i][j]->val,points[i+1][j]->val,points[i+1][j+1]->val,points[i][j+1]->val);
	  		templateDrawInterpolate(state,points[i][j],points[i+1][j],points[i+1][j+1],points[i][j+1]);
	  	}
  	} 
  	/*
  	for(int i = 0; i < N; i++) {
  		DrawCircle(circArray[i]->x,circArray[i]->y,L,W,circArray[i]->r, circleColor);
  	}
  	*/
    Refresh();
    for (int i=0; i < cols; i++) {
		for (int j=0; j < rows; j++) {
			free(points[i][j]);
		}
	}
    /* Sleep a short while to avoid screen flickering. */
    usleep(500);
  }
  FlushDisplay();
  CloseDisplay();
  return 0;
}
