#include "EXGameObject.h"
#include "EventManager.h"

EXGOBackground::~EXGOBackground()
{

}

void EXGOBackground::Update()
{
	xPos += scrollSpeed * EventManager::dTime;
	obj2XPos += scrollSpeed * EventManager::dTime;

	if (xPos < -destRect.w)
		xPos += destRect.w * 2;
	if (obj2XPos < -object2DestRect.w)
		obj2XPos += object2DestRect.w * 2;


	destRect.x = xPos;
	object2DestRect.x = obj2XPos;

}

void EXGOBackground::Draw()
{
	SDL_Point pos;
	pos.x = destRect.x + (destRect.w / 2);
	pos.y = destRect.y + (destRect.h / 2);
	SDL_RenderCopyEx(renderer, texture, NULL, &destRect, 0, &pos, SDL_RendererFlip::SDL_FLIP_NONE);
	


	SDL_RenderCopyEx(renderer, texture, NULL, &object2DestRect, 0, &pos, SDL_RendererFlip::SDL_FLIP_NONE);
}