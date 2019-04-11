#pragma once

#ifndef _SOUNDMANAGER_H
#define _SOUNDMANAGER_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <map>
#include <iostream>

using namespace::std;

class SoundManager
{

private:
	//Handles sound effects (Samples)
	map<string, Mix_Music*> _music;
	//Handles sound effects (Samples)
	map<string, Mix_Chunk*> _SFX;

	static SoundManager* instance;

public:
	SoundManager();
	~SoundManager();

	void Update(SDL_Event* e);

	Mix_Music* GetMusic(string filename);
	Mix_Chunk* GetSFX(string filename);

	static SoundManager* getInstance();

	void PlayMusic(string filename, int loops = -1);
	void PauseMusic();
	void ResumeMusic();

	void PlaySFX(string filename, int loops = 0, int channel = 0);

	bool test = false;
};
#endif

