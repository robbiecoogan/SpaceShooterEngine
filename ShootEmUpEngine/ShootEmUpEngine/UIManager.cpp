#include "UIManager.h"



UIManager::UIManager(bool* gameRunning, SDL_Window *window, SDL_Renderer *gameRenderer) : gameIsRunning(gameRunning), gameWindow(window), renderer(gameRenderer)
{
	int screenWidth, screenHeight;
	SDL_GetWindowSize(gameWindow, &screenWidth, &screenHeight);

	int borderSize = 10;//elements will not always push to the exact edge of the screen, the border size is that threshhold

	//Initialize the rectangles for the UI
	sideRect = { screenWidth - (screenWidth / 4), 0, screenWidth / 4, screenHeight };
	bottomRect = { 0, (screenHeight - (screenHeight / 3)), screenWidth - (sideRect.w) + 1, screenHeight / 3 };
	EventManager::getInstance().debugYOffset = 30;
	topRect = { 0, 0, screenWidth - sideRect.w + 1, EventManager::getInstance().debugYOffset };

	//init the font type of this textbox
	defaultFont = EventManager::getInstance().defaultFont;

	//initalize the colors to be used in the UI
	uiColor = { 100, 100, 100, 255 };
	uiOutlineColor = { 0, 0, 0, 255 };

	//create the labels to be drawn on the UI
	UILabel* detailsLabel = new UILabel(sideRect.x + (sideRect.w/4), sideRect.y + 2, " ", renderer, window, 1, defaultFont);

	//add labels to list
	DefaultUI.push_back(detailsLabel);

	UIButton* AddObjectBtn = new UIButton(sideRect.x + (sideRect.w - sideRect.w/6), sideRect.y + borderSize, 20, 20, "+", renderer, window, defaultFont);
	DefaultUI.push_back(AddObjectBtn);
	UIButton* DeleteObjectBtn = new UIButton(screenWidth - borderSize - AddObjectBtn->GetWidth(), AddObjectBtn->GetY(), 20, 20, "-", renderer, window, defaultFont);
	DefaultUI.push_back(DeleteObjectBtn);

	//Open & Save buttons
	UIButton* OpenFileDialogBtn = new UIButton(topRect.x, topRect.y, 60, 30, "Open", renderer, window, defaultFont);
	DefaultUI.push_back(OpenFileDialogBtn);
	UIButton* SaveFileDialogBtn = new UIButton(OpenFileDialogBtn->GetX() + OpenFileDialogBtn->GetWidth() -1, topRect.y, 60, 30, "Save", renderer, window, defaultFont);
	DefaultUI.push_back(SaveFileDialogBtn);
	UIButton* ExportFileBtn = new UIButton(SaveFileDialogBtn->GetX() + SaveFileDialogBtn->GetWidth() - 1, topRect.y, 60, 30, "Export", renderer, window, defaultFont);
	DefaultUI.push_back(ExportFileBtn);

	SDL_Rect gameScreenRect = { 0, topRect.y + topRect.h, 0 + sideRect.x, bottomRect.y - (topRect.y + topRect.h) };

	//Create Selection Boxes
	UISelectionBox* objectSelectionBox = new UISelectionBox(sideRect.x + (sideRect.w / 2) + borderSize, AddObjectBtn->GetY() + AddObjectBtn->GetHeight() + borderSize, (sideRect.w / 2) - (borderSize*2), sideRect.h - AddObjectBtn->GetHeight() - (borderSize * 3), renderer, window, defaultFont, gameScreenRect);
	objectSelectionBox->SetAlignment(0);

	//add Selection Boxes to SelectionBox list
	
	//DefaultUI.push_back(objectSelectionBox);
	levelSelectionBoxes.emplace_back(objectSelectionBox);

	UISelectionBox *tempLevelSelectionBox = new UISelectionBox(bottomRect.x + borderSize, bottomRect.y + borderSize, (bottomRect.w / 2) - borderSize, bottomRect.h - (borderSize * 2), renderer, window, defaultFont, gameScreenRect);
	tempLevelSelectionBox->SetAlignment(1);//set alignment to centre

	//add Selection Boxes to SelectionBox list

	DefaultUI.push_back(tempLevelSelectionBox);
	levelSelectionBox = tempLevelSelectionBox;
	

	//add items to the selection box
	levelSelectionBox->AddItem(new SelectionBoxItem( "Forest Level"));
	UISelectionBox* objectSelectionBox2 = new UISelectionBox(sideRect.x + (sideRect.w / 2) + borderSize, AddObjectBtn->GetY() + AddObjectBtn->GetHeight() + borderSize, (sideRect.w / 2) - (borderSize * 2), sideRect.h - AddObjectBtn->GetHeight() - (borderSize * 3), renderer, window, defaultFont, gameScreenRect); objectSelectionBox2->SetAlignment(0);
	levelSelectionBoxes.emplace_back(objectSelectionBox2);

	levelSelectionBox->AddItem(new SelectionBoxItem( "City Level (Part 1)"));
	UISelectionBox* objectSelectionBox3 = new UISelectionBox(sideRect.x + (sideRect.w / 2) + borderSize, AddObjectBtn->GetY() + AddObjectBtn->GetHeight() + borderSize, (sideRect.w / 2) - (borderSize * 2), sideRect.h - AddObjectBtn->GetHeight() - (borderSize * 3), renderer, window, defaultFont, gameScreenRect); objectSelectionBox3->SetAlignment(0);
	levelSelectionBoxes.emplace_back(objectSelectionBox3);

	levelSelectionBox->AddItem(new SelectionBoxItem( "City Level (Part 2)"));
	UISelectionBox* objectSelectionBox4 = new UISelectionBox(sideRect.x + (sideRect.w / 2) + borderSize, AddObjectBtn->GetY() + AddObjectBtn->GetHeight() + borderSize, (sideRect.w / 2) - (borderSize * 2), sideRect.h - AddObjectBtn->GetHeight() - (borderSize * 3), renderer, window, defaultFont, gameScreenRect); objectSelectionBox4->SetAlignment(0);
	levelSelectionBoxes.emplace_back(objectSelectionBox4);

	levelSelectionBox->AddItem(new SelectionBoxItem( "Underwater Level (Part 1)"));
	UISelectionBox* objectSelectionBox5 = new UISelectionBox(sideRect.x + (sideRect.w / 2) + borderSize, AddObjectBtn->GetY() + AddObjectBtn->GetHeight() + borderSize, (sideRect.w / 2) - (borderSize * 2), sideRect.h - AddObjectBtn->GetHeight() - (borderSize * 3), renderer, window, defaultFont, gameScreenRect); objectSelectionBox5->SetAlignment(0);
	levelSelectionBoxes.emplace_back(objectSelectionBox5);

	levelSelectionBox->AddItem(new SelectionBoxItem( "Super Mega Awesome Secret Level 1"));
	UISelectionBox* objectSelectionBox6 = new UISelectionBox(sideRect.x + (sideRect.w / 2) + borderSize, AddObjectBtn->GetY() + AddObjectBtn->GetHeight() + borderSize, (sideRect.w / 2) - (borderSize * 2), sideRect.h - AddObjectBtn->GetHeight() - (borderSize * 3), renderer, window, defaultFont, gameScreenRect); objectSelectionBox6->SetAlignment(0);
	levelSelectionBoxes.emplace_back(objectSelectionBox6);

	levelSelectionBox->AddItem(new SelectionBoxItem( "Level 6"));
	UISelectionBox* objectSelectionBox7 = new UISelectionBox(sideRect.x + (sideRect.w / 2) + borderSize, AddObjectBtn->GetY() + AddObjectBtn->GetHeight() + borderSize, (sideRect.w / 2) - (borderSize * 2), sideRect.h - AddObjectBtn->GetHeight() - (borderSize * 3), renderer, window, defaultFont, gameScreenRect); objectSelectionBox7->SetAlignment(0);
	levelSelectionBoxes.emplace_back(objectSelectionBox7);


	//add Scrollbar to bottom of view panel
	UIScrollbar* scrollBar = new UIScrollbar(bottomRect.x, bottomRect.y - 20 + 1, bottomRect.w, 20, renderer, window);
	DefaultUI.push_back(scrollBar);

	//add drag drop elements
	std::string imgDir = SDL_GetBasePath(); imgDir.append("Assets\\UIPlayer.jpg");
	UIDragDrop* playerDragDrop = new UIDragDrop("Player", bottomRect.x + (bottomRect.w/2) + borderSize, bottomRect.y + borderSize, 80, 80, imgDir, renderer, window);
	imgDir = SDL_GetBasePath(); imgDir.append("Assets\\UIEnemyWave.jpg");
	UIDragDrop* enemyWaveDragDrop = new UIDragDrop("Enemy", playerDragDrop->GetX() + playerDragDrop->GetWidth() + borderSize, playerDragDrop->GetY(), 80, 80, imgDir, renderer, window);
	imgDir = SDL_GetBasePath(); imgDir.append("Assets\\UIBGMusic.jpg");
	UIDragDrop* backgroundMusicDragDrop = new UIDragDrop("BGMusic", enemyWaveDragDrop->GetX() + enemyWaveDragDrop->GetWidth() + borderSize, enemyWaveDragDrop->GetY(), 80, 80, imgDir, renderer, window);
	imgDir = SDL_GetBasePath(); imgDir.append("Assets\\UIBackground.jpg");
	UIDragDrop* backgroundDragDrop = new UIDragDrop("Background", backgroundMusicDragDrop->GetX() + backgroundMusicDragDrop->GetWidth() + borderSize, enemyWaveDragDrop->GetY(), 80, 80, imgDir, renderer, window);

	DefaultUI.push_back(playerDragDrop);
	DefaultUI.push_back(enemyWaveDragDrop);
	DefaultUI.push_back(backgroundMusicDragDrop);
	DefaultUI.push_back(backgroundDragDrop);

	
	UIButton* playButton = new UIButton(topRect.x + (topRect.w / 2), topRect.y, topRect.h + borderSize, topRect.h, "Run", renderer, window, defaultFont);
	DefaultUI.push_back(playButton);

	_FileDialog = EventManager::getInstance()._FileDialog;

	debugRunning = false;
	debugEngine = nullptr;

}

