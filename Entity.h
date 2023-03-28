#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Math.h"

class Entity {
public:
	Entity(Vector2f position, SDL_Texture* texture);

	void setPosition(Vector2f& position);
	Vector2f& getPosition();
	SDL_Texture* getTexture() const;
	SDL_Rect getCurrentFrame() const;
	void setTexture(SDL_Texture* texture);
private:
	Vector2f position;
	SDL_Rect currentFrame;
	SDL_Texture* texture;
};

