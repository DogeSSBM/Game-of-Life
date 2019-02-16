#include<stdbool.h>
#include<SDL2/SDL.h>
#include<stdint.h>
#include<stdio.h>
#include"Doge_SDL.h"

#define NEIGHBORHOOD 1 // number of cells away from current cell to check
#define CELLLEN 5
#define GRIDX WINDX / CELLLEN
#define GRIDY WINDY / CELLLEN

bool grid[GRIDX][GRIDY];
bool nextGen[GRIDX][GRIDY];

typedef struct{
	uint x, y;
	bool left, right, middle;
}Mouse;

Mouse mouse = {};
Mouse lastMouse = {};

void initGrid()
{
	for(uint y = 0; y < GRIDY; y++){
		for(uint x = 0; x < GRIDX; x++){
			grid[x][y] = rand() & 1;
		}
	}
}

void drawCell(uint x, uint y)
{
	uint xpos = x*CELLLEN;
	uint ypos = y*CELLLEN;

	setColor(10, 10, 10);
	fillRect(xpos, ypos, CELLLEN, CELLLEN);

	if(grid[x][y]){
		setColor(255, 25, 55);
	}
	else{
		setColor(0, 0, 0);
	}

	fillRect(xpos+1, ypos+1, CELLLEN-2, CELLLEN-2);
}

void drawGrid()
{
	for(uint y = 0; y < GRIDY; y++){
		for(uint x = 0; x < GRIDX; x++){
			drawCell(x, y);
		}
	}
}

uint wrapX(int x)
{
	if(x >= GRIDX){
		return (uint)x % GRIDX;
	}
	else if(x < 0){
		return (uint)x + GRIDX;
	}
	return (uint)x;
}

uint wrapY(int y)
{
	if(y >= GRIDY){
		return (uint)y % GRIDY;
	}
	else if(y < 0){
		return (uint)y + GRIDY;
	}
	return (uint)y;
}

uint getNeighbors(uint x, uint y)
{
	uint neighbors=0;
	for(int xoffset = 0-NEIGHBORHOOD; xoffset <= NEIGHBORHOOD; xoffset++){
	for(int yoffset = 0-NEIGHBORHOOD; yoffset <= NEIGHBORHOOD; yoffset++){
		neighbors+=grid[wrapX(x+xoffset)][wrapY(y+yoffset)];
	}
	}
	return neighbors-grid[x][y];
}

void iterate()
{
	for(uint y = 0; y < GRIDY; y++){
		for(uint x = 0; x < GRIDX; x++){
			uint neighbors = getNeighbors(x, y);
			if(neighbors > 3 || neighbors < 2){
				nextGen[x][y] = 0;
			}
			else{
				nextGen[x][y] = grid[x][y] || neighbors==3;
			}
		}
	}
}

void advanceGen()
{
	for(uint y = 0; y < GRIDY; y++){
		for(uint x = 0; x < GRIDX; x++){
			grid[x][y]=nextGen[x][y];
		}
	}
}

int main()
{
	init();
	initGrid();
	while(1){
		drawGrid();
		drawFrame();
		do{
			events();
		}while(pause);
		iterate();
		advanceGen();
		delay(delayTime);
	}

	return 0;
}
