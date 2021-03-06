#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <SDL2/SDL.h>

// window size macros
#define WINDX 400
#define WINDY 400
#define WINDXMID (WINDX / 2)
#define WINDYMID (WINDY / 2)

typedef unsigned int uint;
typedef uint8_t u8;

uint delayTime = 100;
bool pause = false;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;

void randomizeGrid();
void windowSizeUp();
void windowSizedown();
void handleKey();

void quit()
{
	// Destroy renderer
	SDL_DestroyRenderer(renderer);
	// Destroy window
	SDL_DestroyWindow(window);
	// Quit SDL subsystems
	SDL_Quit();
	exit(0);
}


void events()
{
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				printf("Quitting now!\n");
				quit();
				break;
			case SDL_MOUSEMOTION:
				printf("Mouse position (%d, %d)\n", event.motion.x, event.motion.y);
				//handleMouse(event.motion.x, event.motion.y, event.motion.state);
				break;
			case SDL_MOUSEBUTTONDOWN:
				printf("Mouse click at (%d, %d)\n", event.button.x, event.button.y);
				//handleMouse(event.button.x, event.button.y, event.button.);
				break;
			case SDL_MOUSEBUTTONUP:
				printf("Mouse click at (%d, %d)\n", event.button.x, event.button.y);
				//handleMouse(event.button.x, event.button.y, event.button.);
				break;
			case SDL_KEYDOWN:
				printf("Key pressed\n");
				handleKey();
				break;
			/*case SDL_KEYUP:
				printf("Key released\n");
				handleKey();
				break;*/
			default:
				printf("Unknown event!\n");
				break;
		}
	}
}

void delay(uint ms)
{
	SDL_Delay(ms);
}

void setColor(u8 r, u8 g, u8 b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}

void drawPixel(uint x, uint y)
{
	SDL_RenderDrawPoint(renderer, x, y);
}

void drawLine(uint x1, uint y1, uint x2, uint y2)
{
	SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void drawRect(uint x, uint y, uint xlen, uint ylen)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = xlen;
	rect.h = ylen;
	SDL_RenderDrawRect(renderer, &rect);
}

void fillRect(uint x, uint y, uint xlen, uint ylen)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = xlen;
	rect.h = ylen;
	SDL_RenderFillRect(renderer, &rect);
}

void fillScreen()
{
	SDL_RenderClear(renderer);
}

void drawFrame()
{
	SDL_RenderPresent(renderer);
}

void init()
{
	srand(time(NULL));
	if(SDL_Init(SDL_INIT_VIDEO)<0){
		printf("SDL borked! Error: %s\n", SDL_GetError());
		// Destroy renderer
		SDL_DestroyRenderer(renderer);
		// Destroy window
		SDL_DestroyWindow( window );
		// Quit SDL subsystems
		SDL_Quit();
	}
	else{
		//Create window
		SDL_CreateWindowAndRenderer(WINDX, WINDY, 0,
			&window, &renderer);
		setColor(0, 0, 0);
		drawRect(0, 0, WINDX, WINDY);
		drawFrame();
	}
}
