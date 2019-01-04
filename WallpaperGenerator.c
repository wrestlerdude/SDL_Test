//Only tested with GCC
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#define ANSI_COLOR_GREEN "\x1b[32m"

void process_arguments (const char** argv, int* x)
{
	char *end;
	for (int i = 0; i < 3; ++i)
		x[i] = (int)strtol(argv[i+1], &end, 10);

	if (!x[0] || !x[1] || !x[2])
	{
		printf("Invalid values\n");
		exit(0);
	}
}

void wallpaper(uint32_t* pixels, int* dimension, double side)
{
	for (int i = 0; i < dimension[0]; ++i)
		for (int j = 0; j < dimension[1]; ++j)
		{
			double x = i*side/dimension[0];
			double y = j*side/dimension[1];
			int c = (int)trunc(x*x + y*y);

		#ifdef THREE
			if (c % 3 == 0)
				pixels[j*dimension[0] + i] = 0xff0000;
			else
		#endif
			if (c % 2 == 0)
				pixels[j*dimension[0] + i] = 127;
		
		}
}

int main(const int argc, const char **argv)
{
	if (argc != 4)
	{
		printf(ANSI_COLOR_GREEN "wallpaper1 <width> <height> <side_wallpaper>\n");
		return 0;
	}

	int *x = malloc(3*sizeof(int)); 
	process_arguments(argv, x);

	uint32_t *pixels = malloc(x[0]*x[1]*sizeof(uint32_t));
	memset(pixels, 0x800080, x[0]*x[1]*sizeof(uint32_t));
	wallpaper(pixels, x, (double)x[2]);

	char quit = 0;
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("The Turing Omnibus", SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, x[0], x[1], 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STATIC, x[0], x[1]);

	while (!quit)
	{
		SDL_UpdateTexture(texture, NULL, pixels, x[0]*sizeof(uint32_t));
		SDL_WaitEvent(&event);

		switch (event.type)
		{
			case SDL_QUIT:
			quit = 1;
			break;
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}


	free(x);
	free(pixels);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}