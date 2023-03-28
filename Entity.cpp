#include "Entity.h"
#include <SDL.h>
#include <SDL_image.h>

Entity::Entity(Vector2f position, SDL_Texture* texture) : position{ position }, texture{ texture } {
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 32;
	currentFrame.h = 32;
}

void Entity::setPosition(Vector2f& position) {
	this->position = position;
}

Vector2f& Entity::getPosition() {
	return position;
}

SDL_Texture* Entity::getTexture() const {
	return texture;
}

SDL_Rect Entity::getCurrentFrame() const {
	return currentFrame;
}

void Entity::setTexture(SDL_Texture* texture) {
	this->texture = texture;
}
