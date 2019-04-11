#pragma once

#include "SDL.h"
#include "SDL_TTF.h"
#include <string>


class UIButton
{
public:
	UIButton(int inputX, int inputY, int inputWidth, int inputHeight, std::string inputText, SDL_Renderer * gameRenderer, TTF_Font* inputFont);
	~UIButton();

	bool CheckClick();

	void Update(SDL_Event* e);
	void Draw();

	int x, y, width, height;
	bool isClicked;

private:
	SDL_Renderer * renderer;

	SDL_Color outlineColor;
	SDL_Color notClickedColor;
	SDL_Color clickedColor;
	SDL_Color textColor;

	SDL_Rect buttonRect;

	TTF_Font* defaultFont;
	std::string text;

};