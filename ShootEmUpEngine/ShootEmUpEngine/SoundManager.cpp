#include "SoundManager.h"

//Gets music file from the assets folder
Mix_Music* SoundManager::GetMusic(string filename)
{
	string filePath = SDL_GetBasePath();

	string baseFilePath = SDL_GetBasePath();

	if (filename.substr(0, baseFilePath.size()) == baseFilePath) 
	{
		filePath = filename;
	}
	else
	{
		filePath.append("Assets\\" + filename);
	}

	
	cout << filePath.c_str() << endl;

	if (_music[filePath] == nullptr)
	{
		_music[filePath] = Mix_LoadMUS(filePath.c_str());

		if (_music[filePath] == NULL)
		{
			printf("Music Loading Error: ", filename.c_str(), Mix_GetError());
		}

		return _music[filePath];
	}
}

//Gets SFX files from the assets folder
Mix_Chunk* SoundManager::GetSFX(string filename)
{
	string filePath = SDL_GetBasePath();

	string baseFilePath = SDL_GetBasePath();
	for (int i = 0; i < baseFilePath.size(); i++)
	{
		if (baseFilePath[i] != filename[i])
		{
			filePath.append("Assets\\" + filename);
			break;
		}
		else
		{
			filePath = filename.c_str();
		}
	}

	cout << filePath.c_str() << endl;

	_SFX[filePath] = Mix_LoadWAV(filePath.c_str());

		if (_SFX[filePath] == NULL)
		{
			printf("SFX Loading Error: ", filename.c_str(), Mix_GetError());
		}
		return _SFX[filePath];
}

SoundManager* SoundManager::instance = NULL;

SoundManager* SoundManager::getInstance() 
{
	if (!instance)
	{
		instance = new SoundManager();
	}
	return instance;
}

SoundManager::SoundManager()
{
	SDL_Event event;
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2,4096) == -1)
	{
		printf("Mixer Initialization Error: ", Mix_GetError());
	}

	//PlayMusic("background.wav");
	//PlaySFX("explosion.wav");
}


SoundManager::~SoundManager()
{
	delete instance;
	instance = NULL;
	Mix_Quit();
}

void SoundManager::Update()
{
	
}

//Plays the music file 
void SoundManager::PlayMusic(string filename, int loops)
{
	Mix_PlayMusic(GetMusic(filename), loops);
	cout << "playing music" << endl;
}

void SoundManager::PauseMusic()
{
	if (Mix_PlayingMusic() != 0)
	{
		Mix_PauseMusic();
	}
}

void SoundManager::ResumeMusic()
{
	if (Mix_PausedMusic() != 0)
	{
		Mix_ResumeMusic();
	}
}

//plays SFX file
void SoundManager::PlaySFX(string filename, int loops, int channel)
{
	Mix_PlayChannel(-1, GetSFX(filename), 0);
	cout << "playing SFX" << endl;
}