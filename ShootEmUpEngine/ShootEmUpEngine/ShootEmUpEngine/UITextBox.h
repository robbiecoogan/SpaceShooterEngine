#pragma once

#include "SDL.h"
#include "SDL_TTF.h"
#include <string>

class UITextBox
{
	//This is an element of the UI. It is a field for entering data
public:
	UITextBox(int xPos, int yPos, int boxWidth, SDL_Renderer* gameRenderer, TTF_Font* font);
	UITextBox(int xPos, int yPos, int boxWidth, std::string inputText, SDL_Renderer* gameRenderer, TTF_Font* font);
	~UITextBox();

	void Init();

	std::string GetText() { return text; }
	void SetText(std::string input);

	void SetSelected(bool input);
	bool GetSelected() { return isSelected; }

	void Update(SDL_Event* e);

	void Draw();

	int x, y, width, height;

	void GetSize(int* inputW, int* inputH);


private:
	std::string text;

	SDL_Color backColor;//fill color of the text box
	SDL_Color boundColor;//outline color of the text box
	SDL_Color textColor;//color of the text entered

	SDL_Rect boxSquare;

	SDL_Renderer* renderer;

	TTF_Font* defaultFont;

	float timeCounter;
	float deltaTime;
	float lastFrameTick;
	float flashingLineTimer;

	void UpdateText(SDL_Event* e);
	
	bool isSelected;


};