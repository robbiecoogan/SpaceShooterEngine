#pragma once
#include "SDL.h"
#include "SDL_TTF.h"
#include "UIManager.h"
#include "SoundManager.h"
#include "FileDialog.h"


class EngineManager
{

public:

	EngineManager();
	~EngineManager();

	void Init(const char* name, int xPosition, int yPosition, int width, int height, bool isFullscreen);

	void HandleEvents(SDL_Event* e);

	void Update();

	void Draw();

	void Clean();

	bool IsRunning() { return gameIsRunning; }


private:
	SDL_Event * e;

	SDL_Window *window;
	SDL_Renderer *renderer;

	SoundManager* _SoundManager;
	UIManager * _UIManager;
	FileDialog* _FileDialog;

	SDL_Color backColor;

	bool gameIsRunning = false;

};