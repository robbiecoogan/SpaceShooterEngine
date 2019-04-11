#include "EXGameObject.h"
#include "EventManager.h"

EXGOBullet::EXGOBullet(SDL_Rect inputSourceRect, SDL_Rect inputDestRect, SDL_Texture* bulletTexture, SDL_Window* gameWindow, SDL_Renderer* gameRenderer, Vector2 inputMoveVec, int inputRotation) : EXGameObject(inputSourceRect, inputDestRect, Vector2::Zero(), bulletTexture, gameWindow, gameRenderer), moveVec(inputMoveVec)
{
	float screenScale = EventManager::screenScale;

	type = GOType::Bullet;
	rotation = inputRotation;

	destRect.w *= screenScale;
	destRect.h *= screenScale;

	xPos -= destRect.w / 2;
	yPos -= destRect.h / 2;

	destRect.x = xPos;
	destRect.y = yPos; 
}

EXGOBullet::~EXGOBullet()
{
}

void EXGOBullet::Update()
{
	int screenWidth, screenHeight;
 	SDL_GetWindowSize(window, &screenWidth, &screenHeight);

	Vector2 newMoveVec = moveVec * EventManager::dTime * 20;

	xPos += newMoveVec.x;
	yPos += newMoveVec.y;

	destRect.x = xPos;
	destRect.y = yPos;

}