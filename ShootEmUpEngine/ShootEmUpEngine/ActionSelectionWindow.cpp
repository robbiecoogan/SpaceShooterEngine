#include "ActionSelectionWindow.h"
#include "Common.h"

ActionSelectionWindow::ActionSelectionWindow(int xLoc, int yLoc, int mainWindowWidth, int mainWindowHeight, EXEventComponent* myEvent, TTF_Font* inputFont)
{
	backColor = { 70, 70, 70, 255 };
	int borderSize = 10;
	selectedVal = 0;
	mouseDown = false;
	lastFrameMouseState = false;

	int windowWidth = mainWindowWidth / 3;
	int windowHeight = (mainWindowHeight / 3);
	gameWindow = SDL_CreateWindow("Action Selection", xLoc + (mainWindowWidth / 2) - (windowWidth / 2), yLoc + (mainWindowHeight / 2) - (windowHeight / 2), windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
	outLineRect = { 0, 0, windowWidth, windowHeight };
	topRect = { 0, 0, windowWidth, 30 };

	renderer = SDL_CreateRenderer(gameWindow, -1, 0);

	TTF_Font* defaultFont;

	defaultFont = inputFont;

	mainBox = new UISelectionBox(topRect.x, topRect.y + topRect.h - 1, windowWidth - 100, windowHeight - topRect.h + 2, renderer, gameWindow, defaultFont, SDL_Rect{ 0,0,0,0 });
	mainBox->AddItem(new SelectionBoxItem("Do Animation"));
	mainBox->AddItem(new SelectionBoxItem("Move Relative"));
	mainBox->AddItem(new SelectionBoxItem("Add Sound Effect"));
	mainBox->AddItem(new SelectionBoxItem("Score and Lives"));
	mainBox->AddItem(new SelectionBoxItem("Shoot"));
	mainBox->AddItem(new SelectionBoxItem("Add Velocity"));
	mainBox->AddItem(new SelectionBoxItem("Subtract Velocity"));
	mainBox->AddItem(new SelectionBoxItem("Multiply Velocity"));

	scrollBar = new UIVScrollbar(mainBox->GetX() + mainBox->GetWidth() - 1, mainBox->GetY(), 20, mainBox->GetHeight(), renderer, gameWindow);

	okayButton = new UIButton(mainBox->GetX() + mainBox->GetWidth() + borderSize + scrollBar->GetWidth(), mainBox->GetY() + borderSize, (windowWidth - mainBox->GetWidth() - borderSize - scrollBar->GetWidth() - borderSize), 30, "Select", renderer, gameWindow, defaultFont);
	cancelButton = new UIButton(mainBox->GetX() + mainBox->GetWidth() + borderSize + scrollBar->GetWidth(), okayButton->GetY() + okayButton->GetHeight() + borderSize, (windowWidth - mainBox->GetWidth() - borderSize - scrollBar->GetWidth() - borderSize), 30, "Cancel", renderer, gameWindow, defaultFont);
	cornerXButton = new UIButton(topRect.x + topRect.w - topRect.h, topRect.y, topRect.h, topRect.h, "X", renderer, gameWindow, defaultFont);
	deleteEventButton = new UIButton(cancelButton->GetX(), cancelButton->GetY() + cancelButton->GetHeight() + borderSize * 3, cancelButton->GetWidth(), cancelButton->GetHeight(), "Delete", renderer, gameWindow, defaultFont);

	pEvent = myEvent;
}

ActionSelectionWindow::~ActionSelectionWindow()
{
	SDL_DestroyWindow(gameWindow);
	SDL_DestroyRenderer(renderer);
	delete mainBox;
	delete scrollBar;
}

bool ActionSelectionWindow::Update()
{
	int mouseX, mouseY;

	scrollBar->SetMaxVal(mainBox->GetItemCount());
	mainBox->scrollOffset = (-scrollBar->GetValue() / 10);


	if (SDL_GetMouseState(&mouseX, &mouseY) == SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		mainBox->CheckSelected(mouseX, mouseY);
	}

	std::vector<SDL_Event*> newEvent = EventManager::getInstance().GetEvent(SDL_WINDOWEVENT);
	int eSize = newEvent.size();

	for (size_t i = 0; i < eSize; i++)
	{
		if (newEvent[i]->window.event == SDL_WINDOWEVENT_CLOSE && newEvent[i]->window.windowID == SDL_GetWindowID(gameWindow))
		{
			return false;//return value is whether the window should stay open or not. In this case, return false if the X button has been pressed, so the window gets closed
		}
	}

	mainBox->Update(SDL_Rect{ 0, 0, 0, 0 });//NULL rect given as the UISelectionBox doesn't use this.
	scrollBar->Update(SDL_Rect{ 0, 0, 0, 0 });
	okayButton->Update(SDL_Rect{ 0, 0, 0, 0 });
	cancelButton->Update(SDL_Rect{ 0, 0, 0, 0 });
	cornerXButton->Update(SDL_Rect{ 0, 0, 0, 0 });
	deleteEventButton->Update(SDL_Rect{ 0, 0, 0, 0 });

	if (deleteEventButton->CheckClick())
	{
		selectedVal = 2;
	}

	if (okayButton->CheckClick())
	{
		selectedVal = 1;
		switch (mainBox->GetSelectedIndex())
		{
		case ActionComponentType::ADoAnimation:
			pEvent->AddAction(new EXDoAnimation("", 0, 0, 0));
			break;
		case ActionComponentType::AMoveRelative:
			pEvent->AddAction(new EXMoveRelative(0, 0));
			break;
		case ActionComponentType::ASoundEffect:
			pEvent->AddAction(new EXAddSoundEffect(""));
			break;
		case ActionComponentType::AScoreLives:
			pEvent->AddAction(new EXScoreAndLives(0, 0));
			break;
		case ActionComponentType::AShoot:
			pEvent->AddAction(new EXShoot(0, 0));
			break;
		case ActionComponentType::AAddVelocity:
			pEvent->AddAction(new EXAddVelocity(0,0));
			break;
		case ActionComponentType::ASubtractVelocity:
			pEvent->AddAction(new EXSubtractVelocity(0, 0));
			break;
		case ActionComponentType::AMultiplyVelocity:
			pEvent->AddAction(new EXMultiplyVelocity(0, 0));
			break;
		}
	}
	else if (cancelButton->CheckClick() || cornerXButton->CheckClick())
	{
		selectedVal = -1;
	}


	int globalMouseX, globalMouseY, globalWindowX, globalWindowY;
	if (SDL_GetGlobalMouseState(&globalMouseX, &globalMouseY) == SDL_BUTTON(SDL_BUTTON_LEFT) && mouseX > topRect.x && mouseX < topRect.x + topRect.w && mouseY > topRect.y && mouseY < topRect.y + topRect.h)
	{
		mouseDown = true;
	}
	SDL_GetWindowPosition(gameWindow, &globalWindowX, &globalWindowY);

	if (SDL_GetGlobalMouseState(&globalMouseX, &globalMouseY) != SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		mouseDown = false;
	}

	if (mouseDown)
	{
		int xDiff = lastMouseX - globalMouseX;
		int ydiff = lastMouseY - globalMouseY;

		SDL_SetWindowPosition(gameWindow, globalWindowX - xDiff, globalWindowY - ydiff);
	}

	lastMouseX = globalMouseX;
	lastMouseY = globalMouseY;

	return true;
}

void ActionSelectionWindow::Draw()
{
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 50);
	SDL_RenderFillRect(renderer, &topRect);

	mainBox->Draw();
	scrollBar->Draw();
	okayButton->Draw();
	cancelButton->Draw();
	cornerXButton->Draw();
	deleteEventButton->Draw();


	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderDrawRect(renderer, &outLineRect);
	SDL_RenderDrawRect(renderer, &topRect);


	SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
	SDL_RenderPresent(renderer);
}

int ActionSelectionWindow::CheckSelected()
{
	return selectedVal;
}
