#include "FileDialog.h"
#include <nlohmann/json.hpp>
#include <string.h>
#include "EXComponent.h"
using json = nlohmann::json;



FileDialog::FileDialog()
{
	exportLines = vector<string>();
}

void FileDialog::GetOpenFile()
{
	char targetFile[MAX_PATH];
	OPENFILENAME file;
	ifstream dataFile;
	string data;

	ZeroMemory(&targetFile, sizeof(targetFile));
	ZeroMemory(&file, sizeof(file));
	file.lStructSize = sizeof(file);
	file.hwndOwner = NULL; //window handler if needed
	file.lpstrFilter = "All Files\0*.*\0\0"; //filters what type of file you want to be able to load - this will have no filters for now - can filter it to text files if prefered
	file.lpstrFile = targetFile; //file userloads
	file.nMaxFile = MAX_PATH;
	file.lpstrTitle = "Select a File..."; //Dialog File title 

										  /*OFN_DONTADDTORECENT prevents the system from adding a link to the selected file in the
										  system directory that contains the users most recently used documents
										  OFN_FILEMUSTEXIST prevents the user from typing filenames of files that dont exist in the file name entry field
										  if user enters an invalid name the file dialog will display a warning in the message box*/
	file.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&file))
	{
		cout << "You chose the file \ " << targetFile << "\"\n"; //Gets file user open name
		dataFile.open(targetFile);

		if (dataFile.is_open())
		{
			while (getline(dataFile, data))
			{
				cout << data << '\n';
			}
			dataFile.close();
		}
		else
		{
			cout << "Unable to open file";
		}
	}
	else
	{
		CheckErrors();
	}
}

void FileDialog::SaveFile()
{
	char saveFile[MAX_PATH];
	OPENFILENAME file;

	ZeroMemory(&saveFile, sizeof(saveFile));
	ZeroMemory(&file, sizeof(file));
	file.lStructSize = sizeof(file);
	file.hwndOwner = NULL; //window handler if needed
	file.lpstrFilter = "All Files\0*.*\0\0"; //filters what type of file you want to be able to load - this will have no filters for now - can filter it to text files if prefered
	file.lpstrFile = saveFile; //file userloads
	file.nMaxFile = MAX_PATH;
	file.lpstrTitle = "Select a File..."; //Dialog File title 
										  /*OFN_DONTADDTORECENT prevents the system from adding a link to the selected file in the
										  system directory that contains the users most recently used documents
										  OFN_FILEMUSTEXIST prevents the user from typing filenames of files that dont exist in the file name entry field
										  if user enters an invalid name the file dialog will display a warning in the message box*/
	file.Flags = OFN_DONTADDTORECENT;



	if (GetSaveFileNameA(&file))
	{
		ofstream fileToOpen;

		//check if '.proj' is added at the end of the filename
		std::string fileName = saveFile;
		if (fileName[fileName.size() - 1] == 'j' && fileName[fileName.size() - 2] == 'o' && fileName[fileName.size() - 3] == 'r' && fileName[fileName.size() - 4] == 'p' && fileName[fileName.size() - 5] == '.') {}
		else
		{
			fileName.append(".proj");
		}
		for (int i = 0; i < fileName.size(); i++) { saveFile[i] = fileName[i]; }

		fileToOpen.open(saveFile);
		fileToOpen << "Writing to file...\n";
		fileToOpen << "WOOHOOOOOOO \n";
		fileToOpen.close();
		cout << " You saved this file \ " << saveFile << "\"\n"; //Gets file user open name

	}
	else
	{
		CheckErrors();
	}
}

void FileDialog::PrepBGAudioExport(string name, string filePath)
{
	string BGAudioString = "background audio|" + name + filePath;
	exportLines.push_back(BGAudioString);
}

void FileDialog::PrepAudioExport(string name, string filePath)
{
	string audioString = "audio|" + name + filePath;
	exportLines.push_back(audioString);
}

void FileDialog::PrepBGExport(string fileDir, int layerNumber, float speed)
{
	string fullPath = SDL_GetBasePath(); fullPath += fileDir;
	SDL_Surface* temp = IMG_Load(fullPath.c_str());

	string BGSpriteString = "Background|" + fileDir + "|" + std::to_string(temp->clip_rect.x) + "|" + std::to_string(temp->clip_rect.y) + "|" + std::to_string(temp->clip_rect.h) + "|" + std::to_string(temp->clip_rect.w) + "|" + std::to_string(temp->clip_rect.h) + "|" + std::to_string(temp->clip_rect.w) + "|" + std::to_string(layerNumber) + "|" + std::to_string(speed);
	exportLines.push_back(BGSpriteString);

	SDL_FreeSurface(temp);
}

