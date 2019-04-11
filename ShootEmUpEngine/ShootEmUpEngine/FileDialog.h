#pragma once
#include<iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <vector>
#include <SDL.h>
#include <SDL_Image.h>
#include "EXComponent.h"

#ifndef _FILE_DIALOG_H_
#define _FILE_DIALOG_H_

using namespace std;

enum SHOOTING_TYPE
{
	Single,
	Dual_Split,
	Dual_Spread
};

enum FORMATION
{
	stright,
	diaginalDown,
	diaginalUp,
	zigZagUp,
	zigZagDown
};

enum MOVE_PATTERN
{
	slow,
	fast
};


struct PlayerSaveStruct
{
public:
	int health;
	int shootPointX;
	int shootPointY;
	SDL_Rect XY;
	string bulletFilePath;
	string shipFilePath;
	SDL_Rect destRect;
	float shipSpeed;
	std::vector<EXEventComponent*> events;
};
struct EnemySaveStruct
{
public:
	int health;
	int shootPointX;
	int shootPointY;
	SDL_Rect XY;
	string shipFilePath;
	SDL_Rect destRect;
	float shipSpeed;
	float xMoveVec;
	float yMoveVec;
	int rotation;
};

struct BackgroundSaveStruct
{
public:
	SDL_Rect destRect;
	string fileDir;
	float scrollSpeed;
};


class FileDialog
{
public:



	FileDialog();
	~FileDialog();
	void GetOpenFile();
	void SaveFile();

	//export Functions
	void PrepBGAudioExport(string name, string filePath);
	void PrepAudioExport(string name, string filePath);
	void PrepBGExport(string fileDir, int layerNumber, float speed);
	void PrepPlayerExport(int health, bool enabled, int shootPointX, int shootPointY, float shootingSpeed, SHOOTING_TYPE shootingType,
		string shotFilePath, SDL_Rect XY, string shotSoundName, float shotDamage,
		string deathSoundName, string shipFilePath, SDL_Rect destRect, float shipSpeed);
	void PrepDebugFileLoc();
	void ShowDialogPrepFileLoc();
	void ShowDialogPrepReadLoc();
	void AddPlayerToJson(int health, int shootPointX, int shootPointY, SDL_Rect XY, string bulletFilePath, string shipFilePath, SDL_Rect destRect, float shipSpeed, std::vector<EXEventComponent*> inputEvents);
	void AddEnemyToJson(int health, int shootPointX, int shootPointY, SDL_Rect XY, int rotation, string shipFilePath, SDL_Rect destRect, float shipSpeed, float xMoveVec, float yMoveVec);
	void AddBackgroundToJson(SDL_Rect destRect, string inputFileDir, float inputScrollSpeed);
	void ExportJson(int screenWidth, int screenHeight);
	void SaveProject(int screenWidth, int screenHeight);
	void OpenProject(std::vector<BackgroundSaveStruct*>& bgOBJ, std::vector<PlayerSaveStruct*>& playerOBJ, std::vector<EnemySaveStruct*>& enemyOBJ);
	void PrepWaveExport(int health, bool enabled, int shootPointX, int shootPointY, float shootingSpeed, SHOOTING_TYPE shootingType, string shotSpriteName, SDL_Rect shotDestRect, string shotSoundName, float shotDamage, float quantityFrom, float quantityTo, FORMATION formation, MOVE_PATTERN moveMOVE_PATTERN, float speedFrom, float speedTo, float crashDamage, float startPos, float killPoints, SDL_Rect eDestRect);

	void ExportFile(string title, bool orientation);

	void CheckErrors();

	string GetFile();
private:
	vector<std::string> exportLines;

	std::string exportLoc;
	std::vector<PlayerSaveStruct> playerData;
	std::vector<EnemySaveStruct> enemyData;
	std::vector<BackgroundSaveStruct> backgroundData;
};
#endif