UIManager::~UIManager()
{
	for (size_t i = 0; i < DefaultUI.size(); i++)
		delete DefaultUI[i];

	delete _FileDialog;
	for (size_t i = 0; i < levelSelectionBoxes.size(); i++)
	{
		delete levelSelectionBoxes[i];
	}
	levelSelectionBoxes.clear();

	delete levelSelectionBox;
	if (debugEngine) delete debugEngine;

}

void UIManager::Update(SDL_Event* e)
{
	if (!debugRunning)
	{
		//obtain mouse positions and store them
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);//store button press, and feed in references to x and y to store this as well. This is mouse position relative to the window. i.e. 0,0 is the top left of the active window

		int screenWidth, screenHeight;
		SDL_GetWindowSize(gameWindow, &screenWidth, &screenHeight);

		int screenX, screenY;
		SDL_GetWindowPosition(gameWindow, &screenX, &screenY);

		yScrollValue = 0;

		if (EventManager::getInstance().GetEvent(SDL_WINDOWEVENT_CLOSE).size() > 0)
		{
			gameIsRunning = false;
		}

		UISelectionBox* temporr = ObjectList;


		//check buttons
		if (MinusBtn->CheckClick()) { ObjectList->RemoveAt(ObjectList->GetSelectedIndex()); }
		else if (OpenFileBtn->CheckClick()) { LoadProjFile(); }
		else if (SaveFileBtn->CheckClick()) 
		{
			_FileDialog->ShowDialogPrepFileLoc();
			SDL_Rect gameScreenRect = { 0, topRect.y + topRect.h, 0 + sideRect.x, bottomRect.y - (topRect.y + topRect.h) };
			for (unsigned int i = 0; i < ObjectList->GetItemCount(); i++)
			{
				SDL_Rect destRect;
				GameObject* currGameObject = (GameObject*)ObjectList->GetItem(i)->gameObject;
				//the below 3 lines are only declared for use inside their own case statements, application will likely crash if type checking isnt done first.
				GOEnemy* enemyOBJ = ((GOEnemy*)currGameObject);
				GOPlayer* playerOBJ = ((GOPlayer*)currGameObject);
				GOBackground* BGOBJ = ((GOBackground*)currGameObject);

				switch (currGameObject->type)
				{
				case GameObjectType::PLAYER:
					destRect = { currGameObject->x, currGameObject->y, currGameObject->width, currGameObject->height };
					_FileDialog->AddPlayerToJson(100, currGameObject->shootingPoint.x, currGameObject->shootingPoint.y, SDL_Rect{ 0, 0, 0, 0 }, currGameObject->bulletDir, currGameObject->spriteDir, destRect, 10, currGameObject->events);
					break;
				case GameObjectType::ENEMYWAVE:
					destRect = { currGameObject->x, currGameObject->y, currGameObject->width, currGameObject->height };
					_FileDialog->AddEnemyToJson(10, enemyOBJ->shootingPoint.x, enemyOBJ->shootingPoint.y, SDL_Rect{ 0,0,0,0 }, enemyOBJ->rotation, currGameObject->spriteDir, destRect, 10, enemyOBJ->XVec, enemyOBJ->YVec);
					break;
				case GameObjectType::BACKGROUND:
					SDL_Rect destRect = { currGameObject->x, currGameObject->y, currGameObject->width, currGameObject->height };
					_FileDialog->AddBackgroundToJson(destRect, currGameObject->spriteDir, ((GOBackground*)currGameObject)->scrollSpeed);
					break;
				}
			}
			_FileDialog->SaveProject(gameScreenRect.w, gameScreenRect.h);
		}
		else if (ExportBtn->CheckClick())//Check Export button click
		{
			_FileDialog->ShowDialogPrepFileLoc();
			SDL_Rect gameScreenRect = { 0, topRect.y + topRect.h, 0 + sideRect.x, bottomRect.y - (topRect.y + topRect.h) };
			for (unsigned int i = 0; i < ObjectList->GetItemCount(); i++)
			{
				SDL_Rect destRect;
				GameObject* currGameObject = (GameObject*)ObjectList->GetItem(i)->gameObject;
				//the below 3 lines are only declared for use inside their own case statements, application will likely crash if type checking isnt done first.
				GOEnemy* enemyOBJ = ((GOEnemy*)currGameObject);
				GOPlayer* playerOBJ = ((GOPlayer*)currGameObject);
				GOBackground* BGOBJ = ((GOBackground*)currGameObject);

				switch (currGameObject->type)
				{
				case GameObjectType::PLAYER:
					destRect = { currGameObject->x, currGameObject->y, currGameObject->width, currGameObject->height };
					_FileDialog->AddPlayerToJson(100, currGameObject->shootingPoint.x, currGameObject->shootingPoint.y, SDL_Rect{ 0, 0, 0, 0 }, currGameObject->bulletDir, currGameObject->spriteDir, destRect, 10, currGameObject->events);
					break;
				case GameObjectType::ENEMYWAVE:
					destRect = { currGameObject->x, currGameObject->y, currGameObject->width, currGameObject->height };
					_FileDialog->AddEnemyToJson(10, enemyOBJ->shootingPoint.x, enemyOBJ->shootingPoint.y, SDL_Rect{ 0,0,0,0 }, enemyOBJ->rotation, currGameObject->spriteDir, destRect, 10, enemyOBJ->XVec, enemyOBJ->YVec);
					break;
				case GameObjectType::BACKGROUND:
					SDL_Rect destRect = { currGameObject->x, currGameObject->y, currGameObject->width, currGameObject->height };
					_FileDialog->AddBackgroundToJson(destRect, currGameObject->spriteDir, ((GOBackground*)currGameObject)->scrollSpeed);
					break;
				}
			}
			_FileDialog->ExportJson(gameScreenRect.w, gameScreenRect.h);
		}
		else if (RunBtn->CheckClick())
		{
			_FileDialog->PrepDebugFileLoc();
			SDL_Rect gameScreenRect = { 0, topRect.y + topRect.h, 0 + sideRect.x, bottomRect.y - (topRect.y + topRect.h) };
			for (unsigned int i = 0; i < ObjectList->GetItemCount(); i++)
			{
				SDL_Rect destRect;
				GameObject* currGameObject = (GameObject*)ObjectList->GetItem(i)->gameObject;
				//the below 3 lines are only declared for use inside their own case statements, application will likely crash if type checking isnt done first.
				GOEnemy* enemyOBJ = ((GOEnemy*)currGameObject);
				GOPlayer* playerOBJ = ((GOPlayer*)currGameObject);
				GOBackground* BGOBJ = ((GOBackground*)currGameObject);

				switch (currGameObject->type)
				{
				case GameObjectType::PLAYER:
					destRect = { currGameObject->x, currGameObject->y, currGameObject->width, currGameObject->height };
					_FileDialog->AddPlayerToJson(100, currGameObject->shootingPoint.x, currGameObject->shootingPoint.y, SDL_Rect{ 0, 0, 0, 0 }, currGameObject->bulletDir, currGameObject->spriteDir, destRect, 10, currGameObject->events);
					break;
				case GameObjectType::ENEMYWAVE:
					destRect = { currGameObject->x, currGameObject->y, currGameObject->width, currGameObject->height };
					_FileDialog->AddEnemyToJson(10, enemyOBJ->shootingPoint.x, enemyOBJ->shootingPoint.y, SDL_Rect{ 0,0,0,0 }, enemyOBJ->rotation, currGameObject->spriteDir, destRect, 10, enemyOBJ->XVec, enemyOBJ->YVec);
					break;
				case GameObjectType::BACKGROUND:
					SDL_Rect destRect = { currGameObject->x, currGameObject->y, currGameObject->width, currGameObject->height };
					_FileDialog->AddBackgroundToJson(destRect, currGameObject->spriteDir, ((GOBackground*)currGameObject)->scrollSpeed);
					break;
				}
			}
			_FileDialog->ExportJson(gameScreenRect.w, gameScreenRect.h);
			debugRunning = true;
			std::string fileDir = SDL_GetBasePath();
			fileDir += "Assets\\debug.JSON";
			debugEngine = new EXGameEngine((char*)fileDir.c_str(), gameWindow, renderer);
		}

		if (SDL_GetMouseState(NULL, NULL) == SDL_BUTTON(SDL_BUTTON_LEFT))//if the left button is clicked
		{
			ObjectList->CheckSelected(mouseX, mouseY);
			levelSelectionBox->CheckSelected(mouseX, mouseY);
		}

		SelectionBoxItem * temp = ObjectList->GetSelectedItem();


		SDL_Rect gameScreenRect = { 0, topRect.y + topRect.h, 0 + sideRect.x, bottomRect.y - (topRect.y + topRect.h) };
		ObjectList->Update(gameScreenRect);

		//iterate through UI & update
		for (size_t i = 0; i < DefaultUI.size(); i++)
		{
			if (DefaultUI[i]) DefaultUI[i]->Update(gameScreenRect);
			if (DefaultUI[i]->type == UIType::DragDrop)
			{
				if (temp = ((UIDragDrop*)DefaultUI[i])->HasDropped())
				{
					switch (temp->gameObject->type)
					{
					case GameObjectType::PLAYER:
						SetupPlayerUI(temp->gameObject);
						break;
					case GameObjectType::ENEMYWAVE:
						SetupEnemyUI(temp->gameObject);
						break;
					case GameObjectType::BACKGROUNDMUSIC:
						SetupBGMusicUI(temp->gameObject);
						break;
					case GameObjectType::BACKGROUND:
						SetupBackgroundUI(temp->gameObject);
						break;
					}

					if (temp->isGameObject) temp->gameObject->x += -HScrollbar->GetValue();

					ObjectList->AddItem(temp);
				}
			}
		}

		temp = ObjectList->GetSelectedItem();
		GameObject* tempGO = nullptr;

		if (temp) tempGO = (GameObject*)ObjectList->GetSelectedItem()->gameObject;
	}
	else
	{
		RunBtn->Update(SDL_Rect{0, 0, 0, 0});
		bool keepRunning = debugEngine->Update();
		if (!keepRunning)
		{
			delete debugEngine;
			debugEngine = nullptr;
			debugRunning = false;
		}
		if (RunBtn->CheckClick())
		{
			delete debugEngine;
			debugEngine = nullptr;
			debugRunning = false;
		}
	}
}

