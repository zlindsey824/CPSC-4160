#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "frameGenerator.h"
#include "sunray.h"

const std::string TITLE = "Zeandre Lindsey's Sunrise";
const std::string NAME = "zlindse";

const int WIDTH = 640;
const int HEIGHT = 480;

//const SDL_Color background = {200, 200, 255, 255};


void drawTriangle(SDL_Renderer* renderer,
  SDL_Point center, int width, int height, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int h = 0; h < abs(height); h++){
    	int calcWidth = (h/((width > abs(height))?(width/abs(height)):(abs(height)/width)));
      for (int w = 0; w < calcWidth ; w++){
        int dx = center.x - w;
        int dy = 0;
        if (height < 0)
          dy = center.y - h;
        else
          dy = center.y + h;
        SDL_RenderDrawPoint(renderer, dx, dy);
        dx = center.x + w;
        SDL_RenderDrawPoint(renderer, dx, dy);
      }
    }
}

void drawTerrain(SDL_Renderer* renderer, int height, SDL_Color color){
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	int lastHeight = height;
	bool peaked = false;
	bool plateau = false;
	srand(time(NULL));
	for (int w = 0; w < WIDTH; w++){
		int h = lastHeight;
		if (!plateau && w > WIDTH/2 && w % 5 == 0){
			if (!peaked){
				h = lastHeight - (rand() % 7);
				if (h <= height - 100){
					peaked = true;
					h = height - 100;
				}
				
			}
			else {
				h = lastHeight + (rand() % 2);
				if (h >= height){
					plateau = true;
					h = height;
				}
			}
		}
		
		lastHeight = h;
		for (; h < HEIGHT; h++){
			SDL_RenderDrawPoint(renderer, w, h);
		}
	}
}

void drawSword(){
	
}

void writeName(SDL_Renderer* renderer) {
  TTF_Init();
  TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", 24);
  if (font == NULL) {
    throw std::string("error: font not found");
  }
  SDL_Color textColor = {0xff, 0, 0, 0};
  SDL_Surface* surface =
    TTF_RenderText_Solid(font, TITLE.c_str(), textColor);
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

  int textWidth = surface->w;
  int textHeight = surface->h;
  SDL_FreeSurface(surface);
  SDL_Rect dst = {20, HEIGHT-40, textWidth, textHeight};

  SDL_RenderCopy(renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}


int main(void) {
  try {
    if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) {
      std::cout<< "Failed to initialize SDL2" << SDL_GetError() <<std::endl;
      return EXIT_FAILURE;
    }
    SDL_Window* window = SDL_CreateWindow(
      TITLE.c_str(),
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WIDTH,
      HEIGHT,
      SDL_WINDOW_SHOWN
    );
    // Apparently we can't use hardware acceleration with
    // SDL_GetWindowSurface
    SDL_Renderer* renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_SOFTWARE
    );
    SDL_SetRenderDrawBlendMode(renderer,
                               SDL_BLENDMODE_BLEND);

		//SDL_Color background = {200, 200, 255, 255}

    SDL_SetRenderDrawColor( renderer, background.r, background.g, background.b, background.a);
    SDL_RenderClear(renderer);
    
 		SDL_Point center = {320, 300};
 		int radius = 75;
 		SDL_Color color = {237,199,9,255};
		Sunray rays(renderer, center, radius, color);
		rays.drawRays(5);
		SDL_Color groundColor = {97,204,95,255};	// to {50,112,49, 255}
		drawTerrain(renderer, 300, groundColor);


    writeName(renderer);
    SDL_RenderPresent(renderer);
    FrameGenerator frameGen(renderer, window, WIDTH, HEIGHT, NAME);
    frameGen.makeFrame();

    SDL_Event event;
    const Uint8* keystate;
    while ( true ) {
      keystate = SDL_GetKeyboardState(0);
      if (keystate[SDL_SCANCODE_ESCAPE]) { break; }
      if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          break;
        }
      }
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  }
  catch (const std::string& msg) { std::cout << msg << std::endl; }
  catch (...) {
    std::cout << "Oops, someone threw an exception!" << std::endl;
  }
  return EXIT_SUCCESS;
}
