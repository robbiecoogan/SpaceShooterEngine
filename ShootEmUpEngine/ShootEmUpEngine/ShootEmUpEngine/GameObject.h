#pragma once

#include "SDL_image.h"
#include "SDL.h"
#include <string>
#include <iostream>
#include "enums.h"

class GameObject
{
public:
	GameObject(GameObjectType inputType, int inputX, int inputY, std::string imageDir, SDL_Window* gameWindow, SDL_Renderer* gameRenderer);
	~GameObject();

	void Draw();

	GameObjectType type;


private:
	int x, y;
	SDL_Texture* texture;

	SDL_Renderer* renderer;
	SDL_Rect sourceRect;
	SDL_Rect destRect;

};
