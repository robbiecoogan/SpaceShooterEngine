#include "GameObject.h"
#include "FileDialog.h"
#include "EventManager.h"

GameObject::GameObject(std::string inputName, int inputX, int inputY, std::string imageDir, SDL_Window* gameWindow, SDL_Renderer* gameRenderer) : name(inputName), x(inputX), y(inputY), renderer(gameRenderer)
{
	texture = NULL;
	SDL_Surface* loadSurface = IMG_Load(imageDir.c_str());

	std::string basePath = SDL_GetBasePath();
	spriteDir = imageDir.substr(basePath.size(), imageDir.size() - basePath.size());
	
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	if (loadSurface == NULL) std::cout << "Error Loading Image: " << IMG_GetError() << std::endl;
	else
	{
		SDL_ConvertSurfaceFormat(loadSurface, SDL_PIXELFORMAT_RGBA8888, 0);
		
		texture = SDL_CreateTextureFromSurface(renderer, loadSurface);

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		sourceRect = loadSurface->clip_rect;
		destRect = sourceRect;

	}

	std::string spSpriteDir = "";
	spSpriteDir = basePath.append("Assets\\circle.png");
	SDL_Surface* loadShootingPointSurface = IMG_Load(spSpriteDir.c_str());
	if (loadShootingPointSurface == NULL) std::cout << "Error Loading Image: " << IMG_GetError() << std::endl;
	else
	{
		SDL_ConvertSurfaceFormat(loadShootingPointSurface, SDL_PIXELFORMAT_RGBA8888, 0);

		shootingPoint.texture = SDL_CreateTextureFromSurface(renderer, loadShootingPointSurface);

		SDL_SetTextureBlendMode(shootingPoint.texture, SDL_BLENDMODE_BLEND);

		shootingPoint.sourceRect = loadShootingPointSurface->clip_rect;
		shootingPoint.destRect = shootingPoint.sourceRect;
	}


	//adjust the start x and y positions to have the object spawn in the centre of the mouse
	destRect.x = x - (loadSurface->w / 2);
	destRect.y = y - (loadSurface->h / 2);
	width = destRect.w;
	height = destRect.h;

	//free old surface
	SDL_FreeSurface(loadSurface);
	SDL_FreeSurface(loadShootingPointSurface);
	loadSurface = nullptr;
	loadShootingPointSurface = nullptr;

	//init the font
	defaultFont = EventManager::getInstance().defaultFont;

	justDropped = true;


	isClicked = false;
	isSelected = false;
	clickXOffset = 0;
	clickYOffset = 0;

	rotation = 0;

	lastFrameX = 0;
	lastFrameY = 0;

	eventWindow = nullptr;
	actionWindow = nullptr;
	doAnimationWindow = nullptr;
	relativeWindow = nullptr;
	addScoreWindow = nullptr;
	doSoundEffectWindow = nullptr;

	window = gameWindow;

	shootingPoint.isEnabled = false;

	bulletDir = "Assets\\bullet.png";

	eventBtnIndex = 10;

	vScrollBar = nullptr;
	SDL_SetTextureAlphaMod(shootingPoint.texture, 180);
}

GameObject::~GameObject()
{
	SDL_DestroyTexture(texture);
	int UISize = UIComponents.size();
	for (size_t i = 0; i < UISize; i++)
	{
		if (UIComponents[i])
		{
			delete UIComponents[i];
		}
	}
	if (vScrollBar) delete vScrollBar;
}

bool GameObject::IsUpdated()
{
	bool copy = isUpdated;
	if(isUpdated) isUpdated = false;

	return copy;
}

bool GameObject::IsUIUpdated()
{
	if (isUIUpdated)
	{
		isUIUpdated = false;
		return true;
	}
	return false;
}

bool GameObject::SetNewImgFromFile(std::string fileDir)
{
	if (fileDir != "")
	{
		std::string basePath = SDL_GetBasePath();
		spriteDir = fileDir.substr(basePath.size(), fileDir.size() - basePath.size());

		bool success = false;
		SDL_Surface* loadSurface = IMG_Load(fileDir.c_str());

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		if (loadSurface == NULL) std::cout << "Error Loading Image: " << IMG_GetError() << std::endl;
		else
		{
			success = true;
			SDL_ConvertSurfaceFormat(loadSurface, SDL_PIXELFORMAT_RGBA8888, 0);

			texture = SDL_CreateTextureFromSurface(renderer, loadSurface);

			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

			sourceRect = loadSurface->clip_rect;
			width = sourceRect.w;
			height = sourceRect.h;
			destRect = sourceRect;
		}

		return success;
	}
	return false;
}

