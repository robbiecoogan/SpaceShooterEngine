#pragma once

#include "Animator.h"
#include "UIElement.h"
#include "EXComponent.h"

class DoAnimationWindow
{
public:
	DoAnimationWindow(int xLoc, int yLoc, int mainWindowWidth, int mainWindowHeight, EXEventComponent* myEvent, EXActionComponent* myAction, TTF_Font* inputFont);
	~DoAnimationWindow();

	///Update returns whether the game should be running
	bool Update();
	void Draw();
	void GetAnimatedFile();
	int CheckSelected();

	EXEventComponent* pEvent;
	EXActionComponent* pAction;

	Animator* _Animator;

private:
	SDL_Window * gameWindow;
	SDL_Renderer * renderer;
	SDL_Color backColor;

	UIButton* selectFileButton;
	UIButton* okButton;
	UIButton* cancelButton;
	UIButton* deleteButton;
	UIButton* cornerXButton;

	UILabel* framesLabel;
	UITextBox* framesTextBox;

	UILabel* destWidthLabel;
	UITextBox* destWidthTextBox;

	UILabel* destHeightLabel;
	UITextBox* destHeightTextBox;

	int selectedVal;//0 = waiting for user to select, 1 = has selected item, therefore it is safe to grab the returnItem var, -1 = user has cancelled

	SDL_Rect outLineRect;
	SDL_Rect topRect;
	int lastMouseX;
	int lastMouseY;
	bool mouseDown;
	bool lastFrameMouseState;//stores whether the last frame had the left mouse button down. This ensures that the user can't accidentally start dragging the window.

	int windowWidth, windowHeight;
};

