#include "UIElement.h"
#include "EventManager.h"

UIScrollbar::UIScrollbar(int inputX, int inputY, int inputWidth, int inputHeight, SDL_Renderer* gameRenderer, SDL_Window* gameWindow) : renderer(gameRenderer), window(gameWindow)
{
	x = inputX;
	y = inputY;
	width = inputWidth;
	height = inputHeight;

	type = UIType::ScrollBar;

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

void UIScrollbar::Update(SDL_Rect activeScreenRect)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	std::vector<SDL_Event*> tempE = EventManager::getInstance().GetEvent(SDL_MOUSEBUTTONDOWN);


	if (tempE.size() > 0)
	{
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

	}
	tempE = EventManager::getInstance().GetEvent(SDL_MOUSEBUTTONUP);
	if (tempE.size() > 0)
	{
		hasBeenClicked = false;
	}

	if (hasBeenClicked)
	{
		notchRect.x = mouseX - (notchRect.w / 2);
	}

	int calcHighestVal = backRect.x + backRect.w;
	calcHighestVal -= (notchRect.w);
	value = -(notchRect.x - backRect.x) * maxVal;
	if (value < -calcHighestVal * maxVal + 1) value = -calcHighestVal * maxVal;
	if (value > 0 * maxVal - 1) value = 0;

	//clamp x position to it's boundaries
	if (notchRect.x < backRect.x - 1) notchRect.x = backRect.x - 1;
	if (notchRect.x + notchRect.w > backRect.x + backRect.w - 1) notchRect.x = (backRect.x + backRect.w) - notchRect.w - 1;
}

void UIScrollbar::Draw()
{
	//draw back
	SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
	SDL_RenderFillRect(renderer, &backRect);

	//draw notch
	SDL_SetRenderDrawColor(renderer, notchColor.r, notchColor.g, notchColor.b, notchColor.a);
	SDL_RenderFillRect(renderer, &notchRect);

	//draw back outline
	SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
	SDL_RenderDrawRect(renderer, &backRect);

}
