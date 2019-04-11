#include "EXGameEngine.h"
#include "EventManager.h"
#include <fstream>
#include <iostream>
#include <thread>
#include "EXComponent.h"

using json = nlohmann::json;

typedef std::chrono::milliseconds MS;

EXGameEngine::EXGameEngine(char* startupData, SDL_Window* inputWindow, SDL_Renderer* inputRenderer)
{
	startFileLoc = startupData;
	ReadJsonGetWidthHeight();

	window = inputWindow;
	renderer = inputRenderer;

	frameRate = 60;


	//initalize SDL
	Init("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, false);
}

EXGameEngine::~EXGameEngine()
{
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
	}
}

void EXGameEngine::Init(const char * name, int xPosition, int yPosition, int width, int height, bool isFullscreen)
{
	backColor = { 200, 200, 255, 255 };
	float renderSize = 0.1;

	//if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() == 0)
	//{
		//int screenFlag = 0;
		//if (isFullscreen) screenFlag = SDL_WINDOW_FULLSCREEN;

		//window = SDL_CreateWindow(name, xPosition, yPosition, width, height, screenFlag);

		//renderer = SDL_CreateRenderer(window, -1, 0);

		SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);

		//SDL_RenderSetScale(renderer, renderSize, renderSize);

		gameIsRunning = true;

		//SDL_SetWindowBordered(window, SDL_FALSE);

		ReadJson();
	//}
	//else
	//{
	//	gameIsRunning = false;
	//}

}

void EXGameEngine::ReadJsonGetWidthHeight()
{
	std::ifstream* ifs;

	std::cout << "Attempting to obtain screen width/height details...\n";

	if (startFileLoc)
	{
		std::cout << "External data file received.\nReading file...";
		ifs = new std::ifstream(startFileLoc);
	}
	else
	{
		std::cout << "NO DATA FILE!\nReading from Debug file...";
		ifs = new std::ifstream("debugfile.JSON");
	}

	json j;
	j = json::parse(*ifs);

	std::cout << "File read correctly";

	screenWidth = j["StartupInfo"]["ScreenWidth"];
	screenHeight = j["StartupInfo"]["ScreenHeight"];

	delete ifs;

}

