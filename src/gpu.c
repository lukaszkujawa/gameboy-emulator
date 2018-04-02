#include <SDL.h>
#include <string.h>

#include "gpu.h"
#include "cpu.h"

gpu GPU;

SDL_Window *win;
SDL_Renderer *ren;

void gpu_init() {
	memset(&GPU, 0, sizeof(gpu));
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		printf( "SDL_Init Error: %s\n", SDL_GetError());
		exit(-1);
	}

	win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == NULL){
		printf( "SDL_CreateWindow Error: %s\n", SDL_GetError());
		SDL_Quit();
		exit(-1);
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	/*
	
	SDL_Surface *bmp = SDL_LoadBMP("../marbles.bmp");
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
	SDL_FreeSurface(bmp);
	
	SDL_Event e;
	int quit = 1;
	int xx = 0;

	while(quit){
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT){
				quit = 0;
			}
			if (e.type == SDL_KEYDOWN){
				quit = 0;
			}
			if (e.type == SDL_MOUSEBUTTONDOWN){
				quit = 0;
			}
		}



		//First clear the renderer
		SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);

        for(int x = 0 ; x < 160 ; x++) {
        	for(int y = 0 ; y < 140 ; y++) {
        		if(x%2==0 && y%2==1) {
        			SDL_RenderDrawPoint(ren, x*2, y*2);
        			SDL_RenderDrawPoint(ren, x*2+1, y*2);
        			SDL_RenderDrawPoint(ren, x*2, y*2+1);
        			SDL_RenderDrawPoint(ren, x*2+1, y*2+1);
        		}
        	}
        }

		
		//SDL_RenderCopy(ren, tex, NULL, NULL);
		//Update the screen
		SDL_RenderPresent(ren);
		xx++;

		//Take a quick break after all that hard work
		SDL_Delay(100);
		SDL_DestroyTexture(tex);
	}
	*/
	

}


void gpu_step() {
	GPU.mode_clock += CPU.clock_t;
	
	switch(GPU.mode) {
		// Hblank - after the last hblank, push the screen data to canvas
		case 0:
			if(GPU.mode_clock >= 204) {
			    GPU.mode_clock = 0;
			    GPU.line++;

			    if(GPU.line == 143) {
			        // Enter vblank
					GPU.mode = 1;
					//GPU._canvas.putImageData(GPU._scrn, 0, 0);
			    }
			    else {
			    	GPU.mode = 2;
			    }
			}
			break;

		// Vblank (10 lines)
		case 1:
			if(GPU.mode_clock >= 456) {
			    GPU.mode_clock = 0;
			    GPU.line++;

			    if(GPU.line > 153) {
			        // Restart scanning modes
					GPU.mode = 2;
					GPU.line = 0;
			    }
			}
			break;

		// OAM read mode, scanline active
	    case 2:
	        if(GPU.mode_clock >= 80) {
			    // Enter scanline mode 3
			    GPU.mode_clock = 0;
			    GPU.mode = 3;
			}
			break;

		// VRAM read mode, scanline active
	    // Treat end of mode 3 as end of scanline
	    case 3:
	        if(GPU.mode_clock >= 172) {
			    // Enter hblank
			    GPU.mode_clock = 0;
			    GPU.mode = 0;

			    //GPU.renderscan();
			}
			break;
	}
}

void gpu_quit() {
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}