#include "UIElement.h"
#include "Common.h"
#include "GameObject.h"

UISelectionBox::UISelectionBox(int inputX, int inputY, int inputWidth, int inputHeight, SDL_Renderer* gameRenderer, SDL_Window* gameWindow, TTF_Font* font, SDL_Rect gameScreenRect) : renderer(gameRenderer), window(gameWindow), defaultFont(font)
{
	x = inputX;
	y = inputY;
	width = inputWidth;
	height = inputHeight;

	type = UIType::SelectionBox;

	windowRect = SDL_Rect{ inputX, inputY, inputWidth, inputHeight };
	activeScreenRect = gameScreenRect;

	defaultFont = font;

	Init();
}

UISelectionBox::~UISelectionBox()
{
	for (const auto element : items)
	{
		delete element;
	}
}

void UISelectionBox::Init()
{
	//initialize the back rectangle
	backRect = { x, y, width, height };

	//init back and outline colours
	backColor1 = { 70, 70, 70, 255 };
	backColor2 = { 120, 120, 120, 255 };
	backColor3 = { 100, 100, 100, 255 };
	outlineColor = { 0, 0, 0, 255 };

	//initialize text color
	textColor = { 50, 50, 50, 255 };
	highlightColor = { 49, 112, 214, 255 };	

  	selectedID = -1;//-1 identifies that nothing is selected.

	Alignment = 0;//default alignment is left alignment

	newSelected = false;
	prevFrameSelected = -1;
}

void UISelectionBox::AddItem(SelectionBoxItem* item)
{
	items.push_back(item);

	selectedID = items.size() - 1;
	if (items[selectedID]->isGameObject)
	{
		items[selectedID]->gameObject->isSelected = true;
		items[selectedID]->gameObject->SetClicked(true);
		items[selectedID]->gameObject->Update(activeScreenRect);
		items[selectedID]->gameObject->SetClicked(false);
	}
	//if (item->isGameObject)
	//{
	//	for (size_t i = 0; i < items.size(); i++)
	//	{
	//		items[i]->gameObject->SetClicked(false);
	//	}
	//	item->gameObject->SetClicked(false);

	//}
}

void UISelectionBox::RemoveAt(int index)
{
	if (index >= 0)
	{
		delete items[index];
		items.erase(items.begin() + index, items.begin() + index + 1);
		
		if (index == selectedID) { selectedID--; newSelected = true; }
		if (selectedID <= -1 && items.size() >= 1) { selectedID = 0; newSelected = true;}
		
	}
}

int UISelectionBox::GetSelectedIndex()
{
	return selectedID;
}

bool UISelectionBox::NewSelected()
{
	bool returnVal = newSelected;
	if (newSelected) 
		newSelected = false;

	return returnVal;
}

void UISelectionBox::ForceNewSelected()
{
	newSelected = true;
}

SelectionBoxItem* UISelectionBox::CheckSelected(int inputX, int inputY)
{
	bool foundSelected = false;//this will turn true if any element is found to be selected, otherwise, we can turn selectedID to -1
	SDL_Surface* drawText = TTF_RenderText_Solid(defaultFont, "Demo", textColor);//This surface is only generated to allow precise access to the width and height of the text

	if (inputX > x && inputY > y && inputX < (x + width) && inputY < (y + height))//if the mouse is within the box to begin with
	{
		for (int i = 0; i < items.size(); i++)
		{
			if (inputY > (y - scrollOffset + drawText->h * i) && inputY <= y - scrollOffset + drawText->h * (i+1) && i != selectedID)
			{
				selectedID = i;
				newSelected = true;
				foundSelected = true;

				if (items[i]->isGameObject)
				{
					for (size_t k = 0; k < items.size(); k++)
					{
						if (k != selectedID)
						{
							items[k]->gameObject->isSelected = false;
							items[k]->gameObject->isClicked = false;
						}
					}
					items[i]->gameObject->isSelected = true;
					items[i]->gameObject->SetClicked(true);
					items[i]->gameObject->Update(activeScreenRect);
					items[i]->gameObject->SetClicked(false);
				}

				break;
			}
		}
	}
	//free the surface
	SDL_FreeSurface(drawText);
	drawText = nullptr;

	prevFrameSelected = selectedID;

	if (selectedID == -1)
	{
		return nullptr;
	}
	else
	{
		return items[selectedID];
	}
	
}

