#include<stdbool.h>
#include<SDL2/SDL.h>
#include<stdint.h>
#include<stdio.h>
#include"Doge_SDL.h"

#define CELLLEN 8
#define GRIDXMAX (XMAX / CELLLEN)
#define GRIDYMAX (YMAX / CELLLEN)

bool grid[GRIDXMAX][GRIDYMAX];
bool nextGen[GRIDXMAX][GRIDYMAX];
uint delayTime = 100;
bool pause = false;
bool run = true;

typedef struct{
	uint x, y;
	bool left, right, middle;
}Mouse;

Mouse mouse = {};
Mouse lastMouse = {};

void drawCell(uint x, uint y)
{
	setColor(0, 0, 0);
	fillRect(x*CELLLEN, y*CELLLEN,
		x*CELLLEN+CELLLEN, y*CELLLEN+CELLLEN);
	if(grid[x][y]){
		setColor(255, 0, 0);
		fillRect(x*CELLLEN+1, y*CELLLEN+1,
			x*CELLLEN+CELLLEN-1, y*CELLLEN+CELLLEN-1);
	}
}

void drawGrid()
{
	for(uint y = 0; y < GRIDYMAX; y++) {
		for(uint x = 0; x < GRIDXMAX; x++) {
			drawCell(x, y);
		}
	}
	drawFrame();
}

void fillGrid(bool state)
{
	for(uint y = 0; y < GRIDYMAX; y++) {
		for(uint x = 0; x < GRIDXMAX; x++) {
			grid[x][y] = state;
		}
	}
}

uint wrapX(int x)
{
	if(x == -1){
		return GRIDXMAX-1;
	}
	else if(x == GRIDXMAX){
		return 0;
	}
	return x;
}

uint wrapY(int y)
{
	if(y == -1){
		return GRIDYMAX-1;
	}
	else if(y == GRIDYMAX){
		return 0;
	}
	return y;
}

uint getNeighbors(uint x, uint y)
{
	uint liveNeighbors = 0;
	// C = check,

	/*	CCC
			---
			---		*/
	liveNeighbors += grid[wrapX(x-1)]	[wrapY(y-1)];
	liveNeighbors += grid[x]					[wrapY(y-1)];
	liveNeighbors += grid[wrapX(x+1)]	[wrapY(y-1)];

	/*	---
			C-C
			---		*/
	liveNeighbors += grid[wrapX(x-1)]	[y];
	liveNeighbors += grid[wrapX(x+1)]	[y];

	/*	---
			---
			CCC		*/
	liveNeighbors += grid[wrapX(x-1)]	[wrapY(y+1)];
	liveNeighbors += grid[x]					[wrapY(y+1)];
	liveNeighbors += grid[wrapX(x+1)]	[wrapY(y+1)];

	return liveNeighbors;
}

void nextStep()
{
	for(int y = 0; y < GRIDYMAX; y++) {
		for(int x = 0; x < GRIDXMAX; x++) {
			uint neighbors = getNeighbors(x, y);
			if(grid[x][y]){
				nextGen[x][y] = neighbors == 2 || neighbors == 3;
			}
			else{
				nextGen[x][y] = neighbors == 3;
			}
		}
	}
	for(uint y = 0; y < GRIDYMAX; y++) {
		for(uint x = 0; x < GRIDXMAX; x++) {
			grid[x][y] = nextGen[x][y];
		}
	}
	drawGrid();
}

void randomize()
{
	for(uint y = 0; y < GRIDYMAX; y++) {
		for(uint x = 0; x < GRIDXMAX; x++) {
			grid[x][y] = rand() & 1;
		}
	}
}

int main()
{
	init();
	setColor(0, 0, 0);
	fillRect(0, 0, XMAX, YMAX);
	fillGrid(true);
	drawGrid();
	randomize();
	while(run){
		while(pause){
			events();
		}
		events();
		nextStep();
		delay(delayTime);
	}

	// Destroy renderer
	SDL_DestroyRenderer(renderer);
	// Destroy window
	SDL_DestroyWindow(window);
	// Quit SDL subsystems
	SDL_Quit();

	return 0;
}
