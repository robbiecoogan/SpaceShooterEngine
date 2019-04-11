#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include <vector>
#include <iostream>
#include "FileDialog.h"

class EventManager
{
public:
	static EventManager& getInstance()
	{
		if (!defaultFont)
		{
			defaultFont = TTF_OpenFont("Abel.ttf", 20);
			if (defaultFont == NULL)
				std::cout << "Couldnt find font file";
		}


		static EventManager instance;
		return instance;
		thisFrame = SDL_GetPerformanceCounter();
		lastFrame = thisFrame;
	}


	EventManager(EventManager const&) = delete;
	void operator=(EventManager const&) = delete;

	void UpdateEventList();
	std::vector<SDL_Event*> GetEvent(SDL_EventType eType);
	std::vector<SDL_Event*> GetEvent(SDL_WindowEventID eType);
	
	static TTF_Font* defaultFont;

	static float dTime;
	static float screenScale;
	static int debugYOffset;
	static FileDialog* _FileDialog;

private:
	EventManager() {}

	std::vector<SDL_Event*> eventList;

	static float thisFrame, lastFrame;

};