SelectionBoxItem* UISelectionBox::GetSelectedItem()
{
	if (selectedID > -1 && items.size() > selectedID && items[selectedID])
	{
		return items[selectedID];
	}
		return nullptr;
}

void UISelectionBox::SetAlignment(int inAlign)
{
	Alignment = inAlign;
}


void UISelectionBox::Update(SDL_Rect activeScreenRect)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);




	//Update any gameobjects contained within
	for (int i = items.size()-1; i >= 0; i--)//start at end to ensure that the top most object will be selected
	{

		if (items[i]->isGameObject)
		{
			GameObject* tempCast = items[i]->gameObject;
			int goX, goY, goW, goH;//gameobject x, y, width, and height.
			goX = tempCast->destRect.x;
			goY = tempCast->destRect.y;
			goW = tempCast->width;
			goH = tempCast->height;

			std::vector<SDL_Event*> tempE = EventManager::getInstance().GetEvent(SDL_MOUSEBUTTONDOWN);

			if (tempE.size() > 0)
			{
				if (SDL_GetMouseState(NULL, NULL) == SDL_BUTTON(SDL_BUTTON_LEFT) && mouseX > goX && mouseX < goX + goW && mouseY > goY + yOffset && mouseY < goY + goH + yOffset && tempE[0]->window.windowID == SDL_GetWindowID(window) && mouseX < activeScreenRect.x + activeScreenRect.w)
				{
					if (!tempCast->isClicked)
					{
						tempCast->SetClicked(true);
						selectedID = i;
						newSelected = true;
						tempCast->Update(this->activeScreenRect);
					}
				}
			}
			tempE = EventManager::getInstance().GetEvent(SDL_MOUSEBUTTONUP);
			if (tempE.size() > 0)
			{
				for (size_t k = 0; k < items.size(); k++)
				{
					if (k != selectedID)
					{
						items[k]->gameObject->isSelected = false;
						items[k]->gameObject->isClicked = false;
					}
				}
				if (tempCast->isClicked)
				{
					tempCast->SetClicked(false);
					tempCast->isSelected = true;
				}
			}

			tempCast->Update(activeScreenRect);

			if (tempCast->IsUpdated())
			{
				selectedID = i;
				newSelected = true;
			}
			if (tempCast->IsUIUpdated())
			{
				items[i]->name = items[i]->gameObject->name;
			}

			if (tempCast->isClicked) break;
		}
	}
}

int UISelectionBox::GetItemCount()
{
	return items.size();
}

