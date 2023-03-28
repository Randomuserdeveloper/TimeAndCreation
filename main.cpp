#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "RenderWindow.h"
#include "Entity.h"
#include "Utilities.h"
#include <vector>

using namespace std;

int main(int argc, char* args[]) {
	if (SDL_Init(SDL_INIT_VIDEO) > 0)
		cout << SDL_GetError();
	
	if (!(IMG_Init(IMG_INIT_PNG)))
		cout << SDL_GetError();

	RenderWindow window{"Time and Creation", 1280, 720};

	SDL_Texture* skyTexture = window.loadTexture("sky.png");
	SDL_Texture* grassTexture = window.loadTexture("grass.png");
	SDL_Texture* dirtTexture = window.loadTexture("dirt.png");
	SDL_Texture* stoneTexture = window.loadTexture("stone.png");
	SDL_Texture* stoneDirtTransistionTexture = window.loadTexture("stoneAndDirtTransition.png");

	vector<Entity> entities;

	const int layers = 20;

	for (float i = 0; i < layers; i++) {
		for (float e = 0; e < 7; e++) {
			Entity skyLayer{ Vector2f{i * 32, e * 32}, skyTexture };
			entities.push_back(skyLayer);
		}

		Entity grassLayer{Vector2f{i * 32, 200.0}, grassTexture};
		entities.push_back(grassLayer);
		Entity dirtLayer1{ Vector2f{i * 32, 232}, dirtTexture };
		entities.push_back(dirtLayer1);
		Entity dirtLayer2{ Vector2f{i * 32, 264}, dirtTexture };
		entities.push_back(dirtLayer2);
		Entity stoneAndDirtLayer{ Vector2f{i * 32, 296}, stoneDirtTransistionTexture };
		entities.push_back(stoneAndDirtLayer);
		Entity stoneLayer{ Vector2f{i * 32, 328}, stoneTexture };
		entities.push_back(stoneLayer);
	}

	bool gameRunning = true;

	SDL_Event event;

	const float deltaTime = 0.01f;
	float accumulator = 0.0f;
	float currentTime = Utilities::timeInSeconds();
	const short playerSpeedMultiplier = 4;
	short playerFrame = 1;

		while (gameRunning) {
			int startTicks = SDL_GetTicks();

			float newTime = Utilities::timeInSeconds();
			float frameTime = newTime - currentTime;

			currentTime = newTime;

			accumulator += frameTime;

			while (accumulator >= deltaTime) {
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_QUIT)
						gameRunning = false;
				}

				accumulator -= deltaTime;
			}

			const float alpha = accumulator / deltaTime;

			window.clear();

			for (auto& entity: entities)
				window.render(entity);

			window.display();

			int frameTicks = SDL_GetTicks() - startTicks;

			if (frameTicks < 1000 / window.getRefreshRate())
				SDL_Delay(1000 / window.getRefreshRate() - frameTicks);

			}

		SDL_Quit();
		return 0;
}