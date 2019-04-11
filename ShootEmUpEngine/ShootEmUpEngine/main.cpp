#include "SDL.h"
#include "SDL_TTF.h"
#include "EngineManager.h"
using namespace std;

int main(int argc, char* args[])
{
	EngineManager *engineMain = new EngineManager();
	while (engineMain->IsRunning())
	{
		engineMain->Update();
		engineMain->Draw();
	}

	engineMain->Clean();
	SDL_Quit();
	TTF_Quit();

	return 0;

}