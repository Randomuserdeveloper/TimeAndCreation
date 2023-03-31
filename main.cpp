#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "RenderWindow.h"
#include "Entity.h"
#include "Utilities.h"
#include <vector>
#include "BreakableEntity.h"

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
	SDL_Texture* woodPlankTexture = window.loadTexture("woodPlank.png");

	vector<Vector2f> buildEntityPositions;
	vector<Entity> entities;
	vector<BreakableEntity> breakableEntities;

	const float buildEntitySize = 16; // Build Entities (Entities the player builds) are twice as small as regular entities
	const int layers = 20;
	const int rows = 12;

	for (float i = 0; i < layers * 4; i++) {
		for (float j = 0; j < rows * 4; j++) {
			buildEntityPositions.push_back(Vector2f{ i * (buildEntitySize * 2), j * (buildEntitySize * 2) });
		}
	}

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

					if (SDL_MOUSEBUTTONDOWN == event.type)
						if (SDL_BUTTON_LEFT == event.button.button) {
							int mouseX;
							int mouseY;
							SDL_GetMouseState(&mouseX, &mouseY);
							Vector2f mousePosition{ static_cast<float>(mouseX) - buildEntitySize, static_cast<float>(mouseY) - buildEntitySize };
							cout << "Mouse Location: " << "(" << static_cast<float>(mouseX) << ", " << static_cast<float>(mouseY) << ")" << endl;

							for (auto& location : buildEntityPositions) {
								if (location.getDistance(mousePosition) <= buildEntitySize) {
									BreakableEntity woodPlank{ Vector2f{location }, woodPlankTexture };
									breakableEntities.push_back(woodPlank);
								}
							}
						}
				}

				accumulator -= deltaTime;
			}

			const float alpha = accumulator / deltaTime;

			window.clear();

			for (auto& entity: entities)
				window.render(entity);

			for (auto& breakableEntity : breakableEntities)
				window.render(breakableEntity, 1);

			window.display();

			int frameTicks = SDL_GetTicks() - startTicks;

			if (frameTicks < 1000 / window.getRefreshRate())
				SDL_Delay(1000 / window.getRefreshRate() - frameTicks);

			}

		SDL_Quit();
		return 0;
}