void FileDialog::PrepPlayerExport(int health, bool enabled, int shootPointX, int shootPointY, float shootingSpeed, SHOOTING_TYPE shootingType, string shotFilePath, SDL_Rect XY, string shotSoundName, float shotDamage, string deathSoundName, string shipFilePath, SDL_Rect destRect, float shipSpeed)
{
	//convert shooting point enum to string
	string shootingTypeNEW;
	switch (shootingType)
	{
	case Single:
		shootingTypeNEW = "Single";
		break;
	case Dual_Split:
		shootingTypeNEW = "Dual_Split";
		break;
	case Dual_Spread:
		shootingTypeNEW = "Dual_Spread";
		break;
	default:
		shootingTypeNEW = "NULLERR";
		break;
	}

	string fullPath = SDL_GetBasePath();
	if (shipFilePath[0] == 't') shipFilePath = shipFilePath.substr(1, 255);
	fullPath.append(shipFilePath);
	SDL_Surface* temp = IMG_Load(fullPath.c_str());
	SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGBA8888, 0);

	string playerString = "player|" + std::to_string(health) + "|" + ((enabled) ? "true" : "false") + "|" + std::to_string(shootPointX) + "|" + std::to_string(shootPointY) + "|" + std::to_string((int)shootingSpeed) + "|" + shootingTypeNEW + "|" + shotFilePath + "|" + std::to_string(XY.x) + "|" + std::to_string(XY.y) + "|" + std::to_string(XY.w) + "|" + std::to_string(XY.h) + "|" + shotSoundName + "|" + std::to_string((int)shotDamage) + "|" + "null" + "|" + deathSoundName + "|" + shipFilePath + "|" + std::to_string(temp->clip_rect.x) + "|" + std::to_string(temp->clip_rect.y) + "|" + std::to_string(temp->clip_rect.h) + "|" + std::to_string(temp->clip_rect.w) + "|" + std::to_string(destRect.h) + "|" + std::to_string(destRect.w) + "|" + std::to_string((int)shipSpeed);
	int asd = 0;
	exportLines.push_back(playerString);

	SDL_FreeSurface(temp);
}

void FileDialog::PrepDebugFileLoc()
{
	exportLoc = "Assets\\debug.JSON";
}

void FileDialog::ShowDialogPrepFileLoc()
{
	//show a file dialog to get the save directory to be used for other functions.
	char saveFile[MAX_PATH];
	OPENFILENAME file;

	ZeroMemory(&saveFile, sizeof(saveFile));
	ZeroMemory(&file, sizeof(file));
	file.lStructSize = sizeof(file);
	file.hwndOwner = NULL; //window handler if needed
	file.lpstrFilter = ".JSON\0*.*\0\0"; //filters what type of file you want to be able to load - this will have no filters for now - can filter it to text files if prefered
	file.lpstrFile = saveFile; //file userloads
	file.nMaxFile = MAX_PATH;
	file.lpstrTitle = "Select a File..."; //Dialog File title 
										  /*OFN_DONTADDTORECENT prevents the system from adding a link to the selected file in the
										  system directory that contains the users most recently used documents
										  OFN_FILEMUSTEXIST prevents the user from typing filenames of files that dont exist in the file name entry field
										  if user enters an invalid name the file dialog will display a warning in the message box*/
	file.Flags = OFN_DONTADDTORECENT;


	if (GetSaveFileName(&file))
	{
		ofstream fileToOpen;

		exportLoc = (saveFile);
		exportLoc += ".JSON";
	}


}

void FileDialog::ShowDialogPrepReadLoc()
{
	//show a file dialog to get the save directory to be used for other functions.
	char openFile[MAX_PATH];
	OPENFILENAME file;

	ZeroMemory(&openFile, sizeof(openFile));
	ZeroMemory(&file, sizeof(file));
	file.lStructSize = sizeof(file);
	file.hwndOwner = NULL; //window handler if needed
	file.lpstrFilter = ".Proj\0*.*\0\0"; //filters what type of file you want to be able to load - this will have no filters for now - can filter it to text files if prefered
	file.lpstrFile = openFile; //file userloads
	file.nMaxFile = MAX_PATH;
	file.lpstrTitle = "Select a File..."; //Dialog File title 
										  /*OFN_DONTADDTORECENT prevents the system from adding a link to the selected file in the
										  system directory that contains the users most recently used documents
										  OFN_FILEMUSTEXIST prevents the user from typing filenames of files that dont exist in the file name entry field
										  if user enters an invalid name the file dialog will display a warning in the message box*/
	file.Flags = OFN_DONTADDTORECENT;


	if (GetSaveFileName(&file))
	{
		ifstream fileToOpen;

		exportLoc = (openFile);
	}
}

void FileDialog::AddPlayerToJson(int health, int shootPointX, int shootPointY, SDL_Rect XY, string bulletFilePath, string shipFilePath, SDL_Rect destRect, float shipSpeed, std::vector<EXEventComponent*> inputEvents)
{
	//first, create a temp object to add all the data into, and then add the temp object to the player data vector
	PlayerSaveStruct temp;

	temp.health = health;
	temp.shootPointX = shootPointX;
	temp.shootPointY = shootPointY;
	temp.bulletFilePath = bulletFilePath;
	temp.XY = XY;
	temp.shipFilePath = shipFilePath;
	temp.destRect = destRect;
	temp.shipSpeed = shipSpeed;
	temp.events = inputEvents;
	//now that all data has been added, add to the player data vector
	playerData.emplace_back(temp);
}

