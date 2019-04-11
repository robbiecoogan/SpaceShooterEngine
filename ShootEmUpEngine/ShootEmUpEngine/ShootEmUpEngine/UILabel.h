#pragma once

#include "SDL.h"
#include "SDL_TTF.h"
#include <string>
#include <iostream>

class UILabel
{

public:
	UILabel(int inputX, int inputY, std::string inputText, SDL_Renderer* gameRenderer, int inputAlignment, TTF_Font* font);
	~UILabel();

	void Update(SDL_Event* e);

	void Draw();

	int x, y;
	int width;
	int height;

private:
	
	std::string text;

	SDL_Renderer* renderer;

	SDL_Color textColor;

	TTF_Font* defaultFont;

	SDL_Surface* drawText;
	SDL_Texture* textTex;

	int alignment;
};