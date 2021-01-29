// Example program:
// Using SDL2 to create an application window
#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "SDL.h"
#include "defs.h"

int width = 640;
int height = 480;

int main(int argc, char* argv[]) {

    SDL_Window *window;                    // Declare a pointer

    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        width,                             // width, in pixels
        height,                            // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	int count = 100;
	SDL_Point points[count];
	struct Particle *p = calloc(count, sizeof(struct Particle));
	
	for (int i = 0; i < count; i++)
	{
		p[i].x = (i + 1) * 2;
		p[i].y = (i + 1);
		p[i].u = 0.5;
		p[i].m = 200.0;
	}
	
	POLY6 = 315.0 / (65.0 * PI * pow(H, 9));
	SPIKY_GRAD = -45.0 / (PI * pow(H, 6));
	VISC_LAP = 45.0 / (PI * pow(H, 6));
	
	while (1)
	{
		SDL_Event e;
		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				break;
			}
		}
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		
		for (int i = 0; i < count; i++)
		{
			p[i].x += p[i].u;
			p[i].y -= p[i].v;
			points[i].x = p[i].x;
			points[i].y = p[i].y;
		}
		
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawPoints(renderer, points, count);
		SDL_RenderPresent(renderer);
		
		Step(p, count);
	}

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
	
	free(p);

    // Destroys objects
	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
	
    // Clean up
    SDL_Quit();
    return 0;
}