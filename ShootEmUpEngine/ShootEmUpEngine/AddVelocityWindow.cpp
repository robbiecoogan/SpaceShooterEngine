#include "AddVelocityWindow.h"
#include "Common.h"


AddVelocityWindow::AddVelocityWindow(int xLoc, int yLoc, int mainWindowWidth, int mainWindowHeight, EXEventComponent* myEvent, EXActionComponent* myAction, TTF_Font* inputFont)
{
	backColor = { 70,70,70,255 };
	int borderSize = 10;
	selectedVal = 0;
	mouseDown = false;
	lastFrameMouseState = false;

	int windowWidth = mainWindowWidth / 3;
	int windowHeight = mainWindowHeight / 3;

	gameWindow = SDL_CreateWindow("Add Velocity Window", xLoc + (mainWindowWidth / 2) - (windowWidth / 2), yLoc + (mainWindowHeight / 2) - (windowHeight / 2), windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
	outLineRect = { 0,0, windowWidth,windowHeight };
	topRect = { 0,0,windowWidth, 30 };

	renderer = SDL_CreateRenderer(gameWindow, -1, 0);

	TTF_Font* defaultFont;
	defaultFont = inputFont;

	okButton = new UIButton(windowWidth - 100 + borderSize, topRect.y + topRect.h + borderSize, 80, topRect.h, "Select", renderer, gameWindow, defaultFont);
	cancelButton = new UIButton(windowWidth - 100 + borderSize, okButton->GetY() + okButton->GetHeight() + borderSize, 80, topRect.h, "Cancel", renderer, gameWindow, defaultFont);
	deleteButton = new UIButton(windowWidth - 100 + borderSize, cancelButton->GetY() + cancelButton->GetHeight() + borderSize, 80, topRect.h, "Delete", renderer, gameWindow, defaultFont);

	cornerXButton = new UIButton(topRect.x + topRect.w - topRect.h, topRect.y, topRect.h, topRect.h, "X", renderer, gameWindow, defaultFont);

	velocityXLabel = new UILabel(windowWidth / 3, topRect.y + topRect.h + borderSize, "Velocity X:", renderer, gameWindow, 1, defaultFont);
	velocityXTextBox = new UITextBox(velocityXLabel->GetX() + velocityXLabel->GetWidth(), velocityXLabel->GetY(), okButton->GetWidth() + borderSize, renderer, gameWindow, defaultFont);

	velocityYLabel = new UILabel(windowWidth / 3, velocityXLabel->GetY() + velocityXLabel->GetHeight() + borderSize, "Velocity Y:", renderer, gameWindow, 1, defaultFont);
	velocityYTextBox = new UITextBox(velocityYLabel->GetX() + velocityYLabel->GetWidth(), velocityYLabel->GetY(), okButton->GetWidth() + borderSize, renderer, gameWindow, defaultFont);

	pEvent = myEvent;
	pAction = myAction;

	velocityXTextBox->SetText(std::to_string(((EXAddVelocity*)pAction)->xVel));
	velocityYTextBox->SetText(std::to_string(((EXAddVelocity*)pAction)->yVel));
}


AddVelocityWindow::~AddVelocityWindow()
{
	SDL_DestroyWindow(gameWindow);
	SDL_DestroyRenderer(renderer);
}

bool AddVelocityWindow::Update()
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
	velocityXLabel->Update(SDL_Rect{ 0, 0, 0, 0 });
	velocityXTextBox->Update(SDL_Rect{ 0, 0, 0, 0 });
	velocityYLabel->Update(SDL_Rect{ 0, 0, 0, 0 });
	velocityYTextBox->Update(SDL_Rect{ 0, 0, 0, 0 });

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
		((EXAddVelocity*)pAction)->xVel = (std::stof(velocityXTextBox->GetText()));
		((EXAddVelocity*)pAction)->yVel = (std::stof(velocityYTextBox->GetText()));
		selectedVal = 1;
	}

	if (velocityXTextBox->IsUpdated())
	{
		if (velocityXTextBox->GetText() != "")
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

void AddVelocityWindow::Draw()
{
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 50);
	SDL_RenderFillRect(renderer, &topRect);

	cornerXButton->Draw();
	okButton->Draw();
	cancelButton->Draw();
	deleteButton->Draw();
	velocityXLabel->Draw();
	velocityXTextBox->Draw();
	velocityYLabel->Draw();
	velocityYTextBox->Draw();

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderDrawRect(renderer, &outLineRect);
	SDL_RenderDrawRect(renderer, &topRect);


	SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
	SDL_RenderPresent(renderer);
}

int AddVelocityWindow::CheckSelected()
{
	return selectedVal;
}