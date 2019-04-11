#pragma once

#include "SDL_image.h"
#include "SDL.h"
#include <string>
#include <iostream>
#include "enums.h"
#include "Animator.h"
#include "EXComponent.h"
#include "UIElement.h"
#include "EventSelectionWindow.h"
#include "ActionSelectionWindow.h"
#include "DoAnimationWindow.h"
#include "RelativeWindow.h"
#include "AddScoreWindow.h"
#include "DoSoundEffectWindow.h"
#include "ShootWindow.h"
#include "AddVelocityWindow.h"
#include "SubtractVelocityWindow.h"
#include "MultiplyVelocityWindow.h"


struct ShootingPoint
{
	SDL_Texture* texture;
	SDL_Rect sourceRect;
	SDL_Rect destRect;
	int x, y;
	bool isEnabled;
};

class GameObject
{

public:
	GameObject(std::string inputName, int inputX, int inputY, std::string imageDir, SDL_Window* gameWindow, SDL_Renderer* gameRenderer);
	~GameObject();
	
	GameObjectType type;

	virtual void Update(SDL_Rect gameScreenRect) = 0;
	virtual void Draw(int scrollBarOffset) = 0;
	void DrawUI();

	bool IsUpdated();
	bool IsUIUpdated();

	bool SetNewImgFromFile(std::string fileDir);
	bool SetNewBulletImgFromFile(std::string fileDir);

	void SetClicked(bool inputBool);

	void AddComponent(UIElement* inputElement);

	void ForceUpdateEvents();

	bool isClicked;
	bool isSelected;
	std::string name;
	std::string spriteDir;
	std::string bulletDir;
	int x, y, modX, width, height, rotation;
	SDL_Rect destRect;
	
	std::vector<EXEventComponent*> events;

	ShootingPoint shootingPoint;

protected:
	
	void UIAddEvent(EXEventComponent* addComponent);

	void ProcessWindows();///<summary>This function runs through all action windows and event windows and manages their states</summary>

	SDL_Texture* texture;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect sourceRect;
	
	int clickXOffset;
	int clickYOffset;

	bool justDropped;
	bool isUpdated;
	bool isUIUpdated;

	int scrollBarOffset;

	int lastFrameX, lastFrameY;

	std::vector<UIElement*> UIComponents;

	EventSelectionWindow* eventWindow;
	ActionSelectionWindow* actionWindow;
	DoAnimationWindow* doAnimationWindow;
	RelativeWindow* relativeWindow;
	AddScoreWindow* addScoreWindow;
	DoSoundEffectWindow* doSoundEffectWindow;
	ShootWindow* shootWindow;
	AddVelocityWindow* addVelocityWindow;
	SubtractVelocityWindow* subtractVelocityWindow;
	MultiplyVelocityWindow* multiplyVelocityWindow;

	TTF_Font* defaultFont;

	int eventBtnIndex;


	UIVScrollbar* vScrollBar;

	SDL_Rect activeScreenRect;

};

class GOPlayer : public GameObject
{
public:
	GOPlayer(std::string inputName, int inputX, int inputY, std::string imageDir, SDL_Window* gameWindow, SDL_Renderer* gameRenderer) : GameObject(inputName, inputX, inputY, imageDir, gameWindow, gameRenderer) 
	{
		type = GameObjectType::PLAYER;
		eventBtnIndex = 10;
	};

	void Update(SDL_Rect gameScreenRect);
	void Draw(int scrollBarOffset);

};

class GOEnemy : public GameObject
{
public:
	GOEnemy(std::string inputName, int inputX, int inputY, std::string imageDir, SDL_Window* gameWindow, SDL_Renderer* gameRenderer) : GameObject(inputName, inputX, inputY, imageDir, gameWindow, gameRenderer)
	{
		type = GameObjectType::ENEMYWAVE;
		XVec = 0.0f;
		YVec = 0.0f;
	};
	void Update(SDL_Rect gameScreenRect);
	void Draw(int scrollBarOffset);

	float XVec;
	float YVec;
private:

};

class GOBackground : public GameObject
{
public:
	GOBackground(std::string inputName, int inputX, int inputY, std::string imageDir, SDL_Window* gameWindow, SDL_Renderer* gameRenderer) : GameObject(inputName, inputX, inputY, imageDir, gameWindow, gameRenderer)
	{
		type = GameObjectType::BACKGROUND;
	};

	void Update(SDL_Rect gameScreenRect);
	void Draw(int scrollBarOffset);

	float scrollSpeed;
private:

};

