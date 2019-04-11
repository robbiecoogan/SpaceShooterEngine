#pragma once
#include "SDL.h"
#include "SDL_TTF.h"
#include "UIElement.h"
#include "enums.h"
#include "FileDialog.h"
#include "Animator.h"
#include "SoundManager.h"
#include "EventSelectionWindow.h"
#include "ActionSelectionWindow.h"
#include "GameObject.h"
#include "Common.h"
#include "EXGameEngine.h"
#include <vector>
#include <iostream>

class UIManager
{
	//this manager will hold all UI elements and control interaction with the UI


public:
	UIManager(bool*  gameRunning, SDL_Window *window, SDL_Renderer *gameRenderer);//needs a handle to the game window to be able to calculate UI dimensions
	~UIManager();

	void Update(SDL_Event* e);

	void Draw();

private:

	void ClearObjectDetails();
	void SetupPlayerUI(GameObject* object);
	void SetupEnemyUI(GameObject* object);
	void SetupBGMusicUI(GameObject* object);
	void SetupBackgroundUI(GameObject* object);

	void LoadProjFile();

	SDL_Window *gameWindow;
	SDL_Renderer *renderer;

	SDL_Color uiColor;//UI Color
	SDL_Color uiOutlineColor;//UI Color

	//UI elements
	SDL_Rect bottomRect;//rectangle at bottom of screen
	SDL_Rect sideRect;//rectangle at side of screen
	SDL_Rect topRect;//rectangle at top of screen

	std::vector<UIElement*> DefaultUI;//elements of the UI that will not be deleted

	TTF_Font* defaultFont;
	FileDialog* _FileDialog;
	bool* gameIsRunning;

	std::vector<UISelectionBox*>levelSelectionBoxes;
	UISelectionBox* levelSelectionBox;

	int yScrollValue;//used for offsetting the position of all UI elements on the right hand of the screen.

	bool debugRunning;
	EXGameEngine* debugEngine;

	#define CurrentIsATextBox ObjectList->GetItem(i)->gameObject->type == TextBox
	#define CurrentTextBox ((UITextBox*)ObjectList->GetItem(i)->)
	////////////////////////////////////////////////////////////////
	#define AddBtn ((UIButton*)DefaultUI[1]) 
	#define MinusBtn ((UIButton*)DefaultUI[2]) 
	#define OpenFileBtn ((UIButton*)DefaultUI[3])
	#define SaveFileBtn ((UIButton*)DefaultUI[4])
	#define ExportBtn ((UIButton*)DefaultUI[5])
	//#define ObjectList ((UISelectionBox*)DefaultUI[6])
	#define ObjectList levelSelectionBoxes[levelSelectionBox->GetSelectedIndex()]
	//#define LevelList ((UISelectionBox*)DefaultUI[7])
	#define HScrollbar ((UIScrollbar*)DefaultUI[7])
	#define PlayerDragDrop ((UIDragDrop*)DefaultUI[8])
	#define EnemyWaveDragDrop ((UIDragDrop*)DefaultUI[9])
	#define BGMusicDragDrop ((UIDragDrop*)DefaultUI[10])

	#define RunBtn ((UIButton*)DefaultUI[12])
	//////////////////////////////////////////////////////////////////
	#define GetNumOfTextBoxes for (size_t i = 0; i < ObjectList->GetItemCount(); i++) { if (CurrentIsATextBox) numOfTextBoxes++; }
	//////////////////////////////////////////////////////////////////
};