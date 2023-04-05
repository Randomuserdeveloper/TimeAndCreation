#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "RenderWindow.h"
#include "Entity.h"
#include "Utilities.h"
#include <vector>
#include "BreakableEntity.h"

using namespace std;

enum BuildingTiles {
	WOODPLANK = 1,
	STONEBRICK = 2,
	GLASS = 3,
	GRASS = 4,
	DIRT = 5,
	STONE = 6
};

int main(int argc, char* args[]) {
	if (SDL_Init(SDL_INIT_VIDEO) > 0)
		cout << SDL_GetError();
	
	if (!(IMG_Init(IMG_INIT_PNG)))
		cout << SDL_GetError();

	const short windowWidth = 1280;
	const short windowHeight = 720;

	RenderWindow window{"Time and Creation", windowWidth, windowHeight };

	SDL_Texture* skyTexture = window.loadTexture("sky.png");
	SDL_Texture* grassTexture = window.loadTexture("grass.png");
	SDL_Texture* dirtTexture = window.loadTexture("dirt.png");
	SDL_Texture* stoneTexture = window.loadTexture("stone.png");
	SDL_Texture* stoneDirtTransistionTexture = window.loadTexture("stoneAndDirtTransition.png");
	SDL_Texture* woodPlankTexture = window.loadTexture("woodPlank.png");
	SDL_Texture * stoneBrickTexture = window.loadTexture("stoneBrick.png");
	SDL_Texture* glassTexture = window.loadTexture("glass.png");

	vector<Vector2f> buildEntityPositions;
	vector<Entity> entities;
	vector<BreakableEntity> breakableEntities;

	const float buildEntitySize = 16;
	const int layers = 20;
	const int rows = 12;

	for (float i = 0; i < layers * 2; i++) {
		for (float j = 0; j < rows * 2; j++) {
			buildEntityPositions.push_back(Vector2f{ i * (buildEntitySize * 2), j * (buildEntitySize * 2) });
		}
	}

	//for (float i = 0; i < layers; i++) {
	//	for (float e = 0; e < 7; e++) {
	//		Entity skyLayer{ Vector2f{i * 32, e * 32}, skyTexture };
	//		entities.push_back(skyLayer);
	//	}

	//	Entity grassLayer{Vector2f{i * 32, 200.0}, grassTexture};
	//	entities.push_back(grassLayer);
	//	Entity dirtLayer1{ Vector2f{i * 32, 232}, dirtTexture };
	//	entities.push_back(dirtLayer1);
	//	Entity dirtLayer2{ Vector2f{i * 32, 264}, dirtTexture };
	//	entities.push_back(dirtLayer2);
	//	Entity stoneAndDirtLayer{ Vector2f{i * 32, 296}, stoneDirtTransistionTexture };
	//	entities.push_back(stoneAndDirtLayer);
	//	Entity stoneLayer{ Vector2f{i * 32, 328}, stoneTexture };
	//	entities.push_back(stoneLayer);
	//}

	bool gameRunning = true;

	SDL_Event event;

	const float deltaTime = 0.01f;
	float accumulator = 0.0f;
	float currentTime = Utilities::timeInSeconds();
	unsigned short currentBuildingBlock = 0;

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

					if (SDL_KEYDOWN == event.type) {
						if (SDLK_1 == event.key.keysym.sym)
							currentBuildingBlock = BuildingTiles::WOODPLANK;

						else if (SDLK_2 == event.key.keysym.sym)
							currentBuildingBlock = BuildingTiles::STONEBRICK;

						else if (SDLK_3 == event.key.keysym.sym)
							currentBuildingBlock = BuildingTiles::GLASS;

						else if (SDLK_4 == event.key.keysym.sym)
							currentBuildingBlock = BuildingTiles::GRASS;

						else if (SDLK_5 == event.key.keysym.sym)
							currentBuildingBlock = BuildingTiles::DIRT;

						else if (SDLK_6 == event.key.keysym.sym)
							currentBuildingBlock = BuildingTiles::STONE;

						else if (SDLK_b == event.key.keysym.sym) {
							for (float i = 0; i < layers; i++) {
								for (float e = 0; e < rows; e++) {
									Entity skyLayer{ Vector2f{i * 32, e * 32}, skyTexture };
									entities.insert(entities.begin(), skyLayer);
								}
							}
						}
					}

					if (SDL_MOUSEBUTTONDOWN == event.type)
						if (SDL_BUTTON_LEFT == event.button.button) {
							int mouseX;
							int mouseY;
							SDL_GetMouseState(&mouseX, &mouseY);
							Vector2f mousePosition{ static_cast<float>(mouseX) - buildEntitySize, static_cast<float>(mouseY) - buildEntitySize };
							cout << "Mouse Location: " << "(" << static_cast<float>(mouseX) << ", " << static_cast<float>(mouseY) << ")" << endl;

							for (auto& location : buildEntityPositions) {
								if (location.getDistance(mousePosition) <= buildEntitySize) {
									BreakableEntity woodPlank{ Vector2f{location}, woodPlankTexture };
									BreakableEntity stoneBrick{ Vector2f{location}, stoneBrickTexture };
									BreakableEntity glass { Vector2f{ location }, glassTexture };
									BreakableEntity grass{ Vector2f{location}, grassTexture };
									BreakableEntity dirt{ Vector2f{location}, dirtTexture };
									BreakableEntity stone{ Vector2f{ location }, stoneTexture };

									switch (currentBuildingBlock) {
									case 1:
										breakableEntities.push_back(woodPlank);
										break;
									case 2:
										breakableEntities.push_back(stoneBrick);
										break;
									case 3:
										breakableEntities.push_back(glass);
										break;
									case 4:
										breakableEntities.push_back(grass);
										break;
									case 5:
										breakableEntities.push_back(dirt);
										break;
									case 6:
										breakableEntities.push_back(stone);
										break;
									}

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