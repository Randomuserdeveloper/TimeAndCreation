#pragma once
#include "SDL_image.h"
#include "Entity.h"
#include "Math.h"
#include "SDL.h"

class BreakableEntity : public Entity {
public:
	BreakableEntity(Vector2f position, SDL_Texture* texture);
};