bool GameObject::SetNewBulletImgFromFile(std::string fileDir)
{
	if (fileDir != "")
	{
		std::string basePath = SDL_GetBasePath();
		bulletDir = fileDir.substr(basePath.size(), fileDir.size() - basePath.size());

		bool success = false;
		SDL_Surface* loadSurface = IMG_Load(fileDir.c_str());

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		if (loadSurface == NULL) std::cout << "Error Loading Image: " << IMG_GetError() << std::endl;
		else
		{
			success = true;
			SDL_ConvertSurfaceFormat(loadSurface, SDL_PIXELFORMAT_RGBA8888, 0);

			shootingPoint.texture = SDL_CreateTextureFromSurface(renderer, loadSurface);

			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

			shootingPoint.sourceRect = loadSurface->clip_rect;
			shootingPoint.destRect = loadSurface->clip_rect;
			SDL_SetTextureAlphaMod(shootingPoint.texture, 180);
		}

		return success;
	}
	return false;
}

void GameObject::Update(SDL_Rect gameScreenRect)
{
	int mouseX, mouseY, screenX, screenY, screenWidth, screenHeight;
	SDL_GetMouseState(&mouseX, &mouseY);
	SDL_GetWindowPosition(window, &screenX, &screenY);
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);

	if (isClicked)
	{
		x = mouseX + clickXOffset;
		y = mouseY + clickYOffset;
		if (lastFrameX != x || lastFrameY != y)
			isUpdated = true;
	}

	modX = scrollBarOffset;

	if (justDropped)
	{
		x = x - destRect.w / 2;
		y = y - destRect.h / 2;

		justDropped = false;
		x = x - scrollBarOffset;
	}

	destRect.x = x + modX;
	destRect.y = y;
	destRect.w = width;
	destRect.h = height;

	lastFrameX = x;
	lastFrameY = y;





	for (size_t i = 0; i < UIComponents.size(); i++)
	{
		UIComponents[i]->Update(gameScreenRect);
	}



	//if (isClicked)
	//{
	//	switch (type)
	//	{
	//	case PLAYER:
	//		((UITextBox*)UIComponents[0])->SetText(name);//update name
	//		((UITextBox*)UIComponents[1])->SetText(to_string(x));//update X pos textbox
	//		((UITextBox*)UIComponents[2])->SetText(to_string(y));//update Y pos textbox
	//		((UITextBox*)UIComponents[3])->SetText(to_string(width));//update Width textbox
	//		((UITextBox*)UIComponents[4])->SetText(to_string(height));//update Height textbox
	//		((UITextBox*)UIComponents[5])->SetText(to_string(rotation));//update Rotation textbox
	//		break;
	//	case ENEMYWAVE:
	//		((UITextBox*)UIComponents[0])->SetText(name);//update name
	//		((UITextBox*)UIComponents[1])->SetText(to_string(x));//update X pos textbox
	//		((UITextBox*)UIComponents[2])->SetText(to_string(y));//update Y pos textbox
	//		((UITextBox*)UIComponents[3])->SetText(to_string(width));//update Width textbox
	//		((UITextBox*)UIComponents[4])->SetText(to_string(height));//update Height textbox
	//		((UITextBox*)UIComponents[5])->SetText(to_string(rotation));//update Rotation textbox
	//		break;
	//	case BACKGROUNDMUSIC:
	//		break;
	//	case BOSS:
	//		break;
	//	case LEVELOBJ:
	//		break;
	//	default:
	//		break;
	//	}
	//}

	FileDialog* newFileDialog = EventManager::getInstance()._FileDialog;

	//update 
	if (isSelected)
	{
		int windowWidth, windowHeight;
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
		if (UIComponents[eventBtnIndex]->GetY() + UIComponents[eventBtnIndex]->GetHeight() > windowHeight && vScrollBar == nullptr)
		{
			UIVScrollbar* elementScroller = new UIVScrollbar(windowWidth - (windowWidth / 8), 0, 10, windowHeight, renderer, window);
			UIComponents.emplace_back(elementScroller);
			elementScroller->SetMaxVal((UIComponents.size()) - 1);
			vScrollBar = elementScroller;
		}
		else if (vScrollBar)
		{
			vScrollBar->SetMaxVal(UIComponents.size() - 1);
			for (size_t i = 0; i < UIComponents.size(); i++)
			{
				if (UIComponents[i]->type != UIType::VScrollBar)
				{
					UIComponents[i]->setYOffset(vScrollBar->GetValue());
				}
			}
		}



		switch (type)
		{
		case PLAYER:
			if (((UITextBox*)UIComponents[0])->IsUpdated()) { name = ((UITextBox*)UIComponents[0])->GetText().c_str(); isUIUpdated = true; }//get info from object name textbox
			else if (((UITextBox*)UIComponents[1])->IsUpdated()) { 
				x = atoi(((UITextBox*)UIComponents[1])->GetText().c_str()); isUIUpdated = true; }//get info from x textbox
			else if (((UITextBox*)UIComponents[2])->IsUpdated()) { 
				y = atoi(((UITextBox*)UIComponents[2])->GetText().c_str()); isUIUpdated = true; }//get info from y textbox
			else if (((UITextBox*)UIComponents[3])->IsUpdated()) { width = atoi(((UITextBox*)UIComponents[3])->GetText().c_str()); isUIUpdated = true; }//get info from width textbox
			else if (((UITextBox*)UIComponents[4])->IsUpdated()) { height = atoi(((UITextBox*)UIComponents[4])->GetText().c_str()); isUIUpdated = true; }//get info from height textbox
			else if (((UITextBox*)UIComponents[5])->IsUpdated()) { rotation = atoi(((UITextBox*)UIComponents[5])->GetText().c_str()); isUIUpdated = true; }//get info from height textbox

			else if (((UIButton*)UIComponents[9])->CheckClick()) { SetNewImgFromFile(newFileDialog->GetFile()); isUIUpdated = true; }//if load sprite button is clicked, load new sprite
			else if (((UITextBox*)UIComponents[7])->IsUpdated()) { 
				shootingPoint.x = atoi(((UITextBox*)UIComponents[7])->GetText().c_str()); isUIUpdated = true; }
			else if (((UITextBox*)UIComponents[8])->IsUpdated()) {
				shootingPoint.y = atoi(((UITextBox*)UIComponents[8])->GetText().c_str()); isUIUpdated = true; }
			break;
		case ENEMYWAVE:
			if (((UITextBox*)UIComponents[0])->IsUpdated()) { name = ((UITextBox*)UIComponents[0])->GetText().c_str(); isUIUpdated = true; }//get info from object name textbox
			else if (((UITextBox*)UIComponents[1])->IsUpdated()) { x = atoi(((UITextBox*)UIComponents[1])->GetText().c_str()); isUIUpdated = true; }//get info from x textbox
			else if (((UITextBox*)UIComponents[2])->IsUpdated()) { y = atoi(((UITextBox*)UIComponents[2])->GetText().c_str()); isUIUpdated = true; }//get info from y textbox
			else if (((UITextBox*)UIComponents[3])->IsUpdated()) { width = atoi(((UITextBox*)UIComponents[3])->GetText().c_str()); isUIUpdated = true; }//get info from width textbox
			else if (((UITextBox*)UIComponents[4])->IsUpdated()) { height = atoi(((UITextBox*)UIComponents[4])->GetText().c_str()); isUIUpdated = true; }//get info from height textbox
			else if (((UITextBox*)UIComponents[5])->IsUpdated()) { rotation = atoi(((UITextBox*)UIComponents[5])->GetText().c_str()); isUIUpdated = true; }//get info from height textbox
			break;

		}
	}
}

