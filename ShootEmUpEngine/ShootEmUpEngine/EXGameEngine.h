#pragma once
#include "SDL.h"
#include "SDL_TTF.h"
#include <nlohmann/json.hpp>
#include "EXGameObject.h"
#include <chrono>


class EXGameEngine
{

public:

	EXGameEngine(char* startupData, SDL_Window* inputWindow, SDL_Renderer* inputRenderer);//startupData will be a .JSON file location that the application was started with (e.g. if a file is dragged onto the exe)
	~EXGameEngine();

	void Init(const char* name, int xPosition, int yPosition, int width, int height, bool isFullscreen);

	SDL_Window* GetEngineWindow() { return window; }

	void ReadJsonGetWidthHeight();
	void ReadJson();

	void HandleEvents(SDL_Event* e);

	bool Update();

	void Draw();

	void Clean();

	bool IsRunning() { return gameIsRunning; }


private:
	SDL_Event * e;

	SDL_Window *window;
	SDL_Renderer *renderer;

	int screenWidth, screenHeight;

	char* startFileLoc;

	//SoundManager* _SoundManager;

	SDL_Color backColor;

	bool gameIsRunning = false;

	std::vector<EXGameObject*> gameObjects;

	int frameRate;//the framerate to cap the application at.
	void CapFrameRate();
};