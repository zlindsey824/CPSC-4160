#include <iostream>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <cmath>
#include "sunray.h"

void Sunray::drawRays(int numOfRays) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  float baseAngle = (90/numOfRays) *M_PI/180;
  float angle;
  drawCircle();
  SDL_Color colorChoice;
  for (int n = 0; n < numOfRays; n++){
  	if (n % 2 == 0){
  		colorChoice = color;
		}
		else{
  		colorChoice = background;
		}
  		
		angle = baseAngle*n;
		std::cout << angle << "			" << tanh(angle) << std::endl;
		drawPartCircle(tan(angle), colorChoice);
  }
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
  drawCircle();
}

void Sunray::drawCircle() {
  //SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  for (int w = 0; w < radius * 2; w++) {
    for (int h = 0; h < radius * 2; h++) {
      int dx = radius - w; // horizontal offset
      int dy = radius - h; // vertical offset
      if (((dx*dx + dy*dy) <= (radius * radius))) {
        SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
      }
    }
  }
}

void Sunray::drawPartCircle(float slope, SDL_Color color) {
  //SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  
  int radMult = 5;
  float rad = radius * radMult;
  for (int w = 0; w < rad * 2 ; w++) {
    for (int h = 0; h < rad; h++) {	// to draw only one half of a circle
      int dx = rad - w; // horizontal offset
      int dy = h - rad; // reversed vertical offset
      float cSquared = (dx*dx) + (dy*dy);
      float distToCenter = sqrt(cSquared);
      if (((dx*dx + dy*dy) <= (rad * rad)) && ((dy <= -(slope*float(dx))) && (dy <= (slope*float(dx))))) {
      	int alpha = color.a * ((rad - distToCenter)/rad);
      	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);
        SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
      }
    }
  }
}