void FileDialog::AddEnemyToJson(int health, int shootPointX, int shootPointY, SDL_Rect XY, int rotation, string shipFilePath, SDL_Rect destRect, float shipSpeed, float xMoveVec, float yMoveVec)
{
	//first, create a temp object to add all the data into, and then add the temp object to the player data vector
	EnemySaveStruct temp;

	temp.health = health;
	temp.shootPointX = shootPointX;
	temp.shootPointY = shootPointY;
	temp.XY = XY;
	temp.rotation = rotation;
	temp.shipFilePath = shipFilePath;
	temp.destRect = destRect;
	temp.shipSpeed = shipSpeed;
	temp.xMoveVec = xMoveVec;
	temp.yMoveVec = yMoveVec;

	//now that all data has been added, add to the enemy data vector
	enemyData.emplace_back(temp);
}

void FileDialog::AddBackgroundToJson(SDL_Rect inputDestRect, string inputFileDir, float inputScrollSpeed)
{
	BackgroundSaveStruct temp;

	temp.destRect = inputDestRect;
	temp.fileDir = inputFileDir;
	temp.scrollSpeed = inputScrollSpeed;

	backgroundData.emplace_back(temp);
}

void FileDialog::ExportJson(int screenWidth, int screenHeight)
{
	ofstream ofs;

	if (exportLoc == "Assets\\debug.JSON")
	{
		ofs.open(SDL_GetBasePath() + exportLoc);
	}
	else
	{
		ofs.open(exportLoc);
	}


	json j;

	int numOfPlayers = playerData.size();
	int numOfEnemies = enemyData.size();
	int numOfBackgrounds = backgroundData.size();

	j["StartupInfo"]["PlayerCount"] = numOfPlayers;
	j["StartupInfo"]["EnemyCount"] = numOfEnemies;
	j["StartupInfo"]["BackgroundCount"] = numOfBackgrounds;
	j["StartupInfo"]["ScreenWidth"] = screenWidth;
	j["StartupInfo"]["ScreenHeight"] = screenHeight;

	for (int i = 0; i < numOfPlayers; i++)
	{
		string playerString = "Player" + to_string(i);
		j[playerString]["health"] = playerData[i].health;
		j[playerString]["shootPointX"] = playerData[i].shootPointX;
		j[playerString]["shootPointY"] = playerData[i].shootPointY;
		j[playerString]["XY.x"] = playerData[i].XY.x;
		j[playerString]["XY.y"] = playerData[i].XY.y;
		j[playerString]["XY.w"] = playerData[i].XY.w;
		j[playerString]["XY.h"] = playerData[i].XY.h;
		j[playerString]["bulletFilePath"] = playerData[i].bulletFilePath;
		j[playerString]["shipFilePath"] = playerData[i].shipFilePath;
		j[playerString]["destRect.x"] = playerData[i].destRect.x;
		j[playerString]["destRect.y"] = playerData[i].destRect.y;
		j[playerString]["destRect.w"] = playerData[i].destRect.w;
		j[playerString]["destRect.h"] = playerData[i].destRect.h;
		j[playerString]["shipSpeed"] = playerData[i].shipSpeed;
		int numEvents = playerData[i].events.size();
		j[playerString]["numEvents"] = numEvents;
		for (size_t k = 0; k < numEvents; k++)
		{
			string eventString = "Event" + to_string(k);
			j[playerString][eventString]["eventID"] = playerData[i].events[k]->type;
			int actionNum = playerData[i].events[k]->actions.size();
			j[playerString][eventString]["numOfActions"] = actionNum;
			for (size_t l = 0; l < actionNum; l++)
			{
				string actionString = "Action" + to_string(l);
				j[playerString][eventString][actionString]["ActionID"] = playerData[i].events[k]->actions[l]->type;
				//do this
				//if (current action type is e.g.shoot)
				//write how fast the bullet is to file
				//if it isnt shoot, write the necessary data to file.
				if (playerData[i].events[k]->actions[l]->type == EXActionComponentType::EXA_Shoot)
				{
					j[playerString][eventString][actionString]["shootSpeed"] = ((EXShoot*)playerData[i].events[k]->actions[l])->bulletSpeed;
					j[playerString][eventString][actionString]["shootFreq"] = ((EXShoot*)playerData[i].events[k]->actions[l])->shootFrequency;
				}
				else if (playerData[i].events[k]->actions[l]->type == EXActionComponentType::EXA_DoAnimation)
				{
					j[playerString][eventString][actionString]["File"] = ((EXDoAnimation*)playerData[i].events[k]->actions[l])->fileDir;
					j[playerString][eventString][actionString]["Frames"] = ((EXDoAnimation*)playerData[i].events[k]->actions[l])->frames;
				}
				else if (playerData[i].events[k]->actions[l]->type == EXActionComponentType::EXA_MoveRelative)
				{
					j[playerString][eventString][actionString]["X"] = ((EXMoveRelative*)playerData[i].events[k]->actions[l])->X;
					j[playerString][eventString][actionString]["Y"] = ((EXMoveRelative*)playerData[i].events[k]->actions[l])->Y;
				}
				else if (playerData[i].events[k]->actions[l]->type == EXActionComponentType::EXA_ScoreLives)
				{
					j[playerString][eventString][actionString]["Score"] = ((EXScoreAndLives*)playerData[i].events[k]->actions[l])->playerScore;
					j[playerString][eventString][actionString]["Lives"] = ((EXScoreAndLives*)playerData[i].events[k]->actions[l])->playerLives;
				}
				else if (playerData[i].events[k]->actions[l]->type == EXActionComponentType::EXA_SoundEffect)
				{
					j[playerString][eventString][actionString]["File"] = ((EXAddSoundEffect*)playerData[i].events[k]->actions[l])->fileDir;
				}
				else if (playerData[i].events[k]->actions[l]->type == EXActionComponentType::EXA_AddVelocity)
				{
					j[playerString][eventString][actionString]["xVelocity"] = ((EXAddVelocity*)playerData[i].events[k]->actions[l])->xVel;
					j[playerString][eventString][actionString]["yVelocity"] = ((EXAddVelocity*)playerData[i].events[k]->actions[l])->yVel;
				}
				else if (playerData[i].events[k]->actions[l]->type == EXActionComponentType::EXA_SubtractVelocity)
				{
					j[playerString][eventString][actionString]["xVelocity"] = ((EXAddVelocity*)playerData[i].events[k]->actions[l])->xVel;
					j[playerString][eventString][actionString]["yVelocity"] = ((EXAddVelocity*)playerData[i].events[k]->actions[l])->yVel;
				}
				else if (playerData[i].events[k]->actions[l]->type == EXActionComponentType::EXA_MultiplyVelocity)
				{
					j[playerString][eventString][actionString]["xVelocity"] = ((EXAddVelocity*)playerData[i].events[k]->actions[l])->xVel;
					j[playerString][eventString][actionString]["yVelocity"] = ((EXAddVelocity*)playerData[i].events[k]->actions[l])->yVel;
				}
			}
		}
	}
	for (int i = 0; i < numOfEnemies; i++)
	{
		string enemyString = "Enemy" + to_string(i);
		j[enemyString]["health"] = enemyData[i].health;
		j[enemyString]["shootPointX"] = enemyData[i].shootPointX;
		j[enemyString]["shootPointY"] = enemyData[i].shootPointY;
		j[enemyString]["XY.x"] = enemyData[i].XY.x;
		j[enemyString]["XY.y"] = enemyData[i].XY.y;
		j[enemyString]["XY.w"] = enemyData[i].XY.w;
		j[enemyString]["XY.h"] = enemyData[i].XY.h;
		j[enemyString]["shipFilePath"] = enemyData[i].shipFilePath;
		j[enemyString]["destRect.x"] = enemyData[i].destRect.x;
		j[enemyString]["destRect.y"] = enemyData[i].destRect.y;
		j[enemyString]["destRect.w"] = enemyData[i].destRect.w;
		j[enemyString]["destRect.h"] = enemyData[i].destRect.h;
		j[enemyString]["shipSpeed"] = enemyData[i].shipSpeed;
		j[enemyString]["xMoveVec"] = enemyData[i].xMoveVec;
		j[enemyString]["yMoveVec"] = enemyData[i].yMoveVec;
		j[enemyString]["rotation"] = enemyData[i].rotation;
	}
	for (int i = 0; i < numOfBackgrounds; i++)
	{
		string enemyString = "Background" + to_string(i);
		j[enemyString]["destRect.x"] = backgroundData[i].destRect.x;
		j[enemyString]["destRect.y"] = backgroundData[i].destRect.y;
		j[enemyString]["destRect.w"] = backgroundData[i].destRect.w;
		j[enemyString]["destRect.h"] = backgroundData[i].destRect.h;
		j[enemyString]["fileDir"] = backgroundData[i].fileDir;
		j[enemyString]["scrollSpeed"] = backgroundData[i].scrollSpeed;
	}

	ofs << j.dump(1);

	ofs.close();

	//wipe the list of entities, so when writing to file again, there are no buggeroonies
	playerData.clear();
	enemyData.clear();
	backgroundData.clear();

}