void GameObject::SetClicked(bool inputBool)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	clickXOffset = x - mouseX;
	clickYOffset = y - mouseY;

	isClicked = inputBool;
}

void GameObject::AddComponent(UIElement * inputElement)
{
	UIComponents.emplace_back(inputElement);
}

void GameObject::ForceUpdateEvents()
{
	UIAddEvent(nullptr);
}

void GameObject::UIAddEvent(EXEventComponent * addComponent)//passing a nullptr simply updates the list
{
	//firstly, create a clone of the current UIComponent list
	vector<UIElement*> cloneList = UIComponents;
	int originalEventBtnIndex = eventBtnIndex;
	//delete elements of current list and place them in copy list
	UIComponents.clear();
	//start replacing elements
	for (size_t i = 0; i < eventBtnIndex; i++)
	{
		if (cloneList[i]->type == UIType::EventButton || cloneList[i]->type == ActionButton)
		{
			delete cloneList[i];
			eventBtnIndex--;
		}
		else
		{
			UIComponents.emplace_back(cloneList[i]);
		}
	}
	//now all elements prior to the event button have been replaced, we can add the new event, followed by any additional data afterwards

	if (addComponent)
	{
		events.push_back(addComponent);
	}

	//find the second lowest element, and get that position
	int lowestI = 0;
	int secondLowestI = 0;
	for (size_t i = 0; i < cloneList.size(); i++)
	{
		if (cloneList[i]->GetY() > cloneList[lowestI]->GetY() && cloneList[i]->type != EventButton && cloneList[i]->type != ActionButton)
		{
			secondLowestI = lowestI;
			lowestI = i;
		}
		else if (cloneList[i]->GetY() > cloneList[secondLowestI]->GetY() && cloneList[i]->type != EventButton && cloneList[i]->type != ActionButton)
		{
			secondLowestI = i;
		}

	}

	int borderSize = 10;

	int lastX, lastY, lastW, lastH;
	lastX = cloneList[secondLowestI]->GetX();
	lastY = cloneList[secondLowestI]->GetY();
	lastW = cloneList[secondLowestI]->GetWidth();
	lastH = cloneList[secondLowestI]->GetHeight();

	for (size_t i = 0; i < events.size(); i++)
	{
		if (events[i])
		{
			string btnText = "";
			switch (events[i]->type)
			{
			case EXEventComponentType::EXThisHitByBullet: btnText = "On Hit By Bullet";
				break;
			case EXEventComponentType::EXThisHitByEnemy: btnText = "On Hit By Enemy";
				break;
			case EXEventComponentType::EXThisHitByPlayer: btnText = "On Hit By Player";
				break;
			case EXEventComponentType::EXWhenMoveDown: btnText = "On Move Down";
				break;
			case EXEventComponentType::EXWhenMoveLeft: btnText = "On Move Left";
				break;
			case EXEventComponentType::EXWhenMoveRight: btnText = "On Move Right";
				break;
			case EXEventComponentType::EXWhenMoveUp: btnText = "On Move Up";
				break;
			case EXEventComponentType::EXWKeyPress: btnText = "On Key Press W";
				break;
			case EXEventComponentType::EXSKeyPress: btnText = "On Key Press S";
				break;
			case EXEventComponentType::EXAKeyPress: btnText = "On Key Press A";
				break;
			case EXEventComponentType::EXDKeyPress: btnText = "On Key Press D";
				break;
			case EXEventComponentType::EXSpaceKeyPress: btnText = "On Key Press Space";
				break;
			case EXEventComponentType::EXEveryFrame: btnText = "Every Frame";
				break;

			}

			UITextBox* tempBox = ((UITextBox*)cloneList[0]);

			UIButton* newBtn = new UIButton(tempBox->GetX(), lastY + lastH + borderSize, tempBox->GetWidth(), lastH, btnText, renderer, window, defaultFont);
			newBtn->ForceType(UIType::EventButton);
			newBtn->pEvent = events[i];
			newBtn->SetColour(SDL_Color{ 0,255,100,100 });
			AddComponent(newBtn);
			lastX = newBtn->GetX();
			lastY = newBtn->GetY();
			lastW = newBtn->GetWidth();
			lastH = newBtn->GetHeight();
			eventBtnIndex++;



			for (size_t j = 0; j < events[i]->actions.size(); j++)
			{
				EXActionComponentType temp2 = events[i]->actions[j]->type;
				///////////////////Now draw any Actions underneath the event button///////////////////
				switch (temp2)
				{
				case ActionComponentType::ADoAnimation: btnText = "Do Animation";
					break;
				case ActionComponentType::AMoveRelative: btnText = "Move Relative";
					break;
				case ActionComponentType::ASoundEffect: btnText = "Add Sound Effect";
					break;
				case ActionComponentType::AScoreLives: btnText = "Score and Lives";
					break;
				case ActionComponentType::AShoot: btnText = "Shoot";
					break;
				case ActionComponentType::AAddVelocity: btnText = "Add Velocity";
					break;
				case ActionComponentType::ASubtractVelocity: btnText = "Subtract Velocity";
					break;
				case ActionComponentType::AMultiplyVelocity: btnText = "Multiply Velocity";
					break;
				}
				UIButton* newBtn = new UIButton(lastX, lastY + lastH - 1, lastW, lastH, btnText, renderer, window, defaultFont);
				newBtn->ForceType(UIType::ActionButton);
				newBtn->SetColour(SDL_Color{ 100,50,80,100 });
				newBtn->pAction = events[i]->actions[j];
				AddComponent(newBtn);
				lastX = newBtn->GetX();
				lastY = newBtn->GetY();
				lastW = newBtn->GetWidth();
				lastH = newBtn->GetHeight();
				eventBtnIndex++;
			}
		}
	}

	//now add all additional info to the list
	for (size_t i = originalEventBtnIndex; i < cloneList.size(); i++)
	{
		if (cloneList[i]->type == Button && ((UIButton*)cloneList[i])->GetText() == "Add Event")
		{
			((UIButton*)cloneList[i])->SetYPos(lastY + lastH + borderSize);
			eventBtnIndex = UIComponents.size();
		}

		if (cloneList[i]->type == UIType::EventButton || cloneList[i]->type == UIType::ActionButton) 
		{
		}
		else {
			UIComponents.emplace_back(cloneList[i]);
		}
	}
}

