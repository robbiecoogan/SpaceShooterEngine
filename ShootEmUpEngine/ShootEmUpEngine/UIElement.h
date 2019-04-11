#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "SDL.h"
#include "SDL_TTF.h"
#include "SDL_image.h"
#include "SoundManager.h"
#include "EXComponent.h"

enum UIType
{
	Button,
	DragDrop,
	Label,
	ScrollBar,
	SelectionBox,
	TextBox,
	VScrollBar,
	EventButton,
	ActionButton
};


class UIElement
{
public:
	UIType type;
	virtual void Update(SDL_Rect activeScreenRect) = 0;
	virtual void Draw() = 0;
	virtual ~UIElement() = default;
	int GetX() const { return x; }
	int GetY() const { return y; }
	void setXOffset(int offset) { xOffset = offset; };
	void setYOffset(int offset) { yOffset = offset; };
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	void ForceType(UIType newtype) { type = newtype; }
protected:
	int x, y, width, height;
	int xOffset, yOffset;
};


class UIButton : public UIElement
{
public:
	UIButton(int inputX, int inputY, int inputWidth, int inputHeight, std::string inputText, SDL_Renderer * gameRenderer, SDL_Window* gameWindow, TTF_Font* inputFont);
	~UIButton();

	void Update(SDL_Rect activeScreenRect);
	void Draw();

	void SetColour(SDL_Color backCol);

	void SetYPos(int inputY);

	string GetText() { return text; }

	bool CheckClick();

	bool isClicked;
	
	EXEventComponent* pEvent = nullptr;
	EXActionComponent* pAction = nullptr;

private:
	SDL_Renderer * renderer;
	SDL_Window* window;

	SDL_Color outlineColor;
	SDL_Color notClickedColor;
	SDL_Color clickedColor;
	SDL_Color textColor;

	SDL_Rect buttonRect;

	TTF_Font* defaultFont;
	std::string text;

	bool mouseDown;	

};

struct SelectionBoxItem;

class UIDragDrop : public UIElement
{
public:
	UIDragDrop(std::string inputName, int inputX, int inputY, int inputWidth, int inputHeight, std::string imageDir, SDL_Renderer* gameRenderer, SDL_Window* gameWindow);
	~UIDragDrop();

	void Update(SDL_Rect gameScreenRect);
	void Draw();

	SelectionBoxItem* HasDropped();

private:
	SDL_Renderer * renderer;
	SDL_Window* window;

	SDL_Texture* myImage;
	SDL_Rect sourceRect;
	SDL_Rect destRect;

	bool isDragged;
	int dragOffsetX;
	int dragOffsetY;
	bool hasDropped;

	std::string name;
};

class UILabel : public UIElement
{

public:
	UILabel(int inputX, int inputY, std::string inputText, SDL_Renderer* gameRenderer, SDL_Window* gameWindow, int inputAlignment, TTF_Font* font);
	~UILabel();

	void Update(SDL_Rect activeScreenRect);
	void Draw();

	void SetYPos(int y);

private:

	std::string text;

	SDL_Renderer* renderer;
	SDL_Window* window;

	SDL_Color textColor;

	TTF_Font* defaultFont;

	SDL_Surface* drawText;
	SDL_Texture* textTex;

	int alignment;
};

class UIScrollbar : public UIElement
{
public:
	UIScrollbar(int inputX, int inputY, int inputWidth, int inputHeight, SDL_Renderer* gameRenderer, SDL_Window* gameWindow);
	~UIScrollbar();

	void Update(SDL_Rect activeScreenRect);
	void Draw();

	int GetValue() { return value; };

private:

	SDL_Rect backRect;//Back rectangle of the bar
	SDL_Rect notchRect;//rectangle for the notch of the bar

	SDL_Color backColor;//back color of the bar
	SDL_Color notchColor;//color of the drag part of the scrollbar
	SDL_Color outlineColor;//outline color of the bar

	SDL_Renderer* renderer;
	SDL_Window* window;

	int value;
	int minVal;
	int maxVal;

	bool hasBeenClicked;
};

