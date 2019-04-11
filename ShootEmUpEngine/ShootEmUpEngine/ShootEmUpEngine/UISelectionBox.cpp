#include "UISelectionBox.h"

UISelectionBox::UISelectionBox(int inputX, int inputY, int inputWidth, int inputHeight, SDL_Renderer* gameRenderer, TTF_Font* font) : x(inputX), y(inputY), width(inputWidth), height(inputHeight), renderer(gameRenderer), defaultFont(font)
{
	Init();
}

UISelectionBox::~UISelectionBox()
{

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

}

void UISelectionBox::AddItem(SelectionBoxItem* item)
{
	items.push_back(item);
}

void UISelectionBox::RemoveAt(int index)
{
	if (index >= 0)
	{
		items.erase(items.begin() + index, items.begin() + index + 1);
		
		if (index == selectedID) selectedID--;
		if (selectedID <= -1 && items.size() >= 1) selectedID = 0;
		
	}
}

int UISelectionBox::GetSelectedIndex()
{
	return selectedID;
}

SelectionBoxItem* UISelectionBox::CheckSelected(int inputX, int inputY)
{
	bool foundSelected = false;//this will turn true if any element is found to be selected, otherwise, we can turn selectedID to -1
	SDL_Surface* drawText = TTF_RenderText_Solid(defaultFont, "Demo", textColor);//This surface is only generated to allow precise access to the width and height of the text

	if (inputX > x && inputY > y && inputX < (x + width) && inputY < (y + height))//if the mouse is within the box to begin with
	{
		for (int i = 0; i < items.size(); i++)
		{
			if (inputY > (y + drawText->h * i) && inputY <= y + drawText->h * (i+1) && i != selectedID)
			{
				selectedID = i;
				foundSelected = true;
				break;
			}
		}
	}
	//free the surface
	SDL_FreeSurface(drawText);
	drawText = nullptr;

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
	if (selectedID > -1)
	{
		return items[selectedID];
	}
	else
	{
		return nullptr;
	}
}

void UISelectionBox::SetAlignment(int inAlign)
{
	Alignment = inAlign;
}


void UISelectionBox::Update(SDL_Event *e)
{

}

void UISelectionBox::Draw()
{
	//draw any gameobjects contained within if necessary
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->isGameObject)
		{
			items[i]->gameObject->Draw();
		}
	}


	SDL_SetRenderDrawColor(renderer, backColor1.r, backColor1.g, backColor1.b, backColor1.a);//set color to fill color
	SDL_RenderFillRect(renderer, &backRect);//Draw background rectangle

	SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);//set color to outline color
	SDL_RenderDrawRect(renderer, &backRect);//draw outline

	//for each item that it contains, draw the name
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->name == "") items[i]->name = " ";
		SDL_Surface* drawText = TTF_RenderText_Solid(defaultFont, items[i]->name.c_str(), textColor);//create a surface using text & font values
		SDL_Texture* textTex = SDL_CreateTextureFromSurface(renderer, drawText);//convert the surface to a texture so it can be drawn


		//every 2 items, back color will change to list items better
		if (i % 2 == 0)
		{
			SDL_SetRenderDrawColor(renderer, backColor3.r, backColor3.g, backColor3.b, backColor3.a);
			SDL_Rect tempBackRect = { backRect.x + 1, backRect.y + (drawText->h * i) + 1, backRect.w - 2, drawText->h - 2 };
			SDL_RenderFillRect(renderer, &tempBackRect);//Draw rectangle behind current text
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, backColor2.r, backColor2.g, backColor2.b, backColor2.a);
			SDL_Rect tempBackRect = { backRect.x + 1, backRect.y + (drawText->h * i) + 1, backRect.w - 2, drawText->h - 2 };
			SDL_RenderFillRect(renderer, &tempBackRect);//Draw rectangle behind current text
		}


		SDL_Rect textRect = { backRect.x + 5, backRect.y + (drawText->h * i), (drawText != nullptr) ? drawText->w : 0, drawText->h };//default to left alighment
		//dependent on the alignment, the text will be drawn in different locations
		if (Alignment == 0)// = LEFT
		{
			//already set to left
		}
		else if (Alignment == 1)// = CENTRE
		{
			textRect = { (backRect.x + (backRect.w/2) - drawText->w/2), backRect.y + (drawText->h * i), (drawText != nullptr) ? drawText->w : 0, drawText->h };
		}
		else//2 or otherwise = RIGHT
		{
			textRect = { (backRect.x + backRect.w - drawText->w) - 5, backRect.y + (drawText->h * i), (drawText != nullptr) ? drawText->w : 0, drawText->h };
		}
		

		//check if the current element is selected, as we will need to draw a highlight behind it if this is the case
		if (selectedID == i)
		{
			SDL_Rect highlightRect = { backRect.x + 1, backRect.y + (drawText->h * i) + 1, backRect.w - 2, drawText->h - 2 };
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
}