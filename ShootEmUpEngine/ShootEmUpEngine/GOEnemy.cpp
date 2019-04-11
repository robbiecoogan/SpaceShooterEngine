#include "GameObject.h"


void GOEnemy::Update(SDL_Rect gameScreenRect)
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

		activeScreenRect = gameScreenRect;

		x = x - scrollBarOffset;
	}

	destRect.x = x + modX;
	destRect.y = y;
	destRect.w = width;
	destRect.h = height;

	lastFrameX = x;
	lastFrameY = y;


	//if (((UIButton*)UIComponents[eventBtnIndex])->CheckClick() && eventWindow == nullptr)//if add event button is clicked
	//{
	//	eventWindow = new EventSelectionWindow(screenX, screenY, screenWidth, screenHeight, defaultFont);
	//}

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


	for (size_t i = 0; i < UIComponents.size(); i++)
	{
		UIComponents[i]->Update(gameScreenRect);

		//manage actionwindow
		if (UIComponents[i]->type == UIType::EventButton && actionWindow == nullptr && ((UIButton*)UIComponents[i])->CheckClick())//if an eventbutton is clicked
		{
			actionWindow = new ActionSelectionWindow(screenX, screenY, screenWidth, screenHeight, ((UIButton*)UIComponents[i])->pEvent, defaultFont);
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

	if (isClicked)
	{
		((UITextBox*)UIComponents[0])->SetText(name);//update name
		((UITextBox*)UIComponents[1])->SetText(to_string(x));//update X pos textbox
		((UITextBox*)UIComponents[2])->SetText(to_string(y));//update Y pos textbox
		((UITextBox*)UIComponents[3])->SetText(to_string(width));//update Width textbox
		((UITextBox*)UIComponents[4])->SetText(to_string(height));//update Height textbox
		((UITextBox*)UIComponents[5])->SetText(to_string(rotation));//update Rotation textbox
		((UITextBox*)UIComponents[6])->SetText(to_string(XVec));//update moveVec X textbox
		((UITextBox*)UIComponents[7])->SetText(to_string(YVec));//update moveVec X textbox

	}
	FileDialog* newFileDialog = new FileDialog();
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

		name = name;

		if (((UITextBox*)UIComponents[0])->IsUpdated()) { name = ((UITextBox*)UIComponents[0])->GetText().c_str(); isUIUpdated = true; }//get info from object name textbox
		else if (((UITextBox*)UIComponents[1])->IsUpdated()) { x = atoi(((UITextBox*)UIComponents[1])->GetText().c_str()); isUIUpdated = true; }//get info from x textbox
		else if (((UITextBox*)UIComponents[2])->IsUpdated()) { y = atoi(((UITextBox*)UIComponents[2])->GetText().c_str()); isUIUpdated = true; }//get info from y textbox
		else if (((UITextBox*)UIComponents[3])->IsUpdated()) { width = atoi(((UITextBox*)UIComponents[3])->GetText().c_str()); isUIUpdated = true; }//get info from width textbox
		else if (((UITextBox*)UIComponents[4])->IsUpdated()) { height = atoi(((UITextBox*)UIComponents[4])->GetText().c_str()); isUIUpdated = true; }//get info from height textbox
		else if (((UITextBox*)UIComponents[5])->IsUpdated()) { rotation = atoi(((UITextBox*)UIComponents[5])->GetText().c_str()); isUIUpdated = true; }//get info from rotation textbox
		else if (((UITextBox*)UIComponents[6])->IsUpdated() && ((UITextBox*)UIComponents[6])->GetText() != "" && ((UITextBox*)UIComponents[6])->GetText() != "-") { XVec = stof(((UITextBox*)UIComponents[6])->GetText().c_str()); isUIUpdated = true; }//get info from xvec textbox
		else if (((UITextBox*)UIComponents[7])->IsUpdated() && ((UITextBox*)UIComponents[7])->GetText() != "" && ((UITextBox*)UIComponents[7])->GetText() != "-") { YVec = stof(((UITextBox*)UIComponents[7])->GetText().c_str()); isUIUpdated = true; }//get info from yvec textbox
	}

	delete newFileDialog;
}
void GOEnemy::Draw(int IscrollBarOffset)
{
	scrollBarOffset = IscrollBarOffset;
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
	SDL_Point centre = { width / 2, height / 2 };
	SDL_RenderCopyEx(renderer, texture, NULL, &destRect, rotation, &centre, SDL_RendererFlip::SDL_FLIP_NONE);

	if (shootingPoint.texture && isSelected && !isClicked && type == GameObjectType::PLAYER)
	{
		shootingPoint.destRect.w = 5;
		shootingPoint.destRect.h = 5;
		shootingPoint.destRect.x = x + shootingPoint.x - (shootingPoint.destRect.w / 2) - 10;
		shootingPoint.destRect.y = y + shootingPoint.y - (shootingPoint.destRect.h / 2);
		SDL_RenderCopy(renderer, shootingPoint.texture, &shootingPoint.sourceRect, &shootingPoint.destRect);
	}
}