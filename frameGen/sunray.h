#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <cmath>

const SDL_Color background = {252, 151, 63, 255};

class Sunray {
	public:
		Sunray(SDL_Renderer* renderer, SDL_Point center, int radius, SDL_Color color) {
			this->renderer = renderer;
			this->center = {center.x, center.y};
			this->radius = radius;
			this->color = {color.r, color.g, color.b, color.a};
		}
		
		Sunray(const Sunray&){
			
		}
		
		~Sunray() {
		}
	void drawRays(int numOfRays);
	private:
		void drawCircle();
		void drawPartCircle(float slope, SDL_Color color);
		SDL_Renderer *renderer;// = new SDL_Renderer;
		SDL_Point center;
		SDL_Color color;
		int radius;
};
