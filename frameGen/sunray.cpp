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
		drawPartCircle(tan(angle), colorChoice);
  }
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
  drawCircle();
}

void Sunray::drawCircle() {
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
  int radMult = 5;
  float rad = radius * radMult;
  for (int w = 0; w < rad * 2 ; w++) {
    for (int h = 0; h < rad; h++) {	// to draw only one half of a circle
      int dx = rad - w; // horizontal offset
      int dy = h - rad; // reversed vertical offset
      float cSquared = (dx*dx) + (dy*dy);
      float distToCenter = sqrt(cSquared);
      if (((dx*dx + dy*dy) <= (rad * rad)) && ((dy <= -(slope*float(dx))) && (dy <= (slope*float(dx))))) {
        int red, green, blue, alpha;
        if (isEqual(color, background)){
          red = color.r + 2*(center.y + (dy))/5;
          green = color.g + (center.y + (dy))/9;
          blue = color.b - ((center.y + (dy))/3);
          alpha = color.a;
        }
        else{
          red = color.r;
          green = color.g;
          blue = color.b;
          alpha = color.a * ((rad - distToCenter)/rad);
        }
      	SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
        SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
      }
    }
  }
}

bool Sunray::isEqual(SDL_Color color1, SDL_Color color2){
  if (color1.r != color2.r)
    return false;
  else if (color1.g != color2.g)
    return false;
  else if (color1.b != color2.b)
    return false;
  else if (color1.a != color2.a)
      return false;
  return true;
}
