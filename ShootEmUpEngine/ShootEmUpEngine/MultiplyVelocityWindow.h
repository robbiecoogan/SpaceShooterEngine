#pragma once

#include "UIElement.h"
#include "Component.h"

class MultiplyVelocityWindow
{
public:
	MultiplyVelocityWindow(int xLoc, int yLoc, int mainWindowWidth, int mainWindowHeight, EXEventComponent* myEvent, EXActionComponent* myAction, TTF_Font* inputFont);
	~MultiplyVelocityWindow();

	bool Update();
	void Draw();

	int CheckSelected();

	EXEventComponent* pEvent;
	EXActionComponent* pAction;

private:
	SDL_Window * gameWindow;
	SDL_Renderer * renderer;
	SDL_Color backColor;

	UIButton* okButton;
	UIButton* cancelButton;
	UIButton* deleteButton;
	UIButton* cornerXButton;

	UILabel* velocityXLabel;
	UILabel* velocityYLabel;

	UITextBox* velocityXTextBox;
	UITextBox* velocityYTextBox;



	int selectedVal;//0 = waiting for user to select, 1 = has selected item, therefore it is safe to grab the returnItem var, -1 = user has cancelled

	SDL_Rect outLineRect;
	SDL_Rect topRect;
	int lastMouseX;
	int lastMouseY;
	bool mouseDown;
	bool lastFrameMouseState;//stores whether the last frame had the left mouse button down. This ensures that the user can't accidentally start dragging the window.
};

