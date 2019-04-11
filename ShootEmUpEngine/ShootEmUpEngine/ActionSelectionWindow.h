#pragma once

#include "UIElement.h"
#include "EXComponent.h"

class ActionSelectionWindow
{
public:
	ActionSelectionWindow(int xLoc, int yLoc, int mainWindowWidth, int mainWindowHeight, EXEventComponent* myEvent, TTF_Font* inputFont);
	~ActionSelectionWindow();

	///Update returns whether the game should be running
	bool Update();

	void Draw();

	int CheckSelected();

	EXEventComponent* pEvent;

private:

	SDL_Window * gameWindow;
	SDL_Renderer * renderer;
	SDL_Color backColor;

	UISelectionBox* mainBox;
	UIVScrollbar* scrollBar;
	UIButton* okayButton;
	UIButton* cancelButton;
	UIButton* cornerXButton;
	UIButton* deleteEventButton;

	

	int selectedVal;//0 = waiting for user to select, 1 = has selected item, therefore it is safe to grab the returnItem var, -1 = user has cancelled

	SDL_Rect outLineRect;
	SDL_Rect topRect;
	int lastMouseX;
	int lastMouseY;
	bool mouseDown;
	bool lastFrameMouseState;//stores whether the last frame had the left mouse button down. This ensures that the user can't accidentally start dragging the window.

};