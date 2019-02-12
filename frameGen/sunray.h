#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <cmath>

const SDL_Color background = {90, 120, 161, 255};

class Sunray {

public:
	Sunray(SDL_Renderer* rend, SDL_Point c, int rad, SDL_Color col):
  renderer(rend), center(c), radius(rad), color(col){}

	Sunray(const Sunray& s ):
  renderer(s.renderer), center(s.center), radius(s.radius), color(s.color){

	}

  Sunray& operator=(const Sunray& s){
    this->renderer = s.renderer;
    this->center = {s.center.x, s.center.y};
    this->color = {s.color.r, s.color.g, s.color.b, s.color.a};
    this->radius = s.radius;
    return *this;
  }

	~Sunray() {}

	void drawRays(int numOfRays);
private:
  void drawCircle();
  void drawPartCircle(float slope, SDL_Color color);
  bool isEqual(SDL_Color color1, SDL_Color color2);
  SDL_Renderer *renderer;
  SDL_Point center;
  int radius;
  SDL_Color color;

};