void UIManager::ClearObjectDetails()
{
}

void UIManager::SetupPlayerUI(GameObject* object)
{
	int screenWidth, screenHeight;
	SDL_GetWindowSize(gameWindow, &screenWidth, &screenHeight);

	int borderSize = 10;

	//create the labels to be drawn on the UI
	UILabel* detailsLabel = new UILabel(sideRect.x + (sideRect.w / 4), sideRect.y + 2, "Details", renderer, gameWindow, 1, defaultFont);

	UILabel* objectNameLabel = new UILabel(sideRect.x + (sideRect.w / 4), detailsLabel->GetY() + detailsLabel->GetHeight(), "Object Name: ", renderer, gameWindow, 1, defaultFont);
	UITextBox* objectNameTextBox = new UITextBox(sideRect.x + borderSize, objectNameLabel->GetY() + objectNameLabel->GetHeight(), (sideRect.w / 2) - (borderSize * 2), "", renderer, gameWindow, defaultFont);

	UILabel* xPosLabel = new UILabel(sideRect.x + borderSize, objectNameTextBox->GetY() + objectNameTextBox->GetHeight() + borderSize, "X: ", renderer, gameWindow, 0, defaultFont);
	UITextBox* xPosTextBox = new UITextBox(xPosLabel->GetX() + xPosLabel->GetWidth(), xPosLabel->GetY(), objectNameTextBox->GetWidth() / 3, "", renderer, gameWindow, defaultFont);

	UILabel* yPosLabel = new UILabel(xPosTextBox->GetX() + xPosTextBox->GetWidth() + borderSize, xPosLabel->GetY(), "Y: ", renderer, gameWindow, 0, defaultFont);
	UITextBox* yPosTextBox = new UITextBox(yPosLabel->GetX() + yPosLabel->GetWidth(), yPosLabel->GetY(), (sideRect.x + (sideRect.w / 2)) - (yPosLabel->GetX() + yPosLabel->GetWidth()) - borderSize, "", renderer, gameWindow, defaultFont);

	UILabel* widthLabel = new UILabel(xPosLabel->GetX(), xPosLabel->GetY() + yPosLabel->GetHeight() + borderSize, "Width: ", renderer, gameWindow, 0, defaultFont);
	UITextBox* widthTextBox = new UITextBox(widthLabel->GetX() + widthLabel->GetWidth(), widthLabel->GetY(), (sideRect.x + (sideRect.w / 2)) - (widthLabel->GetX() + widthLabel->GetWidth()) - borderSize, "", renderer, gameWindow, defaultFont);

	UILabel* heightLabel = new UILabel(widthLabel->GetX(), widthLabel->GetY() + widthLabel->GetHeight() + borderSize, "Height: ", renderer, gameWindow, 0, defaultFont);
	UITextBox* heightTextBox = new UITextBox(heightLabel->GetX() + heightLabel->GetWidth(), heightLabel->GetY(), (sideRect.x + (sideRect.w / 2)) - (heightLabel->GetX() + heightLabel->GetWidth()) - borderSize, "", renderer, gameWindow, defaultFont);

	UILabel* rotationLabel = new UILabel(objectNameLabel->GetX(), heightLabel->GetY() + heightLabel->GetHeight(), "Rotation: ", renderer, gameWindow, 1, defaultFont);
	UITextBox* rotationTextBox = new UITextBox(sideRect.x + borderSize, rotationLabel->GetY() + rotationLabel->GetHeight(), objectNameTextBox->GetWidth(), "", renderer, gameWindow, defaultFont);

	UILabel* spriteLabel = new UILabel(sideRect.x + borderSize, rotationTextBox->GetY() + rotationTextBox->GetHeight() + borderSize + borderSize, "Sprite:", renderer, gameWindow, 0, defaultFont);
	UIButton* spriteButton = new UIButton(spriteLabel->GetX() + spriteLabel->GetWidth() + borderSize, spriteLabel->GetY(), (sideRect.x + sideRect.w / 2) - (spriteLabel->GetX() + spriteLabel->GetWidth() + borderSize) - borderSize, spriteLabel->GetHeight(), "Load", renderer, gameWindow, defaultFont);

	UILabel*  shootingPointLabel = new UILabel(objectNameLabel->GetX(), spriteButton->GetY() + spriteButton->GetHeight() + borderSize, "Shooting Point: ", renderer, gameWindow, 1, defaultFont);
	UILabel* shootingPointXlabel = new UILabel(rotationTextBox->GetX(), shootingPointLabel->GetY() + shootingPointLabel->GetHeight() + borderSize, "X: ", renderer, gameWindow, 0, defaultFont);
	UITextBox* shootingPointXTextBox = new UITextBox(shootingPointXlabel->GetX() + shootingPointXlabel->GetWidth(), shootingPointXlabel->GetY(), (sideRect.x + (sideRect.w / 2)) - (shootingPointXlabel->GetX() + shootingPointXlabel->GetWidth()) - borderSize, "", renderer, gameWindow, defaultFont);

	UILabel* shootingPointYlabel = new UILabel(rotationTextBox->GetX(), shootingPointXlabel->GetY() + shootingPointXlabel->GetHeight() + borderSize, "Y: ", renderer, gameWindow, 0, defaultFont);
	UITextBox* shootingPointYTextBox = new UITextBox(shootingPointYlabel->GetX() + shootingPointYlabel->GetWidth(), shootingPointYlabel->GetY(), (sideRect.x + (sideRect.w / 2)) - (shootingPointYlabel->GetX() + shootingPointYlabel->GetWidth()) - borderSize, "", renderer, gameWindow, defaultFont);

	UILabel* bulletSpriteLabel = new UILabel(sideRect.x + borderSize, shootingPointYTextBox->GetY() + shootingPointYTextBox->GetHeight() + borderSize + borderSize, "Bullet:", renderer, gameWindow, 0, defaultFont);
	UIButton* bulletSpriteButton = new UIButton(bulletSpriteLabel->GetX() + bulletSpriteLabel->GetWidth() + borderSize, bulletSpriteLabel->GetY(), (sideRect.x + sideRect.w / 2) - (bulletSpriteLabel->GetX() + bulletSpriteLabel->GetWidth() + borderSize) - borderSize, bulletSpriteLabel->GetHeight(), "Load", renderer, gameWindow, defaultFont);


	UILabel* EventsLabel = new UILabel(sideRect.x + sideRect.w / 4, bulletSpriteButton->GetY() + bulletSpriteButton->GetHeight() + borderSize, "Events", renderer, gameWindow, 1, defaultFont);


	//add textboxes to UIList
	object->AddComponent(objectNameTextBox);
	object->AddComponent(xPosTextBox);
	object->AddComponent(yPosTextBox);
	object->AddComponent(widthTextBox);
	object->AddComponent(heightTextBox);
	object->AddComponent(rotationTextBox);
	object->AddComponent(shootingPointXTextBox);
	object->AddComponent(shootingPointYTextBox);

	//add buttons
	object->AddComponent(spriteButton);
	object->AddComponent(bulletSpriteButton);
	

	UIButton* AddEventBtn = new UIButton(spriteLabel->GetX(), EventsLabel->GetY() + EventsLabel->GetHeight() + borderSize, rotationTextBox->GetWidth(), rotationTextBox->GetHeight(), "Add Event", renderer, gameWindow, defaultFont);

	object->AddComponent(AddEventBtn);


	//add labels to list
	object->AddComponent(detailsLabel);
	object->AddComponent(objectNameLabel);
	object->AddComponent(xPosLabel);
	object->AddComponent(yPosLabel);
	object->AddComponent(widthLabel);
	object->AddComponent(heightLabel);
	object->AddComponent(rotationLabel);
	object->AddComponent(spriteLabel);
	object->AddComponent(shootingPointLabel);
	object->AddComponent(shootingPointXlabel);
	object->AddComponent(shootingPointYlabel);
	object->AddComponent(EventsLabel);
	object->AddComponent(bulletSpriteLabel);
}