void UISelectionBox::Draw()
{
	if (selectedID > -1 && items[selectedID]->isGameObject && items[selectedID]->gameObject->type != GameObjectType::BACKGROUND)
	{
		GameObject* tempCast = (GameObject*)items[selectedID]->gameObject;
		SDL_SetRenderDrawColor(renderer, 50, 50, 255, 255);//set color to highlight color
		SDL_Rect outline = tempCast->destRect;
		outline.x -= 1; outline.y -= 1; outline.w += 2; outline.h += 2;
		outline.y += yOffset;
		SDL_RenderDrawRect(renderer, &tempCast->destRect);//draw outline
	}

	SDL_Rect tempRect = backRect;
	tempRect.y += yOffset;
	SDL_SetRenderDrawColor(renderer, backColor1.r, backColor1.g, backColor1.b, backColor1.a);//set color to fill color
	SDL_RenderFillRect(renderer, &tempRect);//Draw background rectangle

	SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);//set color to outline color
	SDL_RenderDrawRect(renderer, &tempRect);//draw outline

	//for each item that it contains, draw the name
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->name == "") items[i]->name = " ";
		SDL_Surface* drawText = TTF_RenderText_Solid(defaultFont, items[i]->name.c_str(), textColor);//create a surface using text & font values
		SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, drawText);//convert the surface to a texture so it can be drawn



		//if we are about to draw below our back square, break the loop to prevent drawing past our boundary
		if ((drawText->h * (i + 1) - scrollOffset) > backRect.h + yOffset)
		{
			SDL_FreeSurface(drawText);
			drawText = nullptr;
			SDL_DestroyTexture(textTex);
			textTex = nullptr;
			break;
		}

		if (!((backRect.y + (drawText->h * i)) - scrollOffset + 1 < backRect.y + yOffset))//if the text is above the top of the box, don't draw it.
		{
			//every 2 items, back color will change to list items better
			if (i % 2 == 0)
			{
				SDL_SetRenderDrawColor(renderer, backColor3.r, backColor3.g, backColor3.b, backColor3.a);
				SDL_Rect tempBackRect = { backRect.x + 1, backRect.y + (drawText->h * i) + 1 - scrollOffset + yOffset, backRect.w - 2, drawText->h - 2 };
				SDL_RenderFillRect(renderer, &tempBackRect);//Draw rectangle behind current text
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, backColor2.r, backColor2.g, backColor2.b, backColor2.a);
				SDL_Rect tempBackRect = { backRect.x + 1, backRect.y + (drawText->h * i) + 1 - scrollOffset + yOffset, backRect.w - 2, drawText->h - 2 };
				SDL_RenderFillRect(renderer, &tempBackRect);//Draw rectangle behind current text
			}


			SDL_Rect textRect = { backRect.x + 5, backRect.y + (drawText->h * i) - scrollOffset + yOffset, (drawText != nullptr) ? drawText->w : 0, drawText->h };//default to left alighment
			//dependent on the alignment, the text will be drawn in different locations
			if (Alignment == 0)// = LEFT
			{
				//already set to left
			}
			else if (Alignment == 1)// = CENTRE
			{
				textRect = { (backRect.x + (backRect.w / 2) - drawText->w / 2), backRect.y + (drawText->h * i) + yOffset, (drawText != nullptr) ? drawText->w : 0, drawText->h };
			}
			else//2 or otherwise = RIGHT
			{
				textRect = { (backRect.x + backRect.w - drawText->w) - 5, backRect.y + (drawText->h * i) + yOffset, (drawText != nullptr) ? drawText->w : 0, drawText->h };
			}


			//check if the current element is selected, as we will need to draw a highlight behind it if this is the case
			if (selectedID == i)
			{
				SDL_Rect highlightRect = { backRect.x + 1, backRect.y - scrollOffset + (drawText->h * i) + 1 + yOffset, backRect.w - 2, drawText->h - 2 };
				SDL_SetRenderDrawColor(renderer, highlightColor.r, highlightColor.g, highlightColor.b, highlightColor.a);
				SDL_RenderFillRect(renderer, &highlightRect);//draw highlight
			}


			//draw the text (this needs to be done after the highlight, but initialization is done before to allow access to the scale of the SDL Surface width and height)
			SDL_RenderCopy(renderer, textTex, NULL, &textRect);

			//free the data when used
			SDL_FreeSurface(drawText);
			drawText = nullptr;
			SDL_DestroyTexture(textTex);
			textTex = nullptr;
		}
		else
		{
			SDL_FreeSurface(drawText);
			drawText = nullptr;
			SDL_DestroyTexture(textTex);
			textTex = nullptr;
		}
	}

}

