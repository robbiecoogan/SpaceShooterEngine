#pragma once
#include <vector>
#include <string>
#include "SDL.h"
#include "SDL_TTF.h"
#include "Common.h"

class UIScrollbar
{
public:
	UIScrollbar(int inputX, int inputY, int inputWidth, int inputHeight, SDL_Renderer* gameRenderer);
	~UIScrollbar();

	void Update(SDL_Event* e);

	void Draw();

	int GetValue() { return value; };


private:
	int x, y, width, height;

	SDL_Rect backRect;//Back rectangle of the bar
	SDL_Rect notchRect;//rectangle for the notch of the bar

	SDL_Color backColor;//back color of the bar
	SDL_Color notchColor;//color of the drag part of the scrollbar
	SDL_Color outlineColor;//outline color of the bar

	SDL_Renderer* renderer;

	int value;
	int minVal;
	int maxVal;

	bool hasBeenClicked;
};