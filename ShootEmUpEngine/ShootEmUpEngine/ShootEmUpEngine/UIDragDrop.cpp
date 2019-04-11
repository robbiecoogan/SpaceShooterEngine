#include "UIDragDrop.h"

UIDragDrop::UIDragDrop(std::string inputName, int inputX, int inputY, int inputWidth, int inputHeight, std::string imageDir, SDL_Renderer* gameRenderer, SDL_Window* gameWindow) : name(inputName),  x(inputX), y(inputY), width(inputWidth), height(inputHeight), window(gameWindow), renderer(gameRenderer)
{
	myImage = NULL;

	SDL_Surface* optimizedSurface = NULL;

	SDL_Surface* loadSurface = IMG_Load(imageDir.c_str());

	SDL_SetSurfaceAlphaMod(loadSurface, 135);

	if (loadSurface == NULL) std::cout << "Error Loading Image: " << IMG_GetError();
	else
	{
		//now that the surface is initialized, format the surface to the screen format
		optimizedSurface = SDL_ConvertSurface(loadSurface, SDL_GetWindowSurface(gameWindow)->format, NULL);
		if (myImage == NULL) std::cout << "Error Optimizing Image: " << SDL_GetError();

		myImage = SDL_CreateTextureFromSurface(renderer, optimizedSurface);
		sourceRect = optimizedSurface->clip_rect;
		destRect = sourceRect;


		//free old surface
		SDL_FreeSurface(loadSurface);
		loadSurface = nullptr;
		SDL_FreeSurface(optimizedSurface);
		optimizedSurface = nullptr;
	}

	destRect.x = x;
	destRect.y = y;
	destRect.w = width;
	destRect.h = height;
}

SelectionBoxItem* UIDragDrop::HasDropped()
{
	SelectionBoxItem* temp = nullptr;
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);


	if (hasDropped)
	{
		hasDropped = false;

		if (name == "Player")
		{
			std::string imageDir = SDL_GetBasePath(); imageDir += "Assets\\PlayerDefault.png";
			temp = new SelectionBoxItem{ "New " + name, true, new GameObject(GameObjectType::PLAYER, mouseX, mouseY, imageDir, window, renderer) };
		}
		else if (name == "Enemy")
		{
			std::string imageDir = SDL_GetBasePath(); imageDir += "Assets\\EnemyDefault.png";
			temp = new SelectionBoxItem{ "New " + name, true, new GameObject(GameObjectType::ENEMYWAVE, mouseX, mouseY, imageDir, window, renderer) };
		}
		else if (name == "BGMusic")
		{
			std::string imageDir = SDL_GetBasePath(); imageDir += "Assets\\BGMusicDefault.png";
			temp = new SelectionBoxItem{ "New " + name, true, new GameObject(GameObjectType::BACKGROUNDMUSIC, mouseX, mouseY, imageDir, window, renderer) };
		}
	}

	return temp;
}

UIDragDrop::~UIDragDrop()
{

}

void UIDragDrop::Update(SDL_Event* e, SDL_Rect gameScreenRect)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	switch (e->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (SDL_GetMouseState(NULL, NULL) == SDL_BUTTON(SDL_BUTTON_LEFT) && mouseX > destRect.x && mouseX < destRect.x + destRect.w && mouseY > destRect.y && mouseY < destRect.y + destRect.h)
		{
			//std::cout << "Mouse Down Detected" << std::endl;
			isDragged = true;

			dragOffsetX = mouseX - x;
			dragOffsetY = mouseY - y;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		//std::cout << "Mouse Up Detected" << std::endl;
		//check if the mouse is released anywhere but the game screen, as we don't want this to trigger anything
		if (mouseX > gameScreenRect.x && mouseX < gameScreenRect.x + gameScreenRect.w && mouseY > gameScreenRect.y && mouseY < gameScreenRect.y + gameScreenRect.h && isDragged)
		{
			hasDropped = true;
		}
		else
		{
			
		}


		isDragged = false;
		
	}
}

void UIDragDrop::Draw()
{
	SDL_RenderCopy(renderer, myImage, &sourceRect, &destRect);

	if (isDragged)
	{
		SDL_Rect newRect = destRect;
		SDL_SetRenderDrawColor(renderer, 70, 70, 70, 0);
		SDL_RenderFillRect(renderer, &newRect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderDrawRect(renderer, &newRect);
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		newRect.x = mouseX - dragOffsetX;
		newRect.y = mouseY - dragOffsetY;
		SDL_RenderCopy(renderer, myImage, &sourceRect, &newRect);
	}
}