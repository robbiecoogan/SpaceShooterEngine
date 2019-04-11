#pragma once

#include <vector>
#include <string>
#include "SDL.h"
#include "SDL_TTF.h"
#include "Common.h"
#include "SoundManager.h"

class UISelectionBox//This defines a box that lists items. This can be used for selecting objects in the scene, or selecting the current level
{
public:
	UISelectionBox(int inputX, int inputY, int inputWidth, int inputHeight, SDL_Renderer* gameRenderer, TTF_Font* font);

	~UISelectionBox();

	void Init();

	void Update(SDL_Event* e);

	void Draw();

	void AddItem(SelectionBoxItem* item);
	void RemoveAt(int index);
	int GetSelectedIndex();
	SelectionBoxItem* GetItem(int index) { return items[index]; }

	SelectionBoxItem* GetSelectedItem();

	SelectionBoxItem* CheckSelected(int inputX, int inputY);//x and y coords of the mouse are passed in.

	///provide an int to change the alignment of how each object is listed. 0 = left, 1 = centre, 2 = right
	void SetAlignment(int inAlign);

private:
	int x, y, width, height;
	SDL_Rect backRect;//Back rectangle of the selection box
	SDL_Color backColor1;//very back wall color of the box
	SDL_Color backColor2;//when drawing the objects, colours shift between each object to make it easier to see. this is color 1
	SDL_Color backColor3;//when drawing the objects, colours shift between each object to make it easier to see. this is color 2
	SDL_Color outlineColor;

	SDL_Renderer* renderer;

	TTF_Font* defaultFont;
	SDL_Color textColor;//color of the text listed
	SDL_Color highlightColor;//color of the highlight behind the text

	std::vector<SelectionBoxItem*> items;

	int selectedID;

	int Alignment;

};