#include "Doge_SDL.h"

uint celllen = 8;
uint gridx;
uint gridy;

bool **grid = NULL;
bool **nextGen = NULL;

typedef struct{
	uint x, y;
	bool left, right, middle;
}Mouse;

Mouse mouse = {};
Mouse lastMouse = {};

void randomizeGrid()
{
	for(uint y = 0; y < gridy; y++){
		for(uint x = 0; x < gridx; x++){
			grid[x][y] = (bool)(rand() & 1);
		}
	}
}

// returns true and sets celllen if valid, otherwise returns false
bool changeCelllen(int change)
{
	uint newLen = celllen + change;
	if(newLen < WINDXMID && newLen < WINDYMID && newLen > 2){
		celllen = newLen;

		return true;
	}
	else{
		return false;
	}
}

// returns true and sets window lengths if valid, otherwise returns false
bool changeWindlen(int change)
{

}

void initGrid()
{
	if(grid != NULL){
		free(grid);
	}
	if(nextGen != NULL){
		free(nextGen);
	}
	gridx = WINDX / celllen;
	gridy = WINDY / celllen;
	grid = malloc(sizeof(bool*) * gridx);
	nextGen = malloc(sizeof(bool*) * gridx);
	for(uint x = 0; x < gridx; x++){
		grid[x] = malloc(sizeof(bool) * gridy);
		nextGen[x] = malloc(sizeof(bool) * gridy);
	}
	randomizeGrid();
}

void handleKey()
{
	switch (event.key.keysym.sym){
		case SDLK_UP:
			changeCelllen(1);
			initGrid();
			break;
		case SDLK_DOWN:
			changeCelllen(-1);
			initGrid();
			break;
		case SDLK_RIGHT:
			if(delayTime > 1000){
				delayTime = 1000;
			}
			else if(delayTime > 100){
				delayTime -= 100;
			}
			else if(delayTime > 10){
				delayTime -= 10;
			}
			else if(delayTime > 0){
				delayTime--;
			}
			break;
		case SDLK_LEFT:
			if(delayTime > 1000){
				delayTime = 1000;
			}
			else if(delayTime > 100){
				delayTime += 100;
			}
			else if(delayTime > 10){
				delayTime += 10;
			}
			else if(delayTime >= 0){
				delayTime++;
			}
			break;
		case SDLK_q:
			quit();
			break;
		case SDLK_r:
			randomizeGrid();
			break;
		case SDLK_SPACE:
			pause = !pause;
			break;
		default:
			printf("Unknown Key event!\n");
			break;
	}
}

void drawCell(uint x, uint y)
{
	uint xpos = x*celllen;
	uint ypos = y*celllen;

	setColor(10, 10, 10);
	fillRect(xpos, ypos, celllen, celllen);

	if(grid[x][y]){
		setColor(255, 25, 55);
	}
	else{
		setColor(0, 0, 0);
	}

	fillRect(xpos+1, ypos+1, celllen-2, celllen-2);
}

void drawGrid()
{
	for(uint y = 0; y < gridy; y++){
		for(uint x = 0; x < gridx; x++){
			drawCell(x, y);
		}
	}
}

uint wrapX(int x)
{
	if(x<0){
		return gridx-1;
	}
	else if(x>=gridx){
		return 0;
	}
	return x;
}

uint wrapY(int y)
{
	if(y<0){
		return gridy-1;
	}
	else if(y>=gridy){
		return 0;
	}
	return y;
}

uint getNeighbors(uint x, uint y)
{
	uint neighbors=0;
	for(int xoffset = -1; xoffset < 2; xoffset++){
	for(int yoffset = -1; yoffset < 2; yoffset++){
		neighbors+=grid[wrapX(x+xoffset)][wrapY(y+yoffset)];
	}
	}
	return neighbors-grid[x][y];
}

void iterate()
{
	setColor(0, 0, 0);
	fillScreen();
	for(uint y = 0; y < gridy; y++){
		for(uint x = 0; x < gridx; x++){
			uint neighbors = getNeighbors(x, y);
			if(neighbors > 3 || neighbors < 2){
				nextGen[x][y] = 0;
			}
			else{
				nextGen[x][y] = grid[x][y] || neighbors==3;
			}
		}
	}

	bool **temp = grid;
	grid = nextGen;
	nextGen = temp;

	//for(uint x = 0; x < gridx; x++){
	//	memcpy(grid[x], nextGen[x], sizeof(bool) * gridy);
	//}
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
		delay(delayTime);
	}

	return 0;
}