void UISelectionBox::Draw(bool drawGOOutLine)
{
	if (selectedID > -1 && items[selectedID]->isGameObject && items[selectedID]->gameObject->type != GameObjectType::BACKGROUND && drawGOOutLine)
	{
		GameObject* tempCast = (GameObject*)items[selectedID]->gameObject;
		SDL_SetRenderDrawColor(renderer, 50, 50, 255, 255);//set color to highlight color
		SDL_Rect outline = tempCast->destRect;
		outline.x -= 1; outline.y -= 1; outline.w += 2; outline.h += 2;
		outline.y += yOffset;
		SDL_RenderDrawRect(renderer, &tempCast->destRect);//draw outline
	}

	SDL_Rect tempRect = backRect;
	tempRect.y += yOffset;
	SDL_SetRenderDrawColor(renderer, backColor1.r, backColor1.g, backColor1.b, backColor1.a);//set color to fill color
	SDL_RenderFillRect(renderer, &tempRect);//Draw background rectangle

	SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);//set color to outline color
	SDL_RenderDrawRect(renderer, &tempRect);//draw outline

											//for each item that it contains, draw the name
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->name == "") items[i]->name = " ";
		SDL_Surface* drawText = TTF_RenderText_Solid(defaultFont, items[i]->name.c_str(), textColor);//create a surface using text & font values
		SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, drawText);//convert the surface to a texture so it can be drawn



																				//if we are about to draw below our back square, break the loop to prevent drawing past our boundary
		if ((drawText->h * (i + 1) - scrollOffset) > backRect.h + yOffset)
		{
			SDL_FreeSurface(drawText);
			drawText = nullptr;
			SDL_DestroyTexture(textTex);
			textTex = nullptr;
			break;
		}

		if (!((backRect.y + (drawText->h * i)) - scrollOffset + 1 < backRect.y + yOffset))//if the text is above the top of the box, don't draw it.
		{
			//every 2 items, back color will change to list items better
			if (i % 2 == 0)
			{
				SDL_SetRenderDrawColor(renderer, backColor3.r, backColor3.g, backColor3.b, backColor3.a);
				SDL_Rect tempBackRect = { backRect.x + 1, backRect.y + (drawText->h * i) + 1 - scrollOffset + yOffset, backRect.w - 2, drawText->h - 2 };
				SDL_RenderFillRect(renderer, &tempBackRect);//Draw rectangle behind current text
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, backColor2.r, backColor2.g, backColor2.b, backColor2.a);
				SDL_Rect tempBackRect = { backRect.x + 1, backRect.y + (drawText->h * i) + 1 - scrollOffset + yOffset, backRect.w - 2, drawText->h - 2 };
				SDL_RenderFillRect(renderer, &tempBackRect);//Draw rectangle behind current text
			}


			SDL_Rect textRect = { backRect.x + 5, backRect.y + (drawText->h * i) - scrollOffset + yOffset, (drawText != nullptr) ? drawText->w : 0, drawText->h };//default to left alighment
																																								  //dependent on the alignment, the text will be drawn in different locations
			if (Alignment == 0)// = LEFT
			{
				//already set to left
			}
			else if (Alignment == 1)// = CENTRE
			{
				textRect = { (backRect.x + (backRect.w / 2) - drawText->w / 2), backRect.y + (drawText->h * i) + yOffset, (drawText != nullptr) ? drawText->w : 0, drawText->h };
			}
			else//2 or otherwise = RIGHT
			{
				textRect = { (backRect.x + backRect.w - drawText->w) - 5, backRect.y + (drawText->h * i) + yOffset, (drawText != nullptr) ? drawText->w : 0, drawText->h };
			}


			//check if the current element is selected, as we will need to draw a highlight behind it if this is the case
			if (selectedID == i)
			{
				SDL_Rect highlightRect = { backRect.x + 1, backRect.y - scrollOffset + (drawText->h * i) + 1 + yOffset, backRect.w - 2, drawText->h - 2 };
				SDL_SetRenderDrawColor(renderer, highlightColor.r, highlightColor.g, highlightColor.b, highlightColor.a);
				SDL_RenderFillRect(renderer, &highlightRect);//draw highlight
			}


			//draw the text (this needs to be done after the highlight, but initialization is done before to allow access to the scale of the SDL Surface width and height)
			SDL_RenderCopy(renderer, textTex, NULL, &textRect);

			//free the data when used
			SDL_FreeSurface(drawText);
			drawText = nullptr;
			SDL_DestroyTexture(textTex);
			textTex = nullptr;
		}
		else
		{
			SDL_FreeSurface(drawText);
			drawText = nullptr;
			SDL_DestroyTexture(textTex);
			textTex = nullptr;
		}
	}
}
