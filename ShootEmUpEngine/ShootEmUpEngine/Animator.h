#pragma once
#include "FileDialog.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>


class Animator
{
public:
	Animator(SDL_Renderer* gameRenderer, SDL_Window* window, SDL_Rect destRect);
	void SetAnimFile();
	void SetAnimFile(int inputFrames, std::string inputFileDir);
	void SetWidthHeight(int inputW, int inputH) { destRect.w = inputW, destRect.h = inputH; }

	~Animator();

	void Update();

	std::string GetFileDir() { return fileDir; }

	void Draw();
	void SetFrames(int frames);

	FileDialog* _FileDialog;

private:
	SDL_Surface * spriteImages;
	SDL_Texture* spriteSheetTexture;

	SDL_Renderer* renderer;
	SDL_Window* window;

	SDL_Rect srcRect;
	SDL_Rect destRect;

	Uint32 animationTicks;
	Uint32 sprite;

	std::string fileDir;

	int frames;


};