void EXGameEngine::ReadJson()
{
	std::ifstream* ifs;
	float renderSize = EventManager::getInstance().screenScale;

	if (startFileLoc)
	{
		//std::cout << "External data file received.\nReading file...";
		ifs = new std::ifstream(startFileLoc);
	}
	else
	{
		//std::cout << "NO DATA FILE!\nReading from Debug file...";
		ifs = new std::ifstream("debugfile.JSON");
	}

	json j;
	j = json::parse(*ifs);
	//std::cout << "File read correctly";

	int yOffset = EventManager::getInstance().debugYOffset;

	int numBackgrounds = j["StartupInfo"]["BackgroundCount"];
	for (size_t i = 0; i < numBackgrounds; i++)
	{
		std::string currBGStr = "Background" + std::to_string(i);

		float tempScrollSpeed;
		SDL_Rect tempDestRect;
		std::string filePath = "";

		tempDestRect.x = j[currBGStr]["destRect.x"];
		tempDestRect.y = j[currBGStr]["destRect.y"];
		//tempDestRect.w = j[currBGStr]["destRect.w"];
		tempDestRect.w = screenWidth;
		tempDestRect.h = screenHeight;
		//tempDestRect.h = j[currBGStr]["destRect.h"];
		tempScrollSpeed = j[currBGStr]["scrollSpeed"];
		tempScrollSpeed *= renderSize;


		filePath = j[currBGStr]["fileDir"].dump();
		//as the file path is written as "test\\file", we need to get rid of one of the two backslashes before attempting to read the location, as it won't work.
		//also, the file path begins and ends with a backslash due to the json reader. this will also be clipped off
		filePath = filePath.substr(1, filePath.size() - 2);
		for (size_t k = 0; k < filePath.size(); k++)
		{
			if (filePath[k] == '\\')
			{
				filePath = filePath.substr(0, k) + filePath.substr(k + 1, 255);
				k++;
			}
		}

		EXGOBackground* enemyObj = new EXGOBackground(tempDestRect * renderSize, filePath, window, renderer, tempScrollSpeed);
		gameObjects.emplace_back(enemyObj);

	}
	int numPlayers = j["StartupInfo"]["PlayerCount"];
	for (size_t i = 0; i < numPlayers; i++)
	{
		std::string currPlayerStr = "Player" + std::to_string(i);
		
		SDL_Rect tempSourceRect;
		SDL_Rect tempDestRect;
		EXShootingPoint tempSP;
		std::string shipFilePath = "";
		std::string bulletFilePath = "";

		tempSourceRect.x = j[currPlayerStr]["XY.x"];
		tempSourceRect.y = j[currPlayerStr]["XY.y"];
		tempSourceRect.w = j[currPlayerStr]["XY.w"];
		tempSourceRect.h = j[currPlayerStr]["XY.h"];
		tempDestRect.x = j[currPlayerStr]["destRect.x"];
		tempDestRect.y = j[currPlayerStr]["destRect.y"];
		tempDestRect.w = j[currPlayerStr]["destRect.w"];
		tempDestRect.h = j[currPlayerStr]["destRect.h"];
		tempSP.x = j[currPlayerStr]["shootPointX"] * renderSize;
		tempSP.y = j[currPlayerStr]["shootPointY"] * renderSize;
		bulletFilePath = j[currPlayerStr]["bulletFilePath"].dump();

		//tempSP.x = j[currPlayerStr]["shootingPointX"];
		//tempSP.y = j[currPlayerStr]["shootingPointY"];
		//std::string shootingPointDir = j[currPlayerStr]["bulletFilePath"];

		shipFilePath = j[currPlayerStr]["shipFilePath"].dump();
		//as the file path is written as "test\\file", we need to get rid of one of the two backslashes before attempting to read the location, as it won't work.
		//also, the file path begins and ends with a backslash due to the json reader. this will also be clipped off
		shipFilePath = shipFilePath.substr(1, shipFilePath.size()-2);
		for (size_t k = 0; k < shipFilePath.size(); k++)
		{
			if (shipFilePath[k] == '\\')
			{
				shipFilePath = shipFilePath.substr(0, k) + shipFilePath.substr(k + 1, 255);
				k++;
			}
		}

		bulletFilePath = bulletFilePath.substr(1, bulletFilePath.size() - 2);
		for (size_t k = 0; k < bulletFilePath.size(); k++)
		{
			if (bulletFilePath[k] == '\\')
			{
				bulletFilePath = bulletFilePath.substr(0, k) + bulletFilePath.substr(k + 1, 255);
				k++;
			}
		}
		EXGameObject* PlayerObj = new EXGameObject(tempSourceRect * renderSize, tempDestRect * renderSize, Vector2(tempSP.x, tempSP.y), shipFilePath, bulletFilePath, window, renderer);
		gameObjects.emplace_back(PlayerObj);


		//Now iterate through all events and actions in the data file and add the components to the player.
		int numEvents = j[currPlayerStr]["numEvents"];
		for (size_t k = 0; k < numEvents; k++)
		{
			std::string currEventStr = "Event" + std::to_string(k);
			EXEventComponentType eventType = j[currPlayerStr][currEventStr]["eventID"];
			EXEventComponent* tempEvent = nullptr;
			switch (eventType)
			{
			case EXThisHitByBullet:
				tempEvent = new EXOnThisHitByBullet();
				PlayerObj->AddComponent(tempEvent);
				break;
			case EXThisHitByPlayer:
				tempEvent = new EXOnThisHitByplayer();
				PlayerObj->AddComponent(tempEvent);
				break;
			case EXThisHitByEnemy:
				tempEvent = new EXOnThisHitByEnemy();
				PlayerObj->AddComponent(tempEvent);
				break;
			case EXWhenMoveUp:
				tempEvent = new EXOnMoveUp();
				PlayerObj->AddComponent(tempEvent);
				break;
			case EXWhenMoveDown:
				tempEvent = new EXOnMoveDown();
				PlayerObj->AddComponent(tempEvent);
				break;
			case EXWhenMoveLeft:
				tempEvent = new EXOnMoveLeft();
				PlayerObj->AddComponent(tempEvent);
				break;
			case EXWhenMoveRight:
				tempEvent = new EXOnMoveRight();
				PlayerObj->AddComponent(tempEvent);
				break;
			case EXWKeyPress:
				tempEvent = new EXOnKeyPress('w');
				PlayerObj->AddComponent(tempEvent);
				break;
			case EXSKeyPress:
				tempEvent = new EXOnKeyPress('s');
				PlayerObj->AddComponent(tempEvent);
				break;
			case EXAKeyPress:
				tempEvent = new EXOnKeyPress('a');
				PlayerObj->AddComponent(tempEvent);
				break;
			case EXDKeyPress:
				tempEvent = new EXOnKeyPress('d');
				PlayerObj->AddComponent(tempEvent);
				break;
			case EXSpaceKeyPress:
				tempEvent = new EXOnKeyPress(' ');
				PlayerObj->AddComponent(tempEvent);
				break;
			case EXEveryFrame:
				tempEvent = new EXOnEveryFrame();
				PlayerObj->AddComponent(tempEvent);
				break;
			default:
				break;
			}

			//now iterate through all actions and add them to the events
			int numActions = j[currPlayerStr][currEventStr]["numOfActions"];
			for (size_t l = 0; l < numActions; l++)
			{
				std::string currActionStr = "Action" + std::to_string(l);
				EXActionComponentType actionType = j[currPlayerStr][currEventStr][currActionStr]["ActionID"];
				EXActionComponent* tempAction = nullptr;

				//declaring a bunch of data in case they need to be used in the switch statement.
				std::string tempString;
				float tempFloat1, tempFloat2;
				int tempInt;

				switch (actionType)
				{
				case EXA_DoAnimation:
					tempString = j[currPlayerStr][currEventStr][currActionStr]["File"].dump();
					tempFloat1 = j[currPlayerStr][currEventStr][currActionStr]["destW"];
					tempFloat2 = j[currPlayerStr][currEventStr][currActionStr]["destH"];
					tempInt = j[currPlayerStr][currEventStr][currActionStr]["frames"];
					tempAction = new EXDoAnimation(tempString, tempInt, (int)tempFloat1, (int)tempFloat2);
					break;
				case EXA_MoveRelative:
					tempFloat1 = j[currPlayerStr][currEventStr][currActionStr]["X"];
					tempFloat2 = j[currPlayerStr][currEventStr][currActionStr]["Y"];
					tempAction = new EXMoveRelative(tempFloat1, tempFloat2);
					break;
				case EXA_SoundEffect:
					break;
				case EXA_ScoreLives:
					break;
				case EXA_Shoot:
					tempFloat1 = j[currPlayerStr][currEventStr][currActionStr]["shootSpeed"];
					tempFloat2 = j[currPlayerStr][currEventStr][currActionStr]["shootFreq"];
					tempAction = new EXShoot(tempFloat1, tempFloat2);
					break;
				case EXA_AddVelocity:
					tempFloat1 = j[currPlayerStr][currEventStr][currActionStr]["xVelocity"];
					tempFloat2 = j[currPlayerStr][currEventStr][currActionStr]["yVelocity"];
					tempAction = new EXAddVelocity(tempFloat1, tempFloat2);
					break;
				case EXA_SubtractVelocity:
					tempFloat1 = j[currPlayerStr][currEventStr][currActionStr]["xVelocity"];
					tempFloat2 = j[currPlayerStr][currEventStr][currActionStr]["yVelocity"];
					tempAction = new EXSubtractVelocity(tempFloat1, tempFloat2);
					break;
				case EXA_MultiplyVelocity:
					tempFloat1 = j[currPlayerStr][currEventStr][currActionStr]["xVelocity"];
					tempFloat2 = j[currPlayerStr][currEventStr][currActionStr]["yVelocity"];
					tempAction = new EXMultiplyVelocity(tempFloat1, tempFloat2);
					break;
				default:
					break;
				}

				if (tempAction) tempEvent->AddAction(tempAction);
			}
		}


	}

	int numEnemies = j["StartupInfo"]["EnemyCount"];
	for (size_t i = 0; i < numEnemies; i++)
	{
		std::string currEnemyStr = "Enemy" + std::to_string(i);

		SDL_Rect tempSourceRect;
		SDL_Rect tempDestRect;
		std::string shipFilePath = "";

		tempSourceRect.x = j[currEnemyStr]["XY.x"];
		tempSourceRect.y = j[currEnemyStr]["XY.y"];
		tempSourceRect.w = j[currEnemyStr]["XY.w"];
		tempSourceRect.h = j[currEnemyStr]["XY.h"];
		tempDestRect.x = j[currEnemyStr]["destRect.x"];
		tempDestRect.y = j[currEnemyStr]["destRect.y"];
		tempDestRect.w = j[currEnemyStr]["destRect.w"];
		tempDestRect.h = j[currEnemyStr]["destRect.h"];

		int tempRotation = j[currEnemyStr]["rotation"];

		Vector2 enemyMoveVec;
		enemyMoveVec.x = j[currEnemyStr]["xMoveVec"];
		enemyMoveVec.y = j[currEnemyStr]["yMoveVec"];

		shipFilePath = j[currEnemyStr]["shipFilePath"].dump();
		//as the file path is written as "test\\file", we need to get rid of one of the two backslashes before attempting to read the location, as it won't work.
		//also, the file path begins and ends with a backslash due to the json reader. this will also be clipped off
		shipFilePath = shipFilePath.substr(1, shipFilePath.size() - 2);
		for (size_t k = 0; k < shipFilePath.size(); k++)
		{
			if (shipFilePath[k] == '\\')
			{
				shipFilePath = shipFilePath.substr(0, k) + shipFilePath.substr(k + 1, 255);
				k++;
			}
		}

		EXGOEnemy* enemyObj = new EXGOEnemy(tempSourceRect * renderSize, tempDestRect * renderSize, shipFilePath, window, renderer, enemyMoveVec, tempRotation);
		gameObjects.emplace_back(enemyObj);

	}

	delete ifs;


}

void EXGameEngine::HandleEvents(SDL_Event * e)
{
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

bool EXGameEngine::Update()
{
	SDL_Event e;
	HandleEvents(&e);

	CapFrameRate();

	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Update();
	}

	return gameIsRunning;
}

void EXGameEngine::Draw()
{
	//SDL_RenderClear(renderer);

	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->Draw();
	}

	SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
	//SDL_RenderPresent(renderer);
}

void EXGameEngine::Clean()
{
}

void EXGameEngine::CapFrameRate()
{
	float control = (float)(1.0f / frameRate);
	control *= 1000;

	if (EventManager::dTime * 1000 < control)
	{
		MS delay = MS((int)(control - EventManager::dTime*1000));
		std::this_thread::sleep_for(delay);
	}

}