void UIManager::SetupEnemyUI(GameObject* object)
{
	int screenWidth, screenHeight;
	SDL_GetWindowSize(gameWindow, &screenWidth, &screenHeight);

	int borderSize = 10;

	//create the labels to be drawn on the UI
	UILabel* detailsLabel = new UILabel(sideRect.x + (sideRect.w / 4), sideRect.y + 2, "Details", renderer, gameWindow, 1, defaultFont);

	UILabel* objectNameLabel = new UILabel(sideRect.x + (sideRect.w / 4), detailsLabel->GetY() + detailsLabel->GetHeight(), "Object Name: ", renderer, gameWindow, 1, defaultFont);
	UITextBox* objectNameTextBox = new UITextBox(sideRect.x + borderSize, objectNameLabel->GetY() + objectNameLabel->GetHeight(), (sideRect.w / 2) - (borderSize * 2), "", renderer, gameWindow, defaultFont);

	UILabel* xPosLabel = new UILabel(sideRect.x + borderSize, objectNameTextBox->GetY() + objectNameTextBox->GetHeight() + borderSize, "X: ", renderer, gameWindow, 0, defaultFont);
	UITextBox* xPosTextBox = new UITextBox(xPosLabel->GetX() + xPosLabel->GetWidth(), xPosLabel->GetY(), objectNameTextBox->GetWidth() / 3, "", renderer, gameWindow, defaultFont);

	UILabel* yPosLabel = new UILabel(xPosTextBox->GetX() + xPosTextBox->GetWidth() + borderSize, xPosLabel->GetY(), "Y: ", renderer, gameWindow, 0, defaultFont);
	UITextBox* yPosTextBox = new UITextBox(yPosLabel->GetX() + yPosLabel->GetWidth(), yPosLabel->GetY(), (sideRect.x + (sideRect.w / 2)) - (yPosLabel->GetX() + yPosLabel->GetWidth()) - borderSize, "", renderer, gameWindow, defaultFont);

	UILabel* widthLabel = new UILabel(xPosLabel->GetX(), xPosLabel->GetY() + yPosLabel->GetHeight() + borderSize, "Width: ", renderer, gameWindow, 0, defaultFont);
	UITextBox* widthTextBox = new UITextBox(widthLabel->GetX() + widthLabel->GetWidth(), widthLabel->GetY(), (sideRect.x + (sideRect.w / 2)) - (widthLabel->GetX() + widthLabel->GetWidth()) - borderSize, "", renderer, gameWindow, defaultFont);

	UILabel* heightLabel = new UILabel(widthLabel->GetX(), widthLabel->GetY() + widthLabel->GetHeight() + borderSize, "Height: ", renderer, gameWindow, 0, defaultFont);
	UITextBox* heightTextBox = new UITextBox(heightLabel->GetX() + heightLabel->GetWidth(), heightLabel->GetY(), (sideRect.x + (sideRect.w / 2)) - (heightLabel->GetX() + heightLabel->GetWidth()) - borderSize, "", renderer, gameWindow, defaultFont);

	UILabel* rotationLabel = new UILabel(objectNameLabel->GetX(), heightLabel->GetY() + heightLabel->GetHeight(), "Rotation: ", renderer, gameWindow, 1, defaultFont);
	UITextBox* rotationTextBox = new UITextBox(sideRect.x + borderSize, rotationLabel->GetY() + rotationLabel->GetHeight(), objectNameTextBox->GetWidth(), "", renderer, gameWindow, defaultFont);

	UILabel* moveVecLabel = new UILabel(objectNameLabel->GetX(), rotationTextBox->GetY() + rotationTextBox->GetHeight() + borderSize, "Move Vector: ", renderer, gameWindow, 1, defaultFont);
	UILabel* moveVecXLabel = new UILabel(xPosLabel->GetX(), moveVecLabel->GetY() + moveVecLabel->GetHeight(), "X: ", renderer, gameWindow, 0, defaultFont);
	UILabel* moveVecYLabel = new UILabel(xPosLabel->GetX(), moveVecXLabel->GetY() + moveVecXLabel->GetHeight() + borderSize, "Y: ", renderer, gameWindow, 0, defaultFont);
	UITextBox* moveVecXTextBox = new UITextBox(xPosLabel->GetX() + moveVecXLabel->GetWidth(), moveVecLabel->GetY() + moveVecLabel->GetHeight(), rotationTextBox->GetWidth() - moveVecXLabel->GetWidth(), renderer, gameWindow, defaultFont);
	UITextBox* moveVecYTextBox = new UITextBox(xPosLabel->GetX() + moveVecYLabel->GetWidth(), moveVecXTextBox->GetY() + moveVecXTextBox->GetHeight() + borderSize, rotationTextBox->GetWidth() - moveVecXLabel->GetWidth(), renderer, gameWindow, defaultFont);
	

	//add textboxes to UIList
	object->AddComponent(objectNameTextBox);
	object->AddComponent(xPosTextBox);
	object->AddComponent(yPosTextBox);
	object->AddComponent(widthTextBox);
	object->AddComponent(heightTextBox);
	object->AddComponent(rotationTextBox);
	object->AddComponent(moveVecXTextBox);
	object->AddComponent(moveVecYTextBox);

	;
		//add labels to list
	object->AddComponent(detailsLabel);
	object->AddComponent(objectNameLabel);
	object->AddComponent(xPosLabel);
	object->AddComponent(yPosLabel);
	object->AddComponent(widthLabel);
	object->AddComponent(heightLabel);
	object->AddComponent(rotationLabel);
	object->AddComponent(moveVecLabel);
	object->AddComponent(moveVecXLabel);
	object->AddComponent(moveVecYLabel);
}

void UIManager::SetupBGMusicUI(GameObject* object)
{
	int screenWidth, screenHeight;
	SDL_GetWindowSize(gameWindow, &screenWidth, &screenHeight);

	int borderSize = 10;

	UILabel* detailsLabel = new UILabel(sideRect.x + (sideRect.w / 4), sideRect.y + 2, "Details", renderer, gameWindow, 1, defaultFont);

	UILabel* objectNameLabel = new UILabel(sideRect.x + (sideRect.w / 4), detailsLabel->GetY() + detailsLabel->GetHeight(), "Object Name: ", renderer, gameWindow, 1, defaultFont);
	UITextBox* objectNameTextBox = new UITextBox(sideRect.x + borderSize, objectNameLabel->GetY() + objectNameLabel->GetHeight(), (sideRect.w / 2) - (borderSize * 2), "", renderer, gameWindow, defaultFont);

	UILabel* filePathLabel = new UILabel(detailsLabel->GetX(), objectNameTextBox->GetY() + objectNameTextBox->GetHeight() + borderSize, "File Path:", renderer, gameWindow, 1, defaultFont);
	UITextBox* filePathTextBox = new UITextBox(sideRect.x + borderSize, filePathLabel->GetY() + filePathLabel->GetHeight() + borderSize, (sideRect.w/2) - borderSize - borderSize, renderer, gameWindow, defaultFont);

	UIButton* GetFilePathButton = new UIButton(filePathTextBox->GetX(), filePathTextBox->GetY() + filePathTextBox->GetHeight() + borderSize, filePathTextBox->GetWidth(), filePathTextBox->GetHeight(), "Search..", renderer, gameWindow, defaultFont);
	UIButton* PlayMusicBtn = new UIButton(GetFilePathButton->GetX(), GetFilePathButton->GetY() + GetFilePathButton->GetHeight() + borderSize, objectNameTextBox->GetWidth(), objectNameTextBox->GetHeight(), "Play Music", renderer, gameWindow, defaultFont);

	object->AddComponent(objectNameTextBox);
	object->AddComponent(filePathTextBox);
	
	object->AddComponent(GetFilePathButton);
	object->AddComponent(PlayMusicBtn);

	object->AddComponent(detailsLabel);
	object->AddComponent(objectNameLabel);
	object->AddComponent(filePathLabel);

}

void UIManager::SetupBackgroundUI(GameObject * object)
{
	int screenWidth, screenHeight;
	SDL_GetWindowSize(gameWindow, &screenWidth, &screenHeight);

	int borderSize = 10;

	UILabel* detailsLabel = new UILabel(sideRect.x + (sideRect.w / 4), sideRect.y + 2, "Details", renderer, gameWindow, 1, defaultFont);

	UILabel* objectNameLabel = new UILabel(sideRect.x + (sideRect.w / 4), detailsLabel->GetY() + detailsLabel->GetHeight(), "Object Name: ", renderer, gameWindow, 1, defaultFont);
	UITextBox* objectNameTextBox = new UITextBox(sideRect.x + borderSize, objectNameLabel->GetY() + objectNameLabel->GetHeight(), (sideRect.w / 2) - (borderSize * 2), "", renderer, gameWindow, defaultFont);

	UIButton* SetBackgroundImgButton = new UIButton(objectNameTextBox->GetX(), objectNameTextBox->GetY() + objectNameTextBox->GetHeight() + borderSize, objectNameTextBox->GetWidth(), objectNameTextBox->GetHeight(), "Select Sprite", renderer, gameWindow, defaultFont);

	UILabel* scrollSpeedLabel = new UILabel(SetBackgroundImgButton->GetX(), SetBackgroundImgButton->GetY() + SetBackgroundImgButton->GetHeight() + borderSize, "Scroll Speed: ", renderer, gameWindow, 0, defaultFont);
	UITextBox* scrollSpeedTxtBox = new UITextBox(scrollSpeedLabel->GetX(), scrollSpeedLabel->GetY() + scrollSpeedLabel->GetHeight(), objectNameTextBox->GetWidth(), renderer, gameWindow, defaultFont);

	object->AddComponent(objectNameTextBox);
	object->AddComponent(SetBackgroundImgButton);
	object->AddComponent(scrollSpeedTxtBox);


	object->AddComponent(detailsLabel);
	object->AddComponent(objectNameLabel);
	object->AddComponent(scrollSpeedLabel);
}

void UIManager::LoadProjFile()
{
	_FileDialog->ShowDialogPrepReadLoc();
	std::vector<BackgroundSaveStruct*> tempBackgrounds;
	std::vector<PlayerSaveStruct*> tempPlayers;
	std::vector<EnemySaveStruct*> tempEnemies;

	_FileDialog->OpenProject(tempBackgrounds, tempPlayers, tempEnemies);


	//clear the current project
	int numItems = ObjectList->GetItemCount();
	for (size_t i = 0; i < numItems; i++)
		ObjectList->RemoveAt(0);

	std::string basePath = SDL_GetBasePath();

	//add all the info gained from file.
	int backgroundSize = tempBackgrounds.size();
	for (size_t i = 0; i < backgroundSize; i++)
	{
		float xPos = tempBackgrounds[i]->destRect.x;
		float yPos = tempBackgrounds[i]->destRect.y;
		std::string fileDir = basePath + tempBackgrounds[i]->fileDir;
		GameObject* newBG = new GOBackground("New Background", xPos, yPos, fileDir, gameWindow, renderer);
		SetupBackgroundUI(newBG);

		SelectionBoxItem* tempOBJ = new SelectionBoxItem("New Background", true, newBG);

		ObjectList->AddItem(tempOBJ);
	}

	int playersSize = tempPlayers.size();
	for (size_t i = 0; i < playersSize; i++)
	{
		float xPos = tempPlayers[i]->destRect.x;
		float yPos = tempPlayers[i]->destRect.y;
		std::string fileDir = basePath + tempPlayers[i]->shipFilePath;
		GameObject* newPlayer = new GOPlayer("New Player", xPos, yPos, fileDir, gameWindow, renderer);
		newPlayer->shootingPoint.x = tempPlayers[i]->shootPointX;
		newPlayer->shootingPoint.y = tempPlayers[i]->shootPointY;
		SetupPlayerUI(newPlayer);
		newPlayer->SetNewBulletImgFromFile(basePath + tempPlayers[i]->bulletFilePath);

		newPlayer->events = tempPlayers[i]->events;
		newPlayer->ForceUpdateEvents();
		
		SelectionBoxItem* tempOBJ = new SelectionBoxItem("New Player", true, newPlayer);

		ObjectList->AddItem(tempOBJ);
	}
}

void UIManager::Draw()
{
	//draw gameobjects under the UI
	for (int i = 0; i < ObjectList->GetItemCount(); i++)
	{
		GameObject* tempGO = nullptr;
		if (ObjectList->GetItem(i)) tempGO = (GameObject*)ObjectList->GetItem(i)->gameObject;
		if (tempGO && !debugEngine)
		{
			tempGO->Draw(HScrollbar->GetValue());
		}
	}

	if (debugEngine) debugEngine->Draw();


	SDL_SetRenderDrawColor(renderer, uiColor.r, uiColor.g, uiColor.b, uiColor.a);//set color to fill color
	SDL_RenderFillRect(renderer, &bottomRect);//Draw bottom rectangle fill
	SDL_RenderFillRect(renderer, &sideRect);//draw side rectangle fill
	SDL_RenderFillRect(renderer, &topRect);//draw top rectangle fill

	SDL_SetRenderDrawColor(renderer, uiOutlineColor.r, uiOutlineColor.g, uiOutlineColor.b, uiOutlineColor.a);//set color to outline color
	SDL_RenderDrawRect(renderer, &bottomRect);//Draw bottom rectangle outline
	SDL_RenderDrawRect(renderer, &sideRect);//Draw side rectangle outline
	SDL_RenderDrawRect(renderer, &topRect);//Draw top rectangle outline

	//NOTE: You can remove the below if you don't like how the UI looks regarding the 'smoothed' borders between each UI box (removing the below will add black bars between each section of the UI)
	//create a quick rect to cover up the outline between the top bar and the side bar, and to cover up the bottom bar and side bar.
	SDL_SetRenderDrawColor(renderer, uiColor.r, uiColor.g, uiColor.b, uiColor.a);//set color to fill color
	SDL_Rect invisRect = { sideRect.x, sideRect.y + 1, 1, topRect.h - 2 };
	SDL_RenderFillRect(renderer, &invisRect);//draw
	invisRect = { bottomRect.x + bottomRect.w - 1, bottomRect.y + 1, 1, bottomRect.h - 2 };
	SDL_RenderFillRect(renderer, &invisRect);//draw
	SDL_SetRenderDrawColor(renderer, uiOutlineColor.r, uiOutlineColor.g, uiOutlineColor.b, uiOutlineColor.a);//set color to outline color
	invisRect = { sideRect.x + (sideRect.w / 2), 0, 1, sideRect.h };
	SDL_RenderFillRect(renderer, &invisRect);//draw


	ObjectList->Draw(((debugEngine) ? false : true));


	//iterate through UI & Draw
	for (size_t i = 0; i < DefaultUI.size(); i++)
	{
		DefaultUI[i]->Draw();
	}

	if (ObjectList->GetSelectedItem())ObjectList->GetSelectedItem()->gameObject->DrawUI();


}