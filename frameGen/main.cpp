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

void drawDoor(SDL_Renderer* renderer, SDL_Point center, int radius, SDL_Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w; // horizontal offset
      int dy = radius - h; // vertical offset
      if (center.y + dy >= center.y && (center.x + dx >= radius - center.x && center.x + dx < radius+center.x))
        SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
      else if (((dx*dx + dy*dy) <= (radius * radius))) {
        SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
      }
    }
  }
}
void drawSky(SDL_Renderer* renderer, int height, SDL_Color color){
  int red, green, blue, alpha;
  for (int h = 0; h <= height; h++){
    for (int w = 0; w < WIDTH; w++){
      red = color.r + 2*h/5;
      green = color.g + h/9;
      blue = color.b - h/3;
      alpha = color.a;
      SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
      SDL_RenderDrawPoint(renderer, w, h);
    }
  }

}
void drawTerrain(SDL_Renderer* renderer, int height, SDL_Color color){
	int lastHeight = height;
	bool peaked = false;
	bool plateau = false;
  SDL_Point center;
  int radius = 25;
	srand(time(NULL));
	for (int w = 0; w < WIDTH; w++){
		int h = lastHeight;
		if (!plateau && w > WIDTH/2 && w % 5 == 0){
			if (!peaked){
				h = lastHeight - (rand() % 7);
				if (h <= height - 100){
          peaked = true;
          center = {w + radius/2, height - radius - 1};
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
    int redChange, greenChange, blueChange;
		lastHeight = h;
		for (; h < HEIGHT; h++){
      if (h >= height){
        redChange = -1 * (2 * int(h-height)/3);
        greenChange = -1 * ((h-height)/2);
        blueChange = -1 * ((h-height)/6);
        SDL_SetRenderDrawColor(renderer, 136 + redChange, 170 + greenChange, 55 + blueChange, color.a);
      }
      else
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			SDL_RenderDrawPoint(renderer, w, h);
		}
	}
  drawDoor(renderer, center, radius,{11, 35, 14,255});
}

void writeName(SDL_Renderer* renderer) {
  TTF_Init();
  TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", 24);
  if (font == NULL) {
    throw std::string("error: font not found");
  }
  SDL_Color textColor = {76, 149, 178, 255};
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

    SDL_SetRenderDrawColor( renderer,24, 56, 127,255);
    SDL_RenderClear(renderer);

    drawSky(renderer, 300, background);
 		SDL_Point center = {320, 300};
 		int radius = 75;
 		SDL_Color color = {237,199,9,255};
		Sunray rays(renderer, center, radius, color);
		rays.drawRays(5);
		SDL_Color groundColor = {16,85,25,255};
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
