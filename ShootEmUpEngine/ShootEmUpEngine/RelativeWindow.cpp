#include "RelativeWindow.h"
#include "Common.h"


RelativeWindow::RelativeWindow(int xLoc, int yLoc, int mainWindowWidth, int mainWindowHeight, EXEventComponent* myEvent, EXActionComponent* myAction, TTF_Font* inputFont)
{
	backColor = { 70,70,70,255 };
	int borderSize = 10;
	selectedVal = 0;
	mouseDown = false;
	lastFrameMouseState = false;

	int windowWidth = mainWindowWidth / 3;
	int windowHeight = mainWindowHeight / 3;

	gameWindow = SDL_CreateWindow("Relative Window", xLoc + (mainWindowWidth / 2) - (windowWidth / 2), yLoc + (mainWindowHeight / 2) - (windowHeight / 2), windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
	outLineRect = { 0,0, windowWidth,windowHeight };
	topRect = { 0,0,windowWidth, 30 };

	renderer = SDL_CreateRenderer(gameWindow, -1, 0);

	TTF_Font* defaultFont;
	defaultFont = inputFont;

	okButton = new UIButton(windowWidth - 100 + borderSize, topRect.y + topRect.h + borderSize, 80, topRect.h, "Select", renderer, gameWindow, defaultFont);
	cancelButton = new UIButton(windowWidth - 100 + borderSize, okButton->GetY() + okButton->GetHeight() + borderSize, 80, topRect.h, "Cancel", renderer, gameWindow, defaultFont);
	deleteButton = new UIButton(windowWidth - 100 + borderSize, cancelButton->GetY() + cancelButton->GetHeight() + borderSize, 80, topRect.h, "Delete", renderer, gameWindow, defaultFont);

	cornerXButton = new UIButton(topRect.x + topRect.w - topRect.h, topRect.y, topRect.h, topRect.h, "X", renderer, gameWindow, defaultFont);

	XLabel = new UILabel(windowWidth / 2, topRect.y + topRect.h + borderSize, "X:", renderer, gameWindow, 1, defaultFont);
	XTextBox = new UITextBox(XLabel->GetX() + XLabel->GetWidth(), XLabel->GetY(), okButton ->GetWidth() + borderSize, renderer, gameWindow, defaultFont);

	YLabel = new UILabel(windowWidth / 2, XLabel->GetY() + XLabel->GetHeight() + borderSize, "Y:", renderer, gameWindow, 1, defaultFont);
	YTextBox = new UITextBox(YLabel->GetX() + YLabel->GetWidth(), YLabel->GetY(), okButton->GetWidth() + borderSize, renderer, gameWindow, defaultFont);

	pEvent = myEvent;
	pAction = myAction;

	XTextBox->SetText(std::to_string(((EXMoveRelative*)pAction)->X));
	YTextBox->SetText(std::to_string(((EXMoveRelative*)pAction)->Y));
}


RelativeWindow::~RelativeWindow()
{
	SDL_DestroyWindow(gameWindow);
	SDL_DestroyRenderer(renderer);
}

bool RelativeWindow::Update()
{
	int mouseX, mouseY;

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
	okButton->Update(SDL_Rect{ 0, 0, 0, 0 });
	cancelButton->Update(SDL_Rect{ 0, 0, 0, 0 });
	deleteButton->Update(SDL_Rect{ 0, 0, 0, 0 });
	XLabel->Update(SDL_Rect{ 0, 0, 0, 0 });
	XTextBox->Update(SDL_Rect{ 0, 0, 0, 0 });
	YLabel->Update(SDL_Rect{ 0, 0, 0, 0 });
	YTextBox->Update(SDL_Rect{ 0, 0, 0, 0 });

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
		((EXMoveRelative*)pAction)->X = (std::stof(XTextBox->GetText()));
		((EXMoveRelative*)pAction)->Y = (std::stof(YTextBox->GetText()));
		selectedVal = 1;
	}

	if (XTextBox->IsUpdated())
	{
		if (XTextBox->GetText() != "")
		{
			
		}
	}
	if (YTextBox->IsUpdated())
	{
		if (YTextBox->GetText() != "")
		{

		}
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

void RelativeWindow::Draw()
{
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 50);
	SDL_RenderFillRect(renderer, &topRect);

	cornerXButton->Draw();
	okButton->Draw();
	cancelButton->Draw();
	deleteButton->Draw();
	XLabel->Draw();
	XTextBox->Draw();
	YLabel->Draw();
	YTextBox->Draw();

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderDrawRect(renderer, &outLineRect);
	SDL_RenderDrawRect(renderer, &topRect);


	SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
	SDL_RenderPresent(renderer);
}

int RelativeWindow::CheckSelected()
{
	return selectedVal;
}