void FileDialog::SaveProject(int screenWidth, int screenHeight)
{
	ofstream ofs;

	ofs.open(exportLoc);

	json j;

	int numOfPlayers = playerData.size();
	int numOfEnemies = enemyData.size();
	int numOfBackgrounds = backgroundData.size();

	j["StartupInfo"]["PlayerCount"] = numOfPlayers;
	j["StartupInfo"]["EnemyCount"] = numOfEnemies;
	j["StartupInfo"]["BackgroundCount"] = numOfBackgrounds;
	j["StartupInfo"]["ScreenWidth"] = screenWidth;
	j["StartupInfo"]["ScreenHeight"] = screenHeight;

	for (int i = 0; i < numOfPlayers; i++)
	{
		string playerString = "Player" + to_string(i);
		j[playerString]["health"] = playerData[i].health;
		j[playerString]["shootPointX"] = playerData[i].shootPointX;
		j[playerString]["shootPointY"] = playerData[i].shootPointY;
		j[playerString]["XY.x"] = playerData[i].XY.x;
		j[playerString]["XY.y"] = playerData[i].XY.y;
		j[playerString]["XY.w"] = playerData[i].XY.w;
		j[playerString]["XY.h"] = playerData[i].XY.h;
		j[playerString]["bulletFilePath"] = playerData[i].bulletFilePath;
		j[playerString]["shipFilePath"] = playerData[i].shipFilePath;
		j[playerString]["destRect.x"] = playerData[i].destRect.x;
		j[playerString]["destRect.y"] = playerData[i].destRect.y;
		j[playerString]["destRect.w"] = playerData[i].destRect.w;
		j[playerString]["destRect.h"] = playerData[i].destRect.h;
		j[playerString]["shipSpeed"] = playerData[i].shipSpeed;
		int numEvents = playerData[i].events.size();
		j[playerString]["numEvents"] = numEvents;
		for (size_t k = 0; k < numEvents; k++)
		{
			string eventString = "Event" + to_string(k);
			j[playerString][eventString]["eventID"] = playerData[i].events[k]->type;
			int actionNum = playerData[i].events[k]->actions.size();
			j[playerString][eventString]["numOfActions"] = actionNum;
			for (size_t l = 0; l < actionNum; l++)
			{
				string actionString = "Action" + to_string(l);
				j[playerString][eventString][actionString]["ActionID"] = playerData[i].events[k]->actions[l]->type;
				//do this
				//if (current action type is e.g.shoot)
				//write how fast the bullet is to file
				//if it isnt shoot, write the necessary data to file.
				if (playerData[i].events[k]->actions[l]->type == EXActionComponentType::EXA_Shoot)
				{
					j[playerString][eventString][actionString]["shootSpeed"] = ((EXShoot*)playerData[i].events[k]->actions[l])->bulletSpeed;
					j[playerString][eventString][actionString]["shootFreq"] = ((EXShoot*)playerData[i].events[k]->actions[l])->shootFrequency;
				}
				else if (playerData[i].events[k]->actions[l]->type == EXActionComponentType::EXA_DoAnimation)
				{
					j[playerString][eventString][actionString]["File"] = ((EXDoAnimation*)playerData[i].events[k]->actions[l])->fileDir;
					j[playerString][eventString][actionString]["Frames"] = ((EXDoAnimation*)playerData[i].events[k]->actions[l])->frames;
				}
				else if (playerData[i].events[k]->actions[l]->type == EXActionComponentType::EXA_MoveRelative)
				{
					j[playerString][eventString][actionString]["X"] = ((EXMoveRelative*)playerData[i].events[k]->actions[l])->X;
					j[playerString][eventString][actionString]["Y"] = ((EXMoveRelative*)playerData[i].events[k]->actions[l])->Y;
				}
				else if (playerData[i].events[k]->actions[l]->type == EXActionComponentType::EXA_ScoreLives)
				{
					j[playerString][eventString][actionString]["Score"] = ((EXScoreAndLives*)playerData[i].events[k]->actions[l])->playerScore;
					j[playerString][eventString][actionString]["Lives"] = ((EXScoreAndLives*)playerData[i].events[k]->actions[l])->playerLives;
				}
				else if (playerData[i].events[k]->actions[l]->type == EXActionComponentType::EXA_SoundEffect)
				{
					j[playerString][eventString][actionString]["File"] = ((EXAddSoundEffect*)playerData[i].events[k]->actions[l])->fileDir;
				}
				else if (playerData[i].events[k]->actions[l]->type == EXActionComponentType::EXA_AddVelocity)
				{
					j[playerString][eventString][actionString]["xVelocity"] = ((EXAddVelocity*)playerData[i].events[k]->actions[l])->xVel;
					j[playerString][eventString][actionString]["yVelocity"] = ((EXAddVelocity*)playerData[i].events[k]->actions[l])->yVel;
				}
				else if (playerData[i].events[k]->actions[l]->type == EXActionComponentType::EXA_SubtractVelocity)
				{
					j[playerString][eventString][actionString]["xVelocity"] = ((EXAddVelocity*)playerData[i].events[k]->actions[l])->xVel;
					j[playerString][eventString][actionString]["yVelocity"] = ((EXAddVelocity*)playerData[i].events[k]->actions[l])->yVel;
				}
				else if (playerData[i].events[k]->actions[l]->type == EXActionComponentType::EXA_MultiplyVelocity)
				{
					j[playerString][eventString][actionString]["xVelocity"] = ((EXAddVelocity*)playerData[i].events[k]->actions[l])->xVel;
					j[playerString][eventString][actionString]["yVelocity"] = ((EXAddVelocity*)playerData[i].events[k]->actions[l])->yVel;
				}
			}
		}
	}
	for (int i = 0; i < numOfEnemies; i++)
	{
		string enemyString = "Enemy" + to_string(i);
		j[enemyString]["health"] = enemyData[i].health;
		j[enemyString]["shootPointX"] = enemyData[i].shootPointX;
		j[enemyString]["shootPointY"] = enemyData[i].shootPointY;
		j[enemyString]["XY.x"] = enemyData[i].XY.x;
		j[enemyString]["XY.y"] = enemyData[i].XY.y;
		j[enemyString]["XY.w"] = enemyData[i].XY.w;
		j[enemyString]["XY.h"] = enemyData[i].XY.h;
		j[enemyString]["shipFilePath"] = enemyData[i].shipFilePath;
		j[enemyString]["destRect.x"] = enemyData[i].destRect.x;
		j[enemyString]["destRect.y"] = enemyData[i].destRect.y;
		j[enemyString]["destRect.w"] = enemyData[i].destRect.w;
		j[enemyString]["destRect.h"] = enemyData[i].destRect.h;
		j[enemyString]["shipSpeed"] = enemyData[i].shipSpeed;
		j[enemyString]["xMoveVec"] = enemyData[i].xMoveVec;
		j[enemyString]["yMoveVec"] = enemyData[i].yMoveVec;
		j[enemyString]["rotation"] = enemyData[i].rotation;
	}
	for (int i = 0; i < numOfBackgrounds; i++)
	{
		string enemyString = "Background" + to_string(i);
		j[enemyString]["destRect.x"] = backgroundData[i].destRect.x;
		j[enemyString]["destRect.y"] = backgroundData[i].destRect.y;
		j[enemyString]["destRect.w"] = backgroundData[i].destRect.w;
		j[enemyString]["destRect.h"] = backgroundData[i].destRect.h;
		j[enemyString]["fileDir"] = backgroundData[i].fileDir;
		j[enemyString]["scrollSpeed"] = backgroundData[i].scrollSpeed;
	}

	ofs << j.dump(1);

	ofs.close();

	//wipe the list of entities, so when writing to file again, there are no buggeroonies
	playerData.clear();
	enemyData.clear();
	backgroundData.clear();

}

void FileDialog::PrepWaveExport(int health, bool enabled, int shootPointX, int shootPointY, float shootingSpeed, SHOOTING_TYPE shootingType, string shotSpriteName, SDL_Rect shotDestRect, string shotSoundName, float shotDamage, float quantityFrom, float quantityTo, FORMATION formation, MOVE_PATTERN movePattern, float speedFrom, float speedTo, float crashDamage, float startPos, float killPoints, SDL_Rect eDestRect)
{
	//convert shooting point enum to string
	string shootingTypeNEW;
	string formationNEW;
	string patternNEW;
	switch (shootingType)
	{
	case Single:
		shootingTypeNEW = "Single";
		break;
	case Dual_Split:
		shootingTypeNEW = "Dual_Split";
		break;
	case Dual_Spread:
		shootingTypeNEW = "Dual_Spread";
		break;
	default:
		shootingTypeNEW = "NULLERR";
		break;
	}
	switch (formation)
	{
	case stright:
		formationNEW = "stright";
		break;
	case diaginalDown:
		formationNEW = "diaginalDown";
		break;
	case diaginalUp:
		formationNEW = "diaginalUp";
		break;
	case zigZagUp:
		formationNEW = "zigZagUp";
		break;
	case zigZagDown:
		formationNEW = "zigZagDown";
		break;

	default:
		shootingTypeNEW = "NULLERR";
		break;
	}
	switch (movePattern)
	{
	case slow:
		patternNEW = "slow";
		break;
	case fast:
		patternNEW = "fast";
		break;
	default:
		patternNEW = "NULLERR";
		break;
	}

	string enemyString = "wave|" + std::to_string(health) + "|" + ((enabled) ? "true" : "false") + "|" + std::to_string(shootPointX) + "|" + std::to_string(shootPointY) + "|" + std::to_string((int)shootingSpeed) + "|" + shootingTypeNEW + "|" + shotSpriteName + "|" + std::to_string(shotDestRect.x) + "|" + std::to_string(shotDestRect.y) + "|" + std::to_string(shotDestRect.h) + "|" + std::to_string(shotDestRect.w) + "|" + shotSoundName + "|" + std::to_string((int)shotDamage) + "|" + std::to_string((int)quantityFrom) + "|" + std::to_string((int)quantityTo) + "|" + formationNEW + "|" + patternNEW + "|" + std::to_string((int)speedFrom) + "|" + std::to_string((int)speedTo) + "|" + std::to_string((int)crashDamage) + "|" + std::to_string((int)startPos) + "|" + std::to_string((int)killPoints) + "|" + std::to_string(eDestRect.x) + "|" + std::to_string(eDestRect.y) + "|" + std::to_string(eDestRect.h) + "|" + std::to_string(eDestRect.w);
	exportLines.push_back(enemyString);
}

void FileDialog::ExportFile(string title, bool orientation)
{
	char saveFile[MAX_PATH];
	OPENFILENAME file;

	ZeroMemory(&saveFile, sizeof(saveFile));
	ZeroMemory(&file, sizeof(file));
	file.lStructSize = sizeof(file);
	file.hwndOwner = NULL; //window handler if needed
	file.lpstrFilter = "All Files\0*.*\0\0"; //filters what type of file you want to be able to load - this will have no filters for now - can filter it to text files if prefered
	file.lpstrFile = saveFile; //file userloads
	file.nMaxFile = MAX_PATH;
	file.lpstrTitle = "Select a File..."; //Dialog File title 
										  /*OFN_DONTADDTORECENT prevents the system from adding a link to the selected file in the
										  system directory that contains the users most recently used documents
										  OFN_FILEMUSTEXIST prevents the user from typing filenames of files that dont exist in the file name entry field
										  if user enters an invalid name the file dialog will display a warning in the message box*/
	file.Flags = OFN_DONTADDTORECENT;



	if (GetSaveFileNameA(&file))
	{
		ofstream fileToOpen;

		//check if '.proj' is added at the end of the filename
		std::string fileName = saveFile;

		for (int i = 0; i < fileName.size(); i++) { saveFile[i] = fileName[i]; }

		fileToOpen.open(saveFile);

		fileToOpen << "title|" << title << "\n";
		fileToOpen << "orintation|";
		if (orientation) { fileToOpen << "true"; }
		else { fileToOpen << "false"; }

		fileToOpen << "\n";

		for (unsigned int i = 0; i < exportLines.size(); i++) { fileToOpen << exportLines[i].c_str() << "\n"; }

		fileToOpen.close();
	}
	else
	{
		CheckErrors();
	}

}

string FileDialog::GetFile()
{
	char file[MAX_PATH];
	OPENFILENAME fileDialog;
	ifstream spriteFile;


	ZeroMemory(&file, sizeof(file));
	ZeroMemory(&fileDialog, sizeof(fileDialog));
	fileDialog.lStructSize = sizeof(fileDialog);
	fileDialog.hwndOwner = NULL; //window handler if needed
	fileDialog.lpstrFilter = "All Files\0*.*\0\0"; //filters what type of file you want to be able to load - this will have no filters for now - can filter it to text files if prefered
	fileDialog.lpstrFile = file; //file userloads
	fileDialog.nMaxFile = MAX_PATH;
	fileDialog.lpstrTitle = "Select a File..."; //Dialog File title 

												/*OFN_DONTADDTORECENT prevents the system from adding a link to the selected file in the
												system directory that contains the users most recently used documents
												OFN_FILEMUSTEXIST prevents the user from typing filenames of files that dont exist in the file name entry field
												if user enters an invalid name the file dialog will display a warning in the message box*/
	fileDialog.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&fileDialog))
	{
		cout << "You chose the file \ " << file << "\"\n"; //Gets file user open name
		spriteFile.open(file);
	}
	else
	{
		CheckErrors();
	}
	return file;
}

void FileDialog::OpenProject(std::vector<BackgroundSaveStruct*>& bgOBJ, std::vector<PlayerSaveStruct*>& playerOBJ, std::vector<EnemySaveStruct*>& enemyOBJ)
{
		std::ifstream ifs(exportLoc);

		json j;
		j = json::parse(ifs);

		int numBackgrounds = j["StartupInfo"]["BackgroundCount"];
		for (size_t i = 0; i < numBackgrounds; i++)
		{
			std::string currBGStr = "Background" + std::to_string(i);

			float tempScrollSpeed;
			SDL_Rect tempDestRect;
			std::string filePath = "";

			tempDestRect.x = j[currBGStr]["destRect.x"];
			tempDestRect.y = j[currBGStr]["destRect.y"];
			tempDestRect.w = j[currBGStr]["destRect.w"];;
			tempDestRect.h = j[currBGStr]["destRect.h"];;

			tempScrollSpeed = j[currBGStr]["scrollSpeed"];


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

			BackgroundSaveStruct* tempOBJ = new BackgroundSaveStruct();
			tempOBJ->destRect = tempDestRect;
			tempOBJ->fileDir = filePath;
			tempOBJ->scrollSpeed = tempScrollSpeed;

			bgOBJ.emplace_back(tempOBJ);

		}
		int numPlayers = j["StartupInfo"]["PlayerCount"];
		for (size_t i = 0; i < numPlayers; i++)
		{
			std::string currPlayerStr = "Player" + std::to_string(i);

			SDL_Rect tempSourceRect;
			SDL_Rect tempDestRect;
			float shootPointX, shootPointY;
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
			shootPointX = j[currPlayerStr]["shootPointX"];
			shootPointY = j[currPlayerStr]["shootPointY"];
			bulletFilePath = j[currPlayerStr]["bulletFilePath"].dump();


			shipFilePath = j[currPlayerStr]["shipFilePath"].dump();

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

			bulletFilePath = bulletFilePath.substr(1, bulletFilePath.size() - 2);
			for (size_t k = 0; k < bulletFilePath.size(); k++)
			{
				if (bulletFilePath[k] == '\\')
				{
					bulletFilePath = bulletFilePath.substr(0, k) + bulletFilePath.substr(k + 1, 255);
					k++;
				}
			}

			PlayerSaveStruct* tempOBJ = new PlayerSaveStruct();
			tempOBJ->bulletFilePath = bulletFilePath;
			tempOBJ->destRect = tempDestRect;
			tempOBJ->shootPointX = shootPointX;
			tempOBJ->shootPointY = shootPointY;
			tempOBJ->destRect = tempDestRect;
			tempOBJ->shipFilePath = shipFilePath;
			


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
					tempOBJ->events.emplace_back(tempEvent);
					break;
				case EXThisHitByPlayer:
					tempEvent = new EXOnThisHitByplayer();
					tempOBJ->events.emplace_back(tempEvent);
					break;
				case EXThisHitByEnemy:
					tempEvent = new EXOnThisHitByEnemy();
					tempOBJ->events.emplace_back(tempEvent);
					break;
				case EXWhenMoveUp:
					tempEvent = new EXOnMoveUp();
					tempOBJ->events.emplace_back(tempEvent);
					break;
				case EXWhenMoveDown:
					tempEvent = new EXOnMoveDown();
					tempOBJ->events.emplace_back(tempEvent);
					break;
				case EXWhenMoveLeft:
					tempEvent = new EXOnMoveLeft();
					tempOBJ->events.emplace_back(tempEvent);
					break;
				case EXWhenMoveRight:
					tempEvent = new EXOnMoveRight();
					tempOBJ->events.emplace_back(tempEvent);
					break;
				case EXWKeyPress:
					tempEvent = new EXOnKeyPress('w');
					tempOBJ->events.emplace_back(tempEvent);
					break;
				case EXSKeyPress:
					tempEvent = new EXOnKeyPress('s');
					tempOBJ->events.emplace_back(tempEvent);
					break;
				case EXAKeyPress:
					tempEvent = new EXOnKeyPress('a');
					tempOBJ->events.emplace_back(tempEvent);
					break;
				case EXDKeyPress:
					tempEvent = new EXOnKeyPress('d');
					tempOBJ->events.emplace_back(tempEvent);
					break;
				case EXSpaceKeyPress:
					tempEvent = new EXOnKeyPress(' ');
					tempOBJ->events.emplace_back(tempEvent);
					break;
				case EXEveryFrame:
					tempEvent = new EXOnEveryFrame();
					tempOBJ->events.emplace_back(tempEvent);
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

			playerOBJ.emplace_back(tempOBJ);

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

			float enemyMoveVecX, enemyMoveVecY;
			enemyMoveVecX = j[currEnemyStr]["xMoveVec"];
			enemyMoveVecY = j[currEnemyStr]["yMoveVec"];

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

			EnemySaveStruct* tempOBJ = new EnemySaveStruct();
			tempOBJ->destRect = tempDestRect;
			tempOBJ->rotation = tempRotation;
			tempOBJ->shipFilePath = shipFilePath;
			tempOBJ->xMoveVec = enemyMoveVecX;
			tempOBJ->yMoveVec = enemyMoveVecY;

			enemyOBJ.emplace_back(tempOBJ);

		}
}

