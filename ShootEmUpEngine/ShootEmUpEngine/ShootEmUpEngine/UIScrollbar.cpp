#include "UIScrollbar.h"

UIScrollbar::UIScrollbar(int inputX, int inputY, int inputWidth, int inputHeight, SDL_Renderer* gameRenderer) : x(inputX), y(inputY), width(inputWidth), height(inputHeight), renderer(gameRenderer)
{
	minVal = 0;
	maxVal = 10;

	backRect = { x, y, width, height };
	notchRect = { backRect.x, backRect.y + 1, backRect.w / (maxVal - minVal) , backRect.h - 2 };

	backColor = { 70, 70, 70, 255 };
	notchColor = { 120, 120, 120, 255 };
	outlineColor = { 0, 0, 0, 255 };

	hasBeenClicked = false;
}

UIScrollbar::~UIScrollbar()
{

}

void UIScrollbar::Update(SDL_Event* e)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	switch (e->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		//std::cout << "Mouse Down Detected" << std::endl;
		//check if the user has clicked on the notch
		if (hasBeenClicked == false && mouseX > notchRect.x && mouseX < notchRect.x + notchRect.w && mouseY > notchRect.y && mouseY < notchRect.y + notchRect.h)
		{
			hasBeenClicked = true;
		}

		//check if the user has clicked the bar, but also check if the user has not clicked the notch
		if (mouseX > notchRect.x + notchRect.w && mouseX > backRect.x && mouseX < backRect.x + backRect.w && mouseY > backRect.y && mouseY < backRect.y + backRect.h && (mouseX > notchRect.x && mouseX < notchRect.x + notchRect.w && mouseY > notchRect.y && mouseY < notchRect.y + notchRect.h) == false)
		{
			notchRect.x += notchRect.w;
		}
		else if (mouseX < notchRect.x && mouseX > backRect.x && mouseX < backRect.x + backRect.w && mouseY > backRect.y && mouseY < backRect.y + backRect.h && (mouseX > notchRect.x && mouseX < notchRect.x + notchRect.w && mouseY > notchRect.y && mouseY < notchRect.y + notchRect.h) == false)
		{
			notchRect.x -= notchRect.w;
		}

		break;
	case SDL_MOUSEBUTTONUP:
		//std::cout << "Mouse Up Detected" << std::endl;
		hasBeenClicked = false;
		break;
	}

	if (hasBeenClicked)
	{
		notchRect.x = mouseX - (notchRect.w / 2);
	}


	//clamp x position to it's boundaries
	if (notchRect.x < backRect.x + 1) notchRect.x = backRect.x + 1;
	if (notchRect.x + notchRect.w > backRect.x + backRect.w - 1) notchRect.x = (backRect.x + backRect.w) - notchRect.w - 1;
}

void UIScrollbar::Draw()
{
	//draw back
	SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
	SDL_RenderFillRect(renderer, &backRect);

	//draw back outline
	SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
	SDL_RenderDrawRect(renderer, &backRect);

	//draw notch
	SDL_SetRenderDrawColor(renderer, notchColor.r, notchColor.g, notchColor.b, notchColor.a);
	SDL_RenderFillRect(renderer, &notchRect);
}