class UIVScrollbar : public UIElement
{
public:
	UIVScrollbar(int inputX, int inputY, int inputWidth, int inputHeight, SDL_Renderer* gameRenderer, SDL_Window* gameWindow);
	~UIVScrollbar();

	void Update(SDL_Rect activeScreenRect);
	void Draw();

	int GetValue() { return value; };

	void SetMaxVal(float inputVal);
	
	
private:

	SDL_Rect backRect;//Back rectangle of the bar
	SDL_Rect notchRect;//rectangle for the notch of the bar

	SDL_Color backColor;//back color of the bar
	SDL_Color notchColor;//color of the drag part of the scrollbar
	SDL_Color outlineColor;//outline color of the bar

	SDL_Renderer* renderer;
	SDL_Window* window;

	int value;
	float minVal;
	float maxVal;

	bool hasBeenClicked;
};

struct SelectionBoxItem;

class UISelectionBox : public UIElement//This defines a box that lists items. This can be used for selecting objects in the scene, or selecting the current level
{
public:
	UISelectionBox(int inputX, int inputY, int inputWidth, int inputHeight, SDL_Renderer* gameRenderer, SDL_Window* gameWindow, TTF_Font* font, SDL_Rect gameScreenRect);

	~UISelectionBox();

	void Init();

	void Update(SDL_Rect activeScreenRect);
	void Draw();
	void Draw(bool drawGOOutLine);
	int scrollOffset;

	void AddItem(SelectionBoxItem* item);
	void RemoveAt(int index);
	int GetSelectedIndex();
	int GetItemCount();
	SelectionBoxItem* GetItem(int index) { return items[index]; }

	SelectionBoxItem* GetSelectedItem();

	SelectionBoxItem* CheckSelected(int inputX, int inputY);//x and y coords of the mouse are passed in.

	bool NewSelected();//returns true if the user has just selected something new

	///when called, forces NewSelected to return true on next call, can be used to forcefully update the UI
	void ForceNewSelected();

	///provide an int to change the alignment of how each object is listed. 0 = left, 1 = centre, 2 = right
	void SetAlignment(int inAlign);

private:
	SDL_Rect backRect;//Back rectangle of the selection box
	SDL_Color backColor1;//very back wall color of the box
	SDL_Color backColor2;//when drawing the objects, colours shift between each object to make it easier to see. this is color 1
	SDL_Color backColor3;//when drawing the objects, colours shift between each object to make it easier to see. this is color 2
	SDL_Color outlineColor;

	SDL_Renderer* renderer;
	SDL_Window* window;

	TTF_Font* defaultFont;
	SDL_Color textColor;//color of the text listed
	SDL_Color highlightColor;//color of the highlight behind the text

	std::vector<SelectionBoxItem*> items;

	int selectedID;
	int prevFrameSelected;
	int Alignment;
	bool newSelected;

	SDL_Rect windowRect;
	SDL_Rect activeScreenRect;

};

class UITextBox : public UIElement
{
	//This is an element of the UI. It is a field for entering data
public:
	UITextBox(int xPos, int yPos, int boxWidth, SDL_Renderer* gameRenderer, SDL_Window* gameWindow, TTF_Font* font);
	UITextBox(int xPos, int yPos, int boxWidth, std::string inputText, SDL_Renderer* gameRenderer, SDL_Window* gameWindow, TTF_Font* font);
	~UITextBox();

	void Init();

	void Update(SDL_Rect activeScreenRect);
	void Draw();

	std::string GetText() { return text; }

	void SetText(std::string input);

	void SetSelected(bool input);
	bool GetSelected() { return isSelected; }

	bool IsUpdated();//returns true if there has been a change in text in the box

	void GetSize(int* inputW, int* inputH);

private:
	std::string text;

	SDL_Color backColor;//fill color of the text box
	SDL_Color boundColor;//outline color of the text box
	SDL_Color textColor;//color of the text entered

	SDL_Rect boxSquare;

	SDL_Renderer* renderer;
	SDL_Window* window;

	TTF_Font* defaultFont;

	float timeCounter;
	float deltaTime;
	float lastFrameTick;
	float flashingLineTimer;

	void UpdateText();

	bool isSelected;
	bool isUpdated;


};