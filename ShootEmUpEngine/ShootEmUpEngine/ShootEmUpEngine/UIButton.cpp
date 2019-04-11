#include "UIButton.h"

UIButton::UIButton(int inputX, int inputY, int inputWidth, int inputHeight, std::string inputText, SDL_Renderer * gameRenderer, TTF_Font* inputFont) : x(inputX), y(inputY), width(inputWidth), height(inputHeight), text(inputText), defaultFont(inputFont), renderer(gameRenderer)
{
	outlineColor = { 0, 0, 0, 255 };
	notClickedColor = { 100, 100, 100, 255 };
	clickedColor = { 155, 155, 155, 255 };
	textColor = { 0, 0, 0, 255 };

	buttonRect = { x, y, width, height };

	isClicked = false;
}

UIButton::~UIButton()
{

}

bool UIButton::CheckClick()
{
	if (isClicked)
	{
		isClicked = false;
		return true;
	}
	else
	{
		return false;
	}
}

void UIButton::Update(SDL_Event* e)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	switch (e->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (mouseX > x && mouseX < (x + width) && mouseY > y && mouseY < (y + height) && SDL_GetMouseState(NULL, NULL) == SDL_BUTTON(SDL_BUTTON_LEFT))//if mouse is inside this and left mouse is pressed
		{
			isClicked = true;
		}
		else if (mouseX > x && mouseX < (x + width) && mouseY > y && mouseY < (y + height) == false)//if mouse is outside of the box
		{

		}
		break;
	case SDL_MOUSEBUTTONUP:
		isClicked = false;
		break;
	}
}

void UIButton::Draw()
{
	if (isClicked){SDL_SetRenderDrawColor(renderer, clickedColor.r, clickedColor.g, clickedColor.b, clickedColor.a);//if the button is currently clicked set color to clicked color
	}
	else{SDL_SetRenderDrawColor(renderer, notClickedColor.r, notClickedColor.g, notClickedColor.b, notClickedColor.a);//set color to not clicked color
	}

	SDL_RenderFillRect(renderer, &buttonRect);//Draw background rect

	SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);//set color to outline color
	SDL_RenderDrawRect(renderer, &buttonRect);//draw outline

	//draw the text
	SDL_Surface* drawText = TTF_RenderText_Solid(defaultFont, text.c_str(), textColor);//create a surface using text & font values
	SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, drawText);//convert the surface to a texture so it can be drawn
	//create a rectangle for the text to be drawn in, this will not be the rectangle used for drawing the text box itself.
	SDL_Rect textRect = { buttonRect.x + 4, buttonRect.y - 4, drawText->w, drawText->h };
	SDL_RenderCopy(renderer, textTex, NULL, &textRect);

	SDL_FreeSurface(drawText);
	drawText = nullptr;
	SDL_DestroyTexture(textTex);
	textTex = nullptr;
}