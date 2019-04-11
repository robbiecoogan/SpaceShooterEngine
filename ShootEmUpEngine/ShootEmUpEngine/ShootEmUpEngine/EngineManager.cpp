#include "EngineManager.h"


EngineManager::EngineManager()
{
	//initalize SDL
	Init("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, false);

	//Init UI Manager
	_UIManager = new UIManager(&gameIsRunning, window, renderer);

	//Sound Manager
	_SoundManager = SoundManager::getInstance();
}

void EngineManager::Init(const char* name, int xPosition, int yPosition, int width, int height, bool isFullscreen)
{
	backColor = { 200, 200, 255, 255 };

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() == 0)
	{
		int screenFlag = 0;
		if (isFullscreen) screenFlag = SDL_WINDOW_FULLSCREEN;

		window = SDL_CreateWindow(name, xPosition, yPosition, width, height, screenFlag);

		renderer = SDL_CreateRenderer(window, -1, 0);

		SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);

		gameIsRunning = true;

		//SDL_SetWindowBordered(window, SDL_FALSE);
	}
	else
	{
		gameIsRunning = false;
	}
}

EngineManager::~EngineManager()
{
	_UIManager->~UIManager();
	_UIManager->~UIManager();
	delete _UIManager;
	delete _SoundManager;
}

void EngineManager::HandleEvents()
{

}

void EngineManager::Update()
{
	//as the other update functions will use events, to prevent recreation of these and prevent interference, this is passed through
	SDL_Event* e = new SDL_Event;
	SDL_PollEvent(e);

	if (e->type == SDL_QUIT)//if the 'X' button in the top right is pressed, flag the app to be not running, which will close the application.
	{
		gameIsRunning = false;
	}


	_UIManager->Update(e);//As UI elements will need info regarding key presses, this is passed to the update. If an object doesnt need this, dont bother passing these through.
	_SoundManager->Update(e);

	delete e;
	e = nullptr;

}

void EngineManager::Draw()
{
	SDL_RenderClear(renderer);

	_UIManager->Draw();

	SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
	SDL_RenderPresent(renderer);
}

void EngineManager::Clean()
{

}