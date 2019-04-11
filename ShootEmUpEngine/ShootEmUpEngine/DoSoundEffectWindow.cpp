#include "DoSoundEffectWindow.h"
#include "Common.h"


DoSoundEffectWindow::DoSoundEffectWindow(int xLoc, int yLoc, int mainWindowWidth, int mainWindowHeight, EXEventComponent* myEvent, EXActionComponent* myAction, TTF_Font* inputFont)
{
	backColor = { 70,70,70,255 };
	int borderSize = 10;
	selectedVal = 0;
	mouseDown = false;
	lastFrameMouseState = false;

	int windowWidth = mainWindowWidth / 3;
	int windowHeight = mainWindowHeight / 3;
	gameWindow = SDL_CreateWindow("Add Sound Effect Selector", xLoc + (mainWindowWidth / 2) - (windowWidth / 2), yLoc + (mainWindowHeight / 2) - (windowHeight / 2), windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
	outLineRect = { 0,0, windowWidth,windowHeight };
	topRect = { 0,0,windowWidth, 30 };

	renderer = SDL_CreateRenderer(gameWindow, -1, 0);

	TTF_Font* defaultFont;
	defaultFont = inputFont;

	okButton = new UIButton(windowWidth - 100 + borderSize, topRect.y + topRect.h + borderSize, 80, topRect.h, "Select", renderer, gameWindow, defaultFont);
	cancelButton = new UIButton(windowWidth - 100 + borderSize, okButton->GetY() + okButton->GetHeight() + borderSize, 80, topRect.h, "Cancel", renderer, gameWindow, defaultFont);
	deleteButton = new UIButton(windowWidth - 100 + borderSize, cancelButton->GetY() + cancelButton->GetHeight() + borderSize, 80, topRect.h, "Delete", renderer, gameWindow, defaultFont);
	selectFileButton = new UIButton(windowWidth / 2, topRect.y + topRect.h + borderSize, windowWidth / 4, topRect.h, "File", renderer, gameWindow, defaultFont);

	cornerXButton = new UIButton(topRect.x + topRect.w - topRect.h, topRect.y, topRect.h, topRect.h, "X", renderer, gameWindow, defaultFont);

	pEvent = myEvent;
	pAction = myAction;

	soundManager = SoundManager::getInstance();
}


DoSoundEffectWindow::~DoSoundEffectWindow()
{
	SDL_DestroyWindow(gameWindow);
	SDL_DestroyRenderer(renderer);
	//delete soundManager;
}

bool DoSoundEffectWindow::Update()
{
	int mouseX, mouseY;
	string filename;

	SDL_GetMouseState(&mouseX, &mouseY) == SDL_BUTTON(SDL_BUTTON_LEFT);

	std::vector<SDL_Event*> newEvent = EventManager::getInstance().GetEvent(SDL_WINDOWEVENT);
	int eSize = newEvent.size();

	for (size_t i = 0; i < eSize; i++)
	{
		if (newEvent[i]->window.event == SDL_WINDOWEVENT_CLOSE && newEvent[i]->window.windowID == SDL_GetWindowID(gameWindow))
		{
			return false;//return value is whether the window should stay open or not. In this case, return false if the X button has been pressed, so the window gets closed
		}
	}
	cornerXButton->Update(SDL_Rect{ 0, 0, 0, 0 });
	selectFileButton->Update(SDL_Rect{ 0, 0, 0, 0 });
	okButton->Update(SDL_Rect{ 0, 0, 0, 0 });
	cancelButton->Update(SDL_Rect{ 0, 0, 0, 0 });
	deleteButton->Update(SDL_Rect{ 0, 0, 0, 0 });

	soundManager->Update();

	if (cornerXButton->CheckClick() || cancelButton->CheckClick())
	{
		selectedVal = -1;
	}
	else if (deleteButton->CheckClick())
	{
		selectedVal = 2;
	}
	else if (okButton->CheckClick())
	{
		selectedVal = 1;
	}

	else if (selectFileButton->CheckClick())
	{
		soundManager->PlaySFX(fileDialog->GetFile().c_str());
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

void DoSoundEffectWindow::Draw()
{
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 50);
	SDL_RenderFillRect(renderer, &topRect);


	cornerXButton->Draw();
	selectFileButton->Draw();
	okButton->Draw();
	cancelButton->Draw();
	deleteButton->Draw();



	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderDrawRect(renderer, &outLineRect);
	SDL_RenderDrawRect(renderer, &topRect);


	SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
	SDL_RenderPresent(renderer);
}

int DoSoundEffectWindow::CheckSelected()
{
	return selectedVal;
}