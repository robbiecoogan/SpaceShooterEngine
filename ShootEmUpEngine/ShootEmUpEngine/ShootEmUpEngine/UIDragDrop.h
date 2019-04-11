#pragma once

#include "SDL_image.h"
#include "SDL.h"
#include "Common.h"

class UIDragDrop
{
public:
	UIDragDrop(std::string inputName, int inputX, int inputY, int inputWidth, int inputHeight, std::string imageDir, SDL_Renderer* gameRenderer, SDL_Window* gameWindow);
	~UIDragDrop();

	void Update(SDL_Event* e, SDL_Rect gameScreenRect);

	int x, y, width, height;

	SelectionBoxItem* HasDropped();

	void Draw();

private:
	SDL_Renderer* renderer;

	SDL_Texture* myImage;
	SDL_Rect sourceRect;
	SDL_Rect destRect;

	SDL_Window* window;

	bool isDragged;
	int dragOffsetX;
	int dragOffsetY;
	bool hasDropped;

	std::string name;
};