#include "EngineManager.h"
#include "EventManager.h"


EngineManager::EngineManager()
{
	//initalize SDL
	Init("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1360, 768, false);

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

		e = new SDL_Event;
	}
	else
	{
		gameIsRunning = false;
	}
}

EngineManager::~EngineManager()
{
	delete _UIManager;
	delete _SoundManager;
	delete _FileDialog;
	delete e;
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

void EngineManager::HandleEvents(SDL_Event* e)
{
	EventManager::getInstance().UpdateEventList();
	std::vector<SDL_Event*> tempE = EventManager::getInstance().GetEvent(SDL_WINDOWEVENT);
	int eSize = tempE.size();
	for (size_t i = 0; i < eSize; i++)
	{
		if (tempE[i]->window.event == SDL_WINDOWEVENT_CLOSE && tempE[i]->window.windowID == SDL_GetWindowID(window))
		{
			gameIsRunning = false;
		}
	}
}

void EngineManager::Update()
{
	//as the other update functions will use events, to prevent recreation of these and prevent interference, this is passed through
	HandleEvents(e);

	_UIManager->Update(e);//As UI elements will need info regarding key presses, this is passed to the update. If an object doesnt need this, dont bother passing these through.
	_SoundManager->Update();

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