#include "UIElement.h"
#include "EventManager.h"

UIVScrollbar::UIVScrollbar(int inputX, int inputY, int inputWidth, int inputHeight, SDL_Renderer* gameRenderer, SDL_Window* gameWindow) : renderer(gameRenderer), window(gameWindow)
{
	x = inputX;
	y = inputY;
	width = inputWidth;
	height = inputHeight;

	type = UIType::VScrollBar;

	minVal = 0;
	maxVal = 10;

	backRect = { x, y, width, height };
	//notchRect = { backRect.x, backRect.y + 1, backRect.w / (maxVal - minVal) , backRect.h - 2 };
	notchRect = { backRect.x + 1, backRect.y, backRect.w - 2  , backRect.h / (int)(maxVal - minVal) };

	backColor = { 70, 70, 70, 255 };
	notchColor = { 120, 120, 120, 255 };
	outlineColor = { 0, 0, 0, 255 };

	hasBeenClicked = false;
}

UIVScrollbar::~UIVScrollbar()
{

}

void UIVScrollbar::Update(SDL_Rect activeScreenRect)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	std::vector<SDL_Event*> tempE = EventManager::getInstance().GetEvent(SDL_MOUSEBUTTONDOWN);
	int eSize = tempE.size();

	if (eSize > 0)
	{
		//check if the user has clicked on the notch
		if (hasBeenClicked == false && mouseX > notchRect.x && mouseX < notchRect.x + notchRect.w && mouseY > notchRect.y + yOffset && mouseY < notchRect.y + yOffset + notchRect.h)
		{
			hasBeenClicked = true;
		}

		//check if the user has clicked the bar, but also check if the user has not clicked the notch
		if (mouseY > notchRect.y + notchRect.h + yOffset && mouseY > backRect.y + yOffset && mouseY < backRect.y + backRect.h + yOffset && mouseX > backRect.x && mouseX < backRect.x + backRect.w && (mouseY > notchRect.y + yOffset && mouseY < notchRect.y + yOffset + notchRect.h && mouseX > notchRect.x && mouseX < notchRect.x + notchRect.w) == false)
		{
			notchRect.y += notchRect.w;
		}
		else if (mouseY < notchRect.y + yOffset && mouseY > backRect.y + yOffset && mouseY < backRect.y + yOffset + backRect.h && mouseX > backRect.x && mouseX < backRect.x + backRect.w && (mouseY > notchRect.y + yOffset && mouseY < notchRect.y + yOffset + notchRect.h && mouseX > notchRect.x + yOffset && mouseX < notchRect.x + notchRect.w) == false)
		{
			notchRect.y -= notchRect.w;
		}

	}
	tempE = EventManager::getInstance().GetEvent(SDL_MOUSEBUTTONUP);
	if (tempE.size() > 0)
	{
		hasBeenClicked = false;
	}

	if (hasBeenClicked)
	{
		notchRect.y = mouseY - (notchRect.h / 2);
	}

	if (tempE.size() > 0)//to prevent a weird bug that keeps the notch moving when the mouse is released on another window
	{
		hasBeenClicked = false;
	}
	tempE = EventManager::getInstance().GetEvent(SDL_MOUSEWHEEL);
	eSize = tempE.size();
	for (size_t i = 0; i < eSize; i++)
	{
		if (tempE[i]->window.windowID == SDL_GetWindowID(window))
		{
			notchRect.y -= tempE[i]->wheel.y * 10;
		}
	}


	int calcHighestVal = backRect.y + backRect.h;
	calcHighestVal -= (notchRect.h);
	value = -(notchRect.y - backRect.y) * maxVal;
	if (value < -calcHighestVal * maxVal) value = -calcHighestVal * maxVal;
	if (value > 0 * maxVal) value = 0;

	//clamp x position to it's boundaries
	if (notchRect.y < backRect.y + 1) notchRect.y = backRect.y + 1;
	if (notchRect.y + notchRect.h > backRect.y + backRect.h - 1) notchRect.y = (backRect.y + backRect.h) - notchRect.h - 1;
}

void UIVScrollbar::Draw()
{
	//draw back
	SDL_Rect tempRect = backRect;
	backRect.y += yOffset;
	SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
	SDL_RenderFillRect(renderer, &tempRect);

	//draw back outline
	SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
	SDL_RenderDrawRect(renderer, &tempRect);

	//draw notch
	tempRect = notchRect;
	backRect.y += yOffset;
	SDL_SetRenderDrawColor(renderer, notchColor.r, notchColor.g, notchColor.b, notchColor.a);
	SDL_RenderFillRect(renderer, &tempRect);
}

void UIVScrollbar::SetMaxVal(float inputVal)
{
	maxVal = inputVal/25.0f;
	notchRect.h = backRect.h / (maxVal - minVal);
}
