#pragma once

#include "EXCommon.h"
#include "EXComponent.h"

#include "SDL_image.h"
#include "SDL.h"
#include <string>
#include <iostream>
#include <vector>

struct EXShootingPoint
{
	std::string bulletDir;
	SDL_Texture* bulletTex;
	SDL_Rect sourceRect;
	SDL_Rect destRect;
	int x, y;
	bool isEnabled;
};

class EXGameObject
{

public:
	EXGameObject(SDL_Rect inputSourceRect, SDL_Rect inputDestRect, Vector2 shootingPointPos, std::string inputSpriteDir, std::string inputBulletDir, SDL_Window* gameWindow, SDL_Renderer* gameRenderer);
	EXGameObject(SDL_Rect inputSourceRect, SDL_Rect inputDestRect, Vector2 shootingPointPos, SDL_Texture* myTex, SDL_Window* gameWindow, SDL_Renderer* gameRenderer);
	virtual ~EXGameObject();

	virtual void Update();
	virtual void Draw();

	void AddComponent(EXEventComponent* newEvent);

	std::string spriteDir;
	std::string shootingPointFileDir;

	SDL_Rect destRect;

	float xPos, yPos, rotation;
	Vector2 velocity;

	void ShootBullet(float bulletSpeed);

	std::vector<EXEventComponent*> events;
protected:
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

	int eventBtnIndex;

	EXShootingPoint shootingPoint;
	

	GOType type;


	

	bool upKeyIsDown;
	bool downKeyIsDown;
	bool shootIsDown;

private:
	std::vector<EXGameObject*> bullets;
};

class EXGOEnemy : public EXGameObject
{
public:
	EXGOEnemy(SDL_Rect inputSourceRect,
			SDL_Rect inputDestRect, 
			std::string inputSpriteDir, 
			SDL_Window* gameWindow, 
			SDL_Renderer* gameRenderer, 
			Vector2 inputMoveVec,
			int inputRotation);
	~EXGOEnemy();

	void Update() override;

private:
	Vector2 moveVec;
};

class EXGOBackground : public EXGameObject
{
public:
	EXGOBackground(SDL_Rect inputDestRect, std::string inputSpriteDir, SDL_Window* gameWindow, SDL_Renderer* gameRenderer, float inputScrollSpeed) : EXGameObject(SDL_Rect{ 0,0,0,0 }, inputDestRect, Vector2::Zero(), inputSpriteDir, "", gameWindow, gameRenderer), scrollSpeed(inputScrollSpeed)
	{
		type = GOType::Background;
		object2DestRect = inputDestRect;
		if (scrollSpeed > 0)
		{
			object2DestRect.x = destRect.x - destRect.w;
		}
		else
		{
			object2DestRect.x = destRect.x + destRect.w;
		}
		obj2XPos = object2DestRect.x;
	};

	~EXGOBackground();

	void Update() override;
	void Draw() override;

	float scrollSpeed;
private:
	SDL_Rect object2DestRect;
	float obj2XPos;
};

class EXGOBullet : public EXGameObject
{
public:
	EXGOBullet(SDL_Rect sourceRect,
			SDL_Rect destRect,
			SDL_Texture* bulletTexture,
			SDL_Window* gameWindow,
			SDL_Renderer* gameRenderer,
			Vector2 inputMoveVec,
			int inputRotation);

	~EXGOBullet();

	void Update() override;
private:
	Vector2 moveVec;


};