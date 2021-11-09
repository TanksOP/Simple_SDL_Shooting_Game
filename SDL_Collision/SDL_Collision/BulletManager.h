#include <SDL.h>
#include <SDL_image.h>

#include <vector>
#include <math.h>
#include <algorithm>

#include "Player.h"

#define PI 3.14159265

using namespace std;

class BulletManager{
public:
	BulletManager(SDL_Renderer* renderer, Player* player) : renderer(renderer), player(player) {};
	
	struct Bullet{
		float x, y, rotation, distance;
	};

	void init() {
		SDL_Surface* surface = IMG_Load("cache/lazer.png");
		this->bulletTexture = SDL_CreateTextureFromSurface(this->renderer, surface);
		SDL_FreeSurface(surface);
	}

	void processInput(bool* keyDown) {
		if (keyDown[SDL_SCANCODE_SPACE]) {
			if (SDL_GetTicks() - lastShot > shootTimerMS) {
				bullets.push_back(Bullet{ 315.0f, 230.0f, player->getAngle(),0.0f });
				lastShot = SDL_GetTicks();
			}
		}
	}


	void update() {
		for (auto& b : bullets) {
			b.x += sin(b.rotation * PI / 180.0f) * bulletVelocity;
			b.y -= cos(b.rotation * PI / 180.0f) * bulletVelocity;
			b.distance += bulletVelocity;
		}

		auto remove = remove_if(bullets.begin(), bullets.end(),
			[](const Bullet& b) {return b.distance > 1000; }); // if distace > 1000 remove the bullet
		bullets.erase(remove, bullets.end()); // removes etra spce still in the vector
	}

	void draw() {
		SDL_Point center = { 5,5 };
		for (auto& b : bullets) {
			SDL_Rect dest = { b.x, b.y, 10, 10 };
			SDL_RenderCopyEx(renderer, bulletTexture, 0, &dest, b.rotation, &center, SDL_FLIP_NONE);

		}
	}

	void clean() {
		SDL_DestroyTexture(this->bulletTexture);
	}

	

private:
	SDL_Renderer* renderer;
	SDL_Texture* bulletTexture;
	vector<Bullet> bullets;
	Player* player;

	const int shootTimerMS = 300;
	const int bulletVelocity = 15;
	unsigned int lastShot = 0;

};