void GameObject::ProcessWindows()
{
	int mouseX, mouseY, screenX, screenY, screenWidth, screenHeight;
	SDL_GetMouseState(&mouseX, &mouseY);
	SDL_GetWindowPosition(window, &screenX, &screenY);
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);


	for (size_t i = 0; i < UIComponents.size(); i++)
	{
		//manage actionwindow
		if (UIComponents[i]->type == UIType::EventButton && actionWindow == nullptr && ((UIButton*)UIComponents[i])->CheckClick())//if an eventbutton is clicked
		{
			actionWindow = new ActionSelectionWindow(screenX, screenY, screenWidth, screenHeight, ((UIButton*)UIComponents[i])->pEvent, defaultFont);
		}
		//manage relative window
		if (UIComponents[i]->type == UIType::ActionButton && relativeWindow == nullptr && ((UIButton*)UIComponents[i])->GetText() == "Move Relative" && ((UIButton*)UIComponents[i])->CheckClick())//if an eventbutton is clicked
		{
			relativeWindow = new RelativeWindow(screenX, screenY, screenWidth, screenHeight, ((UIButton*)UIComponents[i])->pEvent, ((UIButton*)UIComponents[i])->pAction, defaultFont);
		}
		//manage animation window
		if (UIComponents[i]->type == UIType::ActionButton && doAnimationWindow == nullptr && ((UIButton*)UIComponents[i])->GetText() == "Do Animation" && ((UIButton*)UIComponents[i])->CheckClick())//if an eventbutton is clicked
		{
			doAnimationWindow = new DoAnimationWindow(screenX, screenY, screenWidth, screenHeight, ((UIButton*)UIComponents[i])->pEvent, ((UIButton*)UIComponents[i])->pAction, defaultFont);
		}
		//manage animation window
		if (UIComponents[i]->type == UIType::ActionButton && addScoreWindow == nullptr && ((UIButton*)UIComponents[i])->GetText() == "Score and Lives" && ((UIButton*)UIComponents[i])->CheckClick())//if an eventbutton is clicked
		{
			addScoreWindow = new AddScoreWindow(screenX, screenY, screenWidth, screenHeight, ((UIButton*)UIComponents[i])->pEvent, ((UIButton*)UIComponents[i])->pAction, defaultFont);
		}
		//manage animation window
		if (UIComponents[i]->type == UIType::ActionButton && doSoundEffectWindow == nullptr && ((UIButton*)UIComponents[i])->GetText() == "Add Sound Effect" && ((UIButton*)UIComponents[i])->CheckClick())//if an eventbutton is clicked
		{
			doSoundEffectWindow = new DoSoundEffectWindow(screenX, screenY, screenWidth, screenHeight, ((UIButton*)UIComponents[i])->pEvent, ((UIButton*)UIComponents[i])->pAction, defaultFont);
		}
		//manage animation window
		if (UIComponents[i]->type == UIType::ActionButton && shootWindow == nullptr && ((UIButton*)UIComponents[i])->GetText() == "Shoot" && ((UIButton*)UIComponents[i])->CheckClick())//if an eventbutton is clicked
		{
			shootWindow = new ShootWindow(screenX, screenY, screenWidth, screenHeight, ((UIButton*)UIComponents[i])->pEvent, ((UIButton*)UIComponents[i])->pAction, defaultFont);
		}
		//manage animation window
		if (UIComponents[i]->type == UIType::ActionButton && addVelocityWindow == nullptr && ((UIButton*)UIComponents[i])->GetText() == "Add Velocity" && ((UIButton*)UIComponents[i])->CheckClick())//if an eventbutton is clicked
		{
			addVelocityWindow = new AddVelocityWindow(screenX, screenY, screenWidth, screenHeight, ((UIButton*)UIComponents[i])->pEvent, ((UIButton*)UIComponents[i])->pAction, defaultFont);
		}
		//manage animation window
		if (UIComponents[i]->type == UIType::ActionButton && subtractVelocityWindow == nullptr && ((UIButton*)UIComponents[i])->GetText() == "Subtract Velocity" && ((UIButton*)UIComponents[i])->CheckClick())//if an eventbutton is clicked
		{
			subtractVelocityWindow = new SubtractVelocityWindow(screenX, screenY, screenWidth, screenHeight, ((UIButton*)UIComponents[i])->pEvent, ((UIButton*)UIComponents[i])->pAction, defaultFont);
		}
		//manage animation window
		if (UIComponents[i]->type == UIType::ActionButton && multiplyVelocityWindow == nullptr && ((UIButton*)UIComponents[i])->GetText() == "Multiply Velocity" && ((UIButton*)UIComponents[i])->CheckClick())//if an eventbutton is clicked
		{
			multiplyVelocityWindow = new MultiplyVelocityWindow(screenX, screenY, screenWidth, screenHeight, ((UIButton*)UIComponents[i])->pEvent, ((UIButton*)UIComponents[i])->pAction, defaultFont);
		}
	}



	if (((UIButton*)UIComponents[eventBtnIndex])->CheckClick() && eventWindow == nullptr)//if add event button is clicked
	{
		eventWindow = new EventSelectionWindow(screenX, screenY, screenWidth, screenHeight, defaultFont);
	}

	if (eventWindow)
	{
		eventWindow->Update();
		int checkVal = eventWindow->CheckSelected();
		if (checkVal == -1 || checkVal == 1)
		{
			if (checkVal == 1)//1 = successful selection
			{
				UIAddEvent(eventWindow->returnItem);
			}
			delete eventWindow;
			eventWindow = nullptr;
		}
	}





	if (actionWindow)
	{
		actionWindow->Update();
		int selectedVal = actionWindow->CheckSelected();
		if (selectedVal == -1 || selectedVal == 1 || selectedVal == 2)
		{
			if (selectedVal == 2)//2 = user pressed delete
			{
				for (size_t i = 0; i < UIComponents.size(); i++)
				{
					if (UIComponents[i]->type == UIType::EventButton && ((UIButton*)UIComponents[i])->pEvent == actionWindow->pEvent)
					{
						UIComponents.erase(UIComponents.begin() + i);
						break;
					}
				}
				for (size_t i = 0; i < events.size(); i++)
				{
					if (events[i] == actionWindow->pEvent)
					{
						events.erase(events.begin() + i);
					}
				}
				eventBtnIndex--;
			}
			UIAddEvent(nullptr);//force an update to the component list
			delete actionWindow;
			actionWindow = nullptr;
		}
	}
	if (doAnimationWindow)
	{
		doAnimationWindow->Update();
		int selectedVal = doAnimationWindow->CheckSelected();
		if (selectedVal == -1 || selectedVal == 1 || selectedVal == 2)
		{
			if (selectedVal == 2)//2 = user pressed delete
			{
				for (size_t i = 0; i < UIComponents.size(); i++)
				{
					if (UIComponents[i]->type == UIType::ActionButton && ((UIButton*)UIComponents[i])->pAction == doAnimationWindow->pAction)
					{
						UIComponents.erase(UIComponents.begin() + i);
						break;
					}
				}
				for (size_t i = 0; i < events.size(); i++)
				{
					for (size_t k = 0; k < events[i]->actions.size(); k++)
					{
						if (events[i]->actions[k] == doAnimationWindow->pAction)
						{
							events[i]->actions.erase(events[i]->actions.begin() + k);
						}
					}
					eventBtnIndex--;
				}
			}
			UIAddEvent(nullptr);//force an update to the component list
			delete doAnimationWindow;
			doAnimationWindow = nullptr;
		}
	}

	if (relativeWindow)
	{
		relativeWindow->Update();
		int selectedVal = relativeWindow->CheckSelected();
		if (selectedVal == -1 || selectedVal == 1 || selectedVal == 2)
		{
			if (selectedVal == 2)//2 = user pressed delete
			{
				for (size_t i = 0; i < UIComponents.size(); i++)
				{
					if (UIComponents[i]->type == UIType::ActionButton && ((UIButton*)UIComponents[i])->pAction == relativeWindow->pAction)
					{
						UIComponents.erase(UIComponents.begin() + i);
						break;
					}
				}
				for (size_t i = 0; i < events.size(); i++)
				{
					for (size_t k = 0; k < events[i]->actions.size(); k++)
					{
						if (events[i]->actions[k] == relativeWindow->pAction)
						{
							events[i]->actions.erase(events[i]->actions.begin() + k);
						}
					}
					eventBtnIndex--;
				}
			}
			UIAddEvent(nullptr);//force an update to the component list
			delete relativeWindow;
			relativeWindow = nullptr;
		}
	}

	if (addScoreWindow)
	{
		addScoreWindow->Update();
		int selectedVal = addScoreWindow->CheckSelected();
		if (selectedVal == -1 || selectedVal == 1 || selectedVal == 2)
		{
			if (selectedVal == 2)//2 = user pressed delete
			{
				for (size_t i = 0; i < UIComponents.size(); i++)
				{
					if (UIComponents[i]->type == UIType::ActionButton && ((UIButton*)UIComponents[i])->pAction == addScoreWindow->pAction)
					{
						UIComponents.erase(UIComponents.begin() + i);
						break;
					}
				}
				for (size_t i = 0; i < events.size(); i++)
				{
					for (size_t k = 0; k < events[i]->actions.size(); k++)
					{
						if (events[i]->actions[k] == addScoreWindow->pAction)
						{
							events[i]->actions.erase(events[i]->actions.begin() + k);
						}
					}
					eventBtnIndex--;
				}
			}
			UIAddEvent(nullptr);//force an update to the component list
			delete addScoreWindow;
			addScoreWindow = nullptr;
		}
	}
	if (doSoundEffectWindow)
	{
		doSoundEffectWindow->Update();
		int selectedVal = doSoundEffectWindow->CheckSelected();
		if (selectedVal == -1 || selectedVal == 1 || selectedVal == 2)
		{
			if (selectedVal == 2)//2 = user pressed delete
			{
				for (size_t i = 0; i < UIComponents.size(); i++)
				{
					if (UIComponents[i]->type == UIType::ActionButton && ((UIButton*)UIComponents[i])->pAction == doSoundEffectWindow->pAction)
					{
						UIComponents.erase(UIComponents.begin() + i);
						break;
					}
				}
				for (size_t i = 0; i < events.size(); i++)
				{
					for (size_t k = 0; k < events[i]->actions.size(); k++)
					{
						if (events[i]->actions[k] == doSoundEffectWindow->pAction)
						{
							events[i]->actions.erase(events[i]->actions.begin() + k);
						}
					}
					eventBtnIndex--;
				}
			}
			UIAddEvent(nullptr);//force an update to the component list
			delete doSoundEffectWindow;
			doSoundEffectWindow = nullptr;
		}
	}
	if (shootWindow)
	{
		shootWindow->Update();
		int selectedVal = shootWindow->CheckSelected();
		if (selectedVal == -1 || selectedVal == 1 || selectedVal == 2)
		{
			if (selectedVal == 2)//2 = user pressed delete
			{
				for (size_t i = 0; i < UIComponents.size(); i++)
				{
					if (UIComponents[i]->type == UIType::ActionButton && ((UIButton*)UIComponents[i])->pAction == shootWindow->pAction)
					{
						UIComponents.erase(UIComponents.begin() + i);
						break;
					}
				}
				for (size_t i = 0; i < events.size(); i++)
				{
					for (size_t k = 0; k < events[i]->actions.size(); k++)
					{
						if (events[i]->actions[k] == shootWindow->pAction)
						{
							events[i]->actions.erase(events[i]->actions.begin() + k);
						}
					}
					eventBtnIndex--;
				}
			}
			UIAddEvent(nullptr);//force an update to the component list
			delete shootWindow;
			shootWindow = nullptr;
		}
	}

	if (addVelocityWindow)
	{
		addVelocityWindow->Update();
		int selectedVal = addVelocityWindow->CheckSelected();
		if (selectedVal == -1 || selectedVal == 1 || selectedVal == 2)
		{
			if (selectedVal == 2)//2 = user pressed delete
			{
				for (size_t i = 0; i < UIComponents.size(); i++)
				{
					if (UIComponents[i]->type == UIType::ActionButton && ((UIButton*)UIComponents[i])->pAction == addVelocityWindow->pAction)
					{
						UIComponents.erase(UIComponents.begin() + i);
						break;
					}
				}
				for (size_t i = 0; i < events.size(); i++)
				{
					for (size_t k = 0; k < events[i]->actions.size(); k++)
					{
						if (events[i]->actions[k] == addVelocityWindow->pAction)
						{
							events[i]->actions.erase(events[i]->actions.begin() + k);
						}
					}
					eventBtnIndex--;
				}
			}
			UIAddEvent(nullptr);//force an update to the component list
			delete addVelocityWindow;
			addVelocityWindow = nullptr;
		}
	}

	if (subtractVelocityWindow)
	{
		subtractVelocityWindow->Update();
		int selectedVal = subtractVelocityWindow->CheckSelected();
		if (selectedVal == -1 || selectedVal == 1 || selectedVal == 2)
		{
			if (selectedVal == 2)//2 = user pressed delete
			{
				for (size_t i = 0; i < UIComponents.size(); i++)
				{
					if (UIComponents[i]->type == UIType::ActionButton && ((UIButton*)UIComponents[i])->pAction == subtractVelocityWindow->pAction)
					{
						UIComponents.erase(UIComponents.begin() + i);
						break;
					}
				}
				for (size_t i = 0; i < events.size(); i++)
				{
					for (size_t k = 0; k < events[i]->actions.size(); k++)
					{
						if (events[i]->actions[k] == subtractVelocityWindow->pAction)
						{
							events[i]->actions.erase(events[i]->actions.begin() + k);
						}
					}
					eventBtnIndex--;
				}
			}
			UIAddEvent(nullptr);//force an update to the component list
			delete subtractVelocityWindow;
			subtractVelocityWindow = nullptr;
		}
	}

	if (multiplyVelocityWindow)
	{
		multiplyVelocityWindow->Update();
		int selectedVal = multiplyVelocityWindow->CheckSelected();
		if (selectedVal == -1 || selectedVal == 1 || selectedVal == 2)
		{
			if (selectedVal == 2)//2 = user pressed delete
			{
				for (size_t i = 0; i < UIComponents.size(); i++)
				{
					if (UIComponents[i]->type == UIType::ActionButton && ((UIButton*)UIComponents[i])->pAction == multiplyVelocityWindow->pAction)
					{
						UIComponents.erase(UIComponents.begin() + i);
						break;
					}
				}
				for (size_t i = 0; i < events.size(); i++)
				{
					for (size_t k = 0; k < events[i]->actions.size(); k++)
					{
						if (events[i]->actions[k] == multiplyVelocityWindow->pAction)
						{
							events[i]->actions.erase(events[i]->actions.begin() + k);
						}
					}
					eventBtnIndex--;
				}
			}
			UIAddEvent(nullptr);//force an update to the component list
			delete multiplyVelocityWindow;
			multiplyVelocityWindow = nullptr;
		}
	}


}


