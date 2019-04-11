#include "UIElement.h"
#include "EventManager.h"

UIButton::UIButton(int inputX, int inputY, int inputWidth, int inputHeight, std::string inputText, SDL_Renderer * gameRenderer, SDL_Window* gameWindow, TTF_Font* inputFont) : text(inputText), defaultFont(inputFont), renderer(gameRenderer), window(gameWindow)
{
	x = inputX;
	y = inputY;
	width = inputWidth;
	height = inputHeight;


	type = UIType::Button;
	outlineColor = { 0, 0, 0, 255 };
	notClickedColor = { 100, 100, 100, 255 };
	clickedColor = { 155, 155, 155, 255 };
	textColor = { 0, 0, 0, 255 };

	buttonRect = { x, y, width, height };

	isClicked = false;
	mouseDown = false;
}

UIButton::~UIButton()
{

}

bool UIButton::CheckClick()
{
	if (text == "Load")
	{
		int asd = 0;
	}


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

void UIButton::Update(SDL_Rect activeScreenRect)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	std::vector<SDL_Event*> tempE = EventManager::getInstance().GetEvent(SDL_MOUSEBUTTONUP);
	int eSize = tempE.size();

	for (size_t i = 0; i < eSize; i++)
	{
		if (mouseDown && tempE[i]->window.windowID == SDL_GetWindowID(window))
		{
			isClicked = true;
			mouseDown = false;
		}
	}

	tempE = EventManager::getInstance().GetEvent(SDL_MOUSEBUTTONDOWN);
	eSize = tempE.size();
	for (size_t i = 0; i < eSize; i++)
	{
		if (tempE[i]->window.windowID == SDL_GetWindowID(window))
		{
			if (mouseX > x && mouseX < (x + width) && mouseY > y + yOffset && mouseY < (y + height) + yOffset && SDL_GetMouseState(NULL, NULL) == SDL_BUTTON(SDL_BUTTON_LEFT))//if mouse is inside this and left mouse is pressed
			{
				mouseDown = true;
			}
			else if (mouseX > x && mouseX < (x + width) && mouseY > y + yOffset && mouseY < (y + height) + yOffset == false)//if mouse is outside of the box
			{

			}
		}
	}
}

void UIButton::Draw()
{
	if (mouseDown)
	{
		SDL_SetRenderDrawColor(renderer, clickedColor.r, clickedColor.g, clickedColor.b, clickedColor.a);//if the button is currently clicked set color to clicked color
	}
	else{SDL_SetRenderDrawColor(renderer, notClickedColor.r, notClickedColor.g, notClickedColor.b, notClickedColor.a);//set color to not clicked color
	}

	SDL_Rect tempRect = buttonRect;
	tempRect.y += yOffset;
	SDL_RenderFillRect(renderer, &tempRect);//Draw background rect

	SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);//set color to outline color
	SDL_RenderDrawRect(renderer, &tempRect);//draw outline

	//draw the text
	SDL_Surface* drawText = TTF_RenderText_Solid(defaultFont, text.c_str(), textColor);//create a surface using text & font values
	SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, drawText);//convert the surface to a texture so it can be drawn
	int xPos = x + buttonRect.w/2 - drawText->clip_rect.w/2;
	int yPos = y + buttonRect.h / 2 - drawText->clip_rect.h / 2;
	xPos += xOffset;
	yPos += yOffset;
	//create a rectangle for the text to be drawn in, this will not be the rectangle used for drawing the text box itself.
	SDL_Rect textRect = { xPos, y + yOffset, drawText->w, drawText->h };
	SDL_RenderCopy(renderer, textTex, NULL, &textRect);

	SDL_FreeSurface(drawText);
	drawText = nullptr;
	SDL_DestroyTexture(textTex);
	textTex = nullptr;
}

void UIButton::SetColour(SDL_Color backCol)
{
	notClickedColor = backCol;
}

void UIButton::SetYPos(int inputY)
{
	y = inputY;
	buttonRect.y = y;
}
