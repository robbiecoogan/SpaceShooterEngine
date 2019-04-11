#include "EventSelectionWindow.h"
#include "Common.h"

EventSelectionWindow::EventSelectionWindow(int xLoc, int yLoc, int mainWindowWidth, int mainWindowHeight, TTF_Font* inputFont)
{
	backColor = { 70, 70, 70, 255 };
	int borderSize = 10;
	selectedVal = 0;
	mouseDown = false;
	lastFrameMouseState = false;

	int windowWidth = mainWindowWidth / 3;
	int windowHeight = (mainWindowHeight / 3);
	gameWindow = SDL_CreateWindow("Event Selection", xLoc + (mainWindowWidth / 2) - (windowWidth / 2), yLoc + (mainWindowHeight / 2) - (windowHeight / 2), windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
	outLineRect = { 0, 0, windowWidth, windowHeight };
	topRect = { 0, 0, windowWidth, 30 };

	renderer = SDL_CreateRenderer(gameWindow, -1, 0);

	TTF_Font* defaultFont;

	defaultFont = inputFont;

	mainBox = new UISelectionBox(topRect.x, topRect.y + topRect.h - 1, windowWidth - 100, windowHeight - topRect.h + 2, renderer, gameWindow, inputFont, SDL_Rect{ 0, 0, 1000, 1000 });
	mainBox->AddItem(new SelectionBoxItem("On This Hit By Bullet"));
	mainBox->AddItem(new SelectionBoxItem("On This Hit By Player"));
	mainBox->AddItem(new SelectionBoxItem("On This Hit By Enemy"));
	mainBox->AddItem(new SelectionBoxItem("When This Moves Up"));
	mainBox->AddItem(new SelectionBoxItem("When This Moves Down"));
	mainBox->AddItem(new SelectionBoxItem("When This Moves Left"));
	mainBox->AddItem(new SelectionBoxItem("When This Moves Right"));
	mainBox->AddItem(new SelectionBoxItem("Key Press W"));
	mainBox->AddItem(new SelectionBoxItem("Key Press S"));
	mainBox->AddItem(new SelectionBoxItem("Key Press A"));
	mainBox->AddItem(new SelectionBoxItem("Key Press D"));
	mainBox->AddItem(new SelectionBoxItem("Key Press Space"));
	mainBox->AddItem(new SelectionBoxItem("Every Frame"));

	scrollBar = new UIVScrollbar(mainBox->GetX() + mainBox->GetWidth() - 1, mainBox->GetY(), 20, mainBox->GetHeight(), renderer, gameWindow);

	okayButton = new UIButton(mainBox->GetX() + mainBox->GetWidth() + borderSize + scrollBar->GetWidth(), mainBox->GetY() + borderSize, (windowWidth - mainBox->GetWidth() - borderSize - scrollBar->GetWidth() - borderSize), 30, "Select", renderer, gameWindow, defaultFont);
	cancelButton = new UIButton(mainBox->GetX() + mainBox->GetWidth() + borderSize + scrollBar->GetWidth(), okayButton->GetY() + okayButton->GetHeight() + borderSize, (windowWidth - mainBox->GetWidth() - borderSize - scrollBar->GetWidth() - borderSize), 30, "Cancel", renderer, gameWindow, defaultFont);
	cornerXButton = new UIButton(topRect.x + topRect.w - topRect.h, topRect.y, topRect.h, topRect.h, "X", renderer, gameWindow, defaultFont);
}

EventSelectionWindow::~EventSelectionWindow()
{
	SDL_DestroyWindow(gameWindow);
	SDL_DestroyRenderer(renderer);
	delete mainBox;
	delete scrollBar;
}

bool EventSelectionWindow::Update()
{
	int mouseX, mouseY;

	scrollBar->SetMaxVal(mainBox->GetItemCount() * 20);
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

	mainBox->Update(SDL_Rect{ 0, 0, 1000, 1000 });
	scrollBar->Update(SDL_Rect{ 0, 0, 1000, 1000 });
	okayButton->Update(SDL_Rect{ 0, 0, 1000, 1000 });
	cancelButton->Update(SDL_Rect{ 0, 0, 1000, 1000 });
	cornerXButton->Update(SDL_Rect{ 0, 0, 1000, 1000 });


	if (okayButton->CheckClick())
	{
		selectedVal = 1;
		switch (mainBox->GetSelectedIndex())
		{
		case EXEventComponentType::EXThisHitByBullet:
			returnItem = new EXOnThisHitByBullet();
			break;
		case EXEventComponentType::EXThisHitByEnemy:
			returnItem = new EXOnThisHitByEnemy();
			break;
		case EXEventComponentType::EXThisHitByPlayer:
			returnItem = new EXOnThisHitByplayer();
			break;
		case EXEventComponentType::EXWhenMoveDown:
			returnItem = new EXOnMoveDown();
			break;
		case EXEventComponentType::EXWhenMoveLeft:
			returnItem = new EXOnMoveLeft();
			break;
		case EXEventComponentType::EXWhenMoveRight:
			returnItem = new EXOnMoveRight();
			break;
		case EXEventComponentType::EXWhenMoveUp:
			returnItem = new EXOnMoveUp();
			break;
		case EXEventComponentType::EXWKeyPress:
			returnItem = new EXOnKeyPress('w');
			break;
		case EXEventComponentType::EXSKeyPress:
			returnItem = new EXOnKeyPress('s');
			break;
		case EXEventComponentType::EXAKeyPress:
			returnItem = new EXOnKeyPress('a');
			break;
		case EXEventComponentType::EXDKeyPress:
			returnItem = new EXOnKeyPress('d');
			break;
		case EXEventComponentType::EXSpaceKeyPress:
			returnItem = new EXOnKeyPress(' ');
			break;
		case EXEventComponentType::EXEveryFrame:
			returnItem = new EXOnEveryFrame();
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

void EventSelectionWindow::Draw()
{
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 50);
	SDL_RenderFillRect(renderer, &topRect);

	mainBox->Draw();
	scrollBar->Draw();
	okayButton->Draw();
	cancelButton->Draw();
	cornerXButton->Draw();


	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderDrawRect(renderer, &outLineRect);
	SDL_RenderDrawRect(renderer, &topRect);


	SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
	SDL_RenderPresent(renderer);
}

int EventSelectionWindow::CheckSelected()
{
	return selectedVal;
}