void GameObject::Draw(int scrollBarOffset)
{
	SDL_Rect offsetRect;//this rectangle will take into consideration how far along the scrollbar is.
	offsetRect = destRect;
	offsetRect.x = offsetRect.x + modX;

	if (isClicked)
	{
		SDL_SetTextureAlphaMod(texture, 70);
		//below code will draw a rect around the sprite when it is being dragged
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 130);
		SDL_Rect outlineRect = { destRect.x - 1, destRect.y - 1, destRect.w + 2, destRect.h + 2 };
		SDL_RenderDrawRect(renderer, &outlineRect);
	}
	else
	{
		SDL_SetTextureAlphaMod(texture, 255);
	}
	SDL_Point centre = { width/2, height/2 };
	SDL_RenderCopyEx(renderer, texture, NULL, &destRect, rotation, &centre, SDL_RendererFlip::SDL_FLIP_NONE);

	if (shootingPoint.texture && isSelected && !isClicked && type == GameObjectType::PLAYER)
	{
		//shootingPoint.destRect.w = 5;
		//shootingPoint.destRect.h = 5;
		shootingPoint.destRect.x = x + shootingPoint.x - (shootingPoint.destRect.w / 2) - 10;
		shootingPoint.destRect.y = y + shootingPoint.y - (shootingPoint.destRect.h / 2);
		SDL_Rect tempRect = shootingPoint.destRect;
		SDL_RenderCopy(renderer, shootingPoint.texture, &shootingPoint.sourceRect, &tempRect);
	}

}

void GameObject::DrawUI()
{
	for (size_t i = 0; i < UIComponents.size(); i++)
	{
		UIComponents[i]->Draw();
	}

	if (eventWindow) eventWindow->Draw();
	if (actionWindow) actionWindow->Draw();
	if (doAnimationWindow) doAnimationWindow->Draw();
	if (relativeWindow) relativeWindow->Draw();
	if (addScoreWindow) addScoreWindow->Draw();
	if (doSoundEffectWindow) doSoundEffectWindow->Draw();
	if (shootWindow) shootWindow->Draw();
	if (addVelocityWindow) addVelocityWindow->Draw();
	if (subtractVelocityWindow) subtractVelocityWindow->Draw();
	if (multiplyVelocityWindow) multiplyVelocityWindow->Draw();
}
