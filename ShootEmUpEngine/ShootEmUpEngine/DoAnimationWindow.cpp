#include "DoAnimationWindow.h"
#include "Common.h"


DoAnimationWindow::DoAnimationWindow(int xLoc, int yLoc, int mainWindowWidth, int mainWindowHeight, EXEventComponent* myEvent, EXActionComponent* myAction, TTF_Font* inputFont)
{
	backColor = { 70,70,70,255 };
	int borderSize = 10;
	selectedVal = 0;
	mouseDown = false;
	lastFrameMouseState = false;

	windowWidth = mainWindowWidth / 3;
	windowHeight = mainWindowHeight / 3;
	gameWindow = SDL_CreateWindow("Animation Selector", xLoc + (mainWindowWidth / 2) - (windowWidth / 2), yLoc + (mainWindowHeight / 2) - (windowHeight / 2),windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
	outLineRect = { 0,0, windowWidth,windowHeight };
	topRect = {0,0,windowWidth, 30};

	renderer = SDL_CreateRenderer(gameWindow, -1, 0);

	TTF_Font* defaultFont;
	defaultFont = inputFont;

	okButton = new UIButton(windowWidth - 100 + borderSize, topRect.y + topRect.h + borderSize, 80, topRect.h, "Select", renderer, gameWindow, defaultFont);
	cancelButton = new UIButton(windowWidth - 100 + borderSize, okButton->GetY() + okButton->GetHeight() + borderSize, 80, topRect.h, "Cancel", renderer, gameWindow, defaultFont);
	deleteButton = new UIButton(windowWidth - 100 + borderSize, cancelButton->GetY() + cancelButton->GetHeight() + borderSize, 80, topRect.h, "Delete", renderer, gameWindow, defaultFont);
	selectFileButton = new UIButton(windowWidth / 2, topRect.y + topRect.h + borderSize, windowWidth / 4, topRect.h, "File", renderer, gameWindow, defaultFont);

	cornerXButton = new UIButton(topRect.x + topRect.w - topRect.h, topRect.y, topRect.h, topRect.h, "X", renderer, gameWindow, defaultFont);

	framesLabel = new UILabel(selectFileButton->GetX(), selectFileButton->GetY() + selectFileButton->GetHeight() + borderSize, "Frames: ", renderer, gameWindow, 0, defaultFont);
	framesTextBox = new UITextBox(framesLabel->GetX() + framesLabel->GetWidth(), framesLabel->GetY(), framesLabel->GetWidth(), renderer, gameWindow, defaultFont);

	destWidthLabel = new UILabel(framesLabel->GetX(), framesLabel->GetY() + framesLabel->GetHeight() + borderSize, "Dest W: ", renderer, gameWindow, 0, defaultFont);
	destWidthTextBox = new UITextBox(destWidthLabel->GetX() + destWidthLabel->GetWidth(), destWidthLabel->GetY(), framesTextBox->GetWidth(), renderer, gameWindow, defaultFont);

	destHeightLabel = new UILabel(destWidthLabel->GetX(), destWidthLabel->GetY() + destWidthLabel->GetHeight() + borderSize, "Dest H: ", renderer, gameWindow, 0, defaultFont);
	destHeightTextBox = new UITextBox(destHeightLabel->GetX() + destHeightLabel->GetWidth(), destHeightLabel->GetY(), destWidthTextBox->GetWidth(), renderer, gameWindow, defaultFont);

	pEvent = myEvent;
	pAction = myAction;

	_Animator = new Animator(renderer, gameWindow, SDL_Rect{ windowWidth / 5, windowHeight / 5, windowWidth / 5, windowHeight / 5 });//Create Animator

	framesTextBox->SetText(std::to_string(((EXDoAnimation*)pAction)->frames));
	destWidthTextBox->SetText(std::to_string(((EXDoAnimation*)pAction)->destWidth));
	destHeightTextBox->SetText(std::to_string(((EXDoAnimation*)pAction)->destHeight));
	_Animator->SetAnimFile(((EXDoAnimation*)pAction)->frames, ((EXDoAnimation*)pAction)->fileDir);

}


DoAnimationWindow::~DoAnimationWindow()
{
	SDL_DestroyWindow(gameWindow);
	SDL_DestroyRenderer(renderer);
	_Animator->~Animator();
}

bool DoAnimationWindow::Update()
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
	selectFileButton->Update(SDL_Rect{ 0, 0, 0, 0 });
	okButton->Update(SDL_Rect{ 0, 0, 0, 0 });
	cancelButton->Update(SDL_Rect{ 0, 0, 0, 0 });
	deleteButton->Update(SDL_Rect{ 0, 0, 0, 0 });
	framesLabel->Update(SDL_Rect{ 0, 0, 0, 0 });
	framesTextBox->Update(SDL_Rect{ 0, 0, 0, 0 });
	destWidthLabel->Update(SDL_Rect{ 0, 0, 0, 0 });
	destWidthTextBox->Update(SDL_Rect{ 0, 0, 0, 0 });
	destHeightLabel->Update(SDL_Rect{ 0, 0, 0, 0 });
	destHeightTextBox->Update(SDL_Rect{ 0, 0, 0, 0 });
	
	_Animator->Update(), SDL_Rect{ 0, 0, 0, 0 };

	if (framesTextBox->IsUpdated() && framesTextBox->GetText() != "")
		_Animator->SetFrames(std::atoi(framesTextBox->GetText().c_str()));
	else if (destWidthTextBox->IsUpdated() && destWidthTextBox->GetText() != "")
	{
		int w = std::stoi(destWidthTextBox->GetText());
		int h = std::stoi(destHeightTextBox->GetText());
		_Animator->SetWidthHeight(w, h);
	}
	else if (destHeightTextBox->IsUpdated() && destHeightTextBox->GetText() != "")
	{
		int w = std::stoi(destWidthTextBox->GetText());
		int h = std::stoi(destHeightTextBox->GetText());
		_Animator->SetWidthHeight(w, h);
	}

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
		((EXDoAnimation*)pAction)->fileDir = _Animator->GetFileDir();
		((EXDoAnimation*)pAction)->frames = std::stoi(framesTextBox->GetText());
		((EXDoAnimation*)pAction)->destWidth = std::stoi(destWidthTextBox->GetText());
		((EXDoAnimation*)pAction)->destHeight = std::stoi(destWidthTextBox->GetText());
		selectedVal = 1;
	}

	else if (selectFileButton->CheckClick())
	{
		_Animator->SetAnimFile();
		//_Animator.push_back(new Animator(renderer, gameWindow, SDL_Rect{windowWidth/5, windowHeight/5, windowWidth/5, windowHeight/5}));//Create Animator
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

void DoAnimationWindow::Draw()
{
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 50);
	SDL_RenderFillRect(renderer, &topRect);

	//for (unsigned int i = 0; i < _Animator.size(); i++) { _Animator[i]->Draw(); }
	_Animator->Draw();

	cornerXButton->Draw();
	selectFileButton->Draw();
	okButton->Draw();
	cancelButton->Draw();
	deleteButton->Draw();
	framesLabel->Draw();
	framesTextBox->Draw();
	destWidthLabel->Draw();
	destWidthTextBox->Draw();
	destHeightLabel->Draw();
	destHeightTextBox->Draw();



	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderDrawRect(renderer, &outLineRect);
	SDL_RenderDrawRect(renderer, &topRect);


	SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
	SDL_RenderPresent(renderer);
}

int DoAnimationWindow::CheckSelected()
{
	return selectedVal;
}
