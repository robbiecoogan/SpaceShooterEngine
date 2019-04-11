#include "GameObject.h"


GameObject::GameObject(GameObjectType inputType, int inputX, int inputY, std::string imageDir, SDL_Window* gameWindow, SDL_Renderer* gameRenderer) : type(inputType), x(inputX), y(inputY), renderer(gameRenderer)
{
	texture = NULL;
	SDL_Surface* loadSurface = IMG_Load(imageDir.c_str());
	
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	if (loadSurface == NULL) std::cout << "Error Loading Image: " << IMG_GetError() << std::endl;
	else
	{
		SDL_ConvertSurfaceFormat(loadSurface, SDL_PIXELFORMAT_RGBA8888, 0);
		
		texture = SDL_CreateTextureFromSurface(renderer, loadSurface);

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		sourceRect = loadSurface->clip_rect;
		destRect = sourceRect;

	}

	//adjust the start x and y positions to have the object spawn in the centre of the mouse
	destRect.x = x - (loadSurface->w / 2);
	destRect.y = y - (loadSurface->h / 2);

	//free old surface
	SDL_FreeSurface(loadSurface);
	loadSurface = nullptr;
}

GameObject::~GameObject()
{

}

void GameObject::Draw()
{
	SDL_RenderCopy(renderer, texture, NULL, &destRect);
}