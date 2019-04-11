#include "EXGameObject.h"
#include "EventManager.h"

EXGOEnemy::EXGOEnemy(SDL_Rect inputSourceRect, SDL_Rect inputDestRect, std::string inputSpriteDir, SDL_Window* gameWindow, SDL_Renderer* gameRenderer, Vector2 inputMoveVec, int inputRotation) : EXGameObject(inputSourceRect, inputDestRect, Vector2::Zero(), inputSpriteDir, "", gameWindow, gameRenderer), moveVec(inputMoveVec)
{
	type = GOType::Enemy;
	rotation = inputRotation;
}

EXGOEnemy::~EXGOEnemy()
{
}

void EXGOEnemy::Update()
{
	Vector2 newMoveVec = moveVec * EventManager::dTime * 500;
	
	xPos += newMoveVec.x;
	yPos += newMoveVec.y;

	destRect.x = xPos;
	destRect.y = yPos;
}