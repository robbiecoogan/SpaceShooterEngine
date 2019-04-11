#include "EventManager.h"

TTF_Font* EventManager::defaultFont;
float EventManager::dTime;
float EventManager::lastFrame;
float EventManager::thisFrame;
float EventManager::screenScale = 1.0f;
int EventManager::debugYOffset = 0.0f;
FileDialog* EventManager::_FileDialog = new FileDialog();



void EventManager::UpdateEventList()
{
	//calc delta time so every class has easy access
	lastFrame = thisFrame;
	thisFrame = SDL_GetPerformanceCounter();
	dTime = (double)(thisFrame - lastFrame) / (double)SDL_GetPerformanceFrequency();
	if (dTime > 0.5f) dTime = 0.5f;

	for (size_t i = 0; i < eventList.size(); i++)
	{
		delete eventList[i];
	}
	eventList.clear();

	SDL_Event* e = new SDL_Event;

	while (SDL_PollEvent(e))
	{
		SDL_Event* newE = new SDL_Event;
		*newE = *e;
		eventList.push_back(newE);
	}

	delete e;

}

std::vector<SDL_Event*> EventManager::GetEvent(SDL_EventType eType)
{
	std::vector<SDL_Event*> tempList;

	for (size_t i = 0; i < eventList.size(); i++)
	{
		if (eventList[i]->type == SDL_QUIT)
		{
			int asd = 0;
		}
		if (eventList[i]->type == eType) tempList.emplace_back(eventList[i]);
	}

	return tempList;
}

std::vector<SDL_Event*> EventManager::GetEvent(SDL_WindowEventID eType)
{
	std::vector<SDL_Event*> tempList;

	for (size_t i = 0; i < eventList.size(); i++)
	{
		if (eventList[i]->window.type == eType)  tempList.emplace_back(eventList[i]);
	}


	return tempList;
}