void FileDialog::CheckErrors()
{
	//this is where all the checks will be -  this will narrow any errors down to a user cancellation if none other errors occur.
	switch (CommDlgExtendedError())
	{
	default: cout << "You Cancelled.\n";
		//Dialog box could not be created - happens when dialog box call specifies an invalid window handle
	case CDERR_DIALOGFAILURE: cout << "CDERR_DIALOG FAILURE: 0xFFFF. Dialog Box could not be loaded."; break;
		//Dialog Box cant find a specified resource
	case CDERR_FINDRESFAILURE: cout << "CDERR_FINDRESFAILURE: 0x0006. Dialog Box failed to find a specific resource."; break;
		//Dialog Box function failed during initailisation. Suffiecient Memory not available.
	case CDERR_INITIALIZATION: cout << "CDERR_INITIALIZATION: 0x0002. Dialog Box failed to initalise."; break;
		//Dialog Box function failed to load the specified resource
	case CDERR_LOADRESFAILURE: cout << "CDERR_LOADRESFAILURE: 0x0007. Dialog Box failed to load specifed resource."; break;
		//Dialog Box failes to load a specific string
	case CDERR_LOADSTRFAILURE: cout << "CDERR_LOADSTRFAILURE: 0x0005. Dialog Box failed to load a specific string"; break;
	case CDERR_LOCKRESFAILURE: cout << "CDERR_LOCKRESFAILURE: 0x0008. Dialog Box failed to lock a specific resource"; break;
	case CDERR_MEMALLOCFAILURE: cout << "CDERR_MEMALLOCFAILURE: 0x0009. Dialog Box unable to allocate the memory associated with the handle"; break;
	case CDERR_MEMLOCKFAILURE: cout << "CDERR_MEMLOCKFAILURE: 0x000A. Dialog Box unable to lock the memory associated with a handle"; break;
	case CDERR_NOHINSTANCE: cout << "CDERR_NOHINSTANCE: 0x0004. The ENABLETEMPLATE flag was set but failed to provide a corresponding instance handle"; break;
	case CDERR_NOHOOK: cout << "CDERR_NOHOOK: 0x000B. Enable Hook flag was set in the flags member but you failed to provide a pointer to a corresponding hook procedure"; break;
	case CDERR_REGISTERMSGFAIL: cout << "CDERR_REGISTERMSGFAIL: 0x000C. Register Window Message function returned an error code when it was called by the dialog box function"; break;
	case CDERR_STRUCTSIZE: cout << "CDERR_STRUCTSIZE: 0x0001. IStructSize member of the intialisation structure for the corresponding common dialog box is invalid."; break;
		//Following error codes can be returned for the GetOpenFileName and GetSaveFileName fucntions.

		/*Buffer pointed to by the IPSTRFile member of OPENFILENAME structure is too small for the file name specified by user First 2 bytesof the PSTRFile buffer
		contain an integer value specifying the size required to recieve the full name in characters*/
	case FNERR_BUFFERTOOSMALL: cout << "FNERR_BUFFERTOOSMALL: 0x3003. buffer pointed to the file is too small."; break;
	case FNERR_INVALIDFILENAME: cout << "FNERR_INVALIDFILENAME: 0x3002. File name is invalid."; break;
		//Attempt to subclass a list box failed due to sufficent memory not being available.
	case FNERR_SUBCLASSFAILURE: cout << "FNERR_SUBCLASSFAILURE: 0x3001. Attempt to subclass a list box failed."; break;
	}
}
FileDialog::~FileDialog()
{
}
