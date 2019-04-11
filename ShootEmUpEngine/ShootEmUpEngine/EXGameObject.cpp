#include "EXGameObject.h"
#include "EventManager.h"

EXGameObject::EXGameObject(SDL_Rect inputSourceRect, SDL_Rect inputDestRect, Vector2 shootingPointPos, std::string inputSpriteDir, std::string inputBulletDir, SDL_Window * gameWindow, SDL_Renderer * gameRenderer)
{
	float screenScale = EventManager::screenScale;

	window = gameWindow;
	renderer = gameRenderer;

	inputSpriteDir = SDL_GetBasePath() + inputSpriteDir;

	//load sprite for this object using above details
	texture = NULL;
	shootingPoint.bulletTex = NULL;

	SDL_Surface* loadSurface = IMG_Load(inputSpriteDir.c_str());

	spriteDir = inputSpriteDir;
	//std::string basePath = SDL_GetBasePath();
	//spriteDir = inputSpriteDir.substr(basePath.size(), inputSpriteDir.size() - basePath.size());

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	if (loadSurface == NULL) std::cout << "Error Loading Image: " << IMG_GetError() << std::endl;
	else
	{
		SDL_ConvertSurfaceFormat(loadSurface, SDL_PIXELFORMAT_RGBA8888, 0);

		texture = SDL_CreateTextureFromSurface(renderer, loadSurface);

		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		if (inputSourceRect.w == 0 || inputSourceRect.h == 0)//width or height of 0 is a flag for default image deets
		{
			sourceRect = loadSurface->clip_rect;
		}
		else
		{
			sourceRect = inputSourceRect;
		}

		if (inputDestRect.w == 0 || inputDestRect.h == 0)
		{
			destRect = sourceRect;
		}
		else
		{
			destRect = inputDestRect;
		}


		if (inputDestRect.x != 0 && inputDestRect.y != 0)
		{
			destRect.x = inputDestRect.x;
			destRect.y = inputDestRect.y;
		}

	}


	//load bullet texture if applicable
	if (inputBulletDir != "")
	{
		inputBulletDir = SDL_GetBasePath() + inputBulletDir;

		shootingPoint.bulletTex = NULL;

		SDL_Surface* bulletLoadSurface = IMG_Load(inputBulletDir.c_str());

		if (bulletLoadSurface == NULL) std::cout << "Error Loading Image: " << IMG_GetError() << std::endl;
		else
		{
			SDL_ConvertSurfaceFormat(bulletLoadSurface, SDL_PIXELFORMAT_RGBA8888, 0);

			shootingPoint.bulletTex = SDL_CreateTextureFromSurface(renderer, bulletLoadSurface);

			SDL_SetTextureBlendMode(shootingPoint.bulletTex, SDL_BLENDMODE_BLEND);

			shootingPoint.sourceRect = bulletLoadSurface->clip_rect;
			shootingPoint.destRect = shootingPoint.sourceRect;
		}
	}


	xPos = destRect.x;
	yPos = destRect.y;

	shootingPoint.x = shootingPointPos.x;
	shootingPoint.y = shootingPointPos.y;

	type = GOType::Player;//despite this constructor being called by all other gameobject types, they will re-set their own type in their constructor, afterwards.
	rotation = 0;

	shootingPoint.bulletDir = inputBulletDir;

	shootIsDown = false;

	velocity = Vector2(0,0);
}

//Set up the gameObject with just a texture, rather than tell it to load from file
EXGameObject::EXGameObject(SDL_Rect inputSourceRect, SDL_Rect inputDestRect, Vector2 shootingPointPos, SDL_Texture* myTex, SDL_Window * gameWindow, SDL_Renderer * gameRenderer)
{
	float screenScale = EventManager::screenScale;

	window = gameWindow;
	renderer = gameRenderer;

	texture = myTex;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);


	sourceRect = inputSourceRect;
	destRect = inputDestRect;

	xPos = destRect.x;
	yPos = destRect.y;

	shootingPoint.x = shootingPointPos.x;
	shootingPoint.y = shootingPointPos.y;

	type = GOType::Player;//despite this constructor being called by all other gameobject types, they will re-set their own type in their constructor, afterwards.
	rotation = 0;

	shootIsDown = false;

	velocity = Vector2(0, 0);
}

EXGameObject::~EXGameObject()
{
	if (texture && type != GOType::Bullet) SDL_DestroyTexture(texture);//if we are a bullet, we are sharing the texture, so deleting this will cause crazyboi issues
	if (shootingPoint.bulletTex) SDL_DestroyTexture(shootingPoint.bulletTex);
	int bulletSize = bullets.size();
	for (size_t i = 0; i < bulletSize; i++)
	{
		delete (*(bullets.begin() + i));
	}
	bullets.clear();
	int eventSize = events.size();
	for (size_t i = 0; i < eventSize; i++)
	{
		int actionSize = events[i]->actions.size();
		for (size_t k = 0; k < actionSize; k++)
		{
			delete events[i]->actions[k];
		}
		delete events[i];
	}
}

void EXGameObject::Update()
{
	int screenWidth, screenHeight;
	SDL_GetWindowSize(window, &screenWidth, &screenHeight);
	screenWidth *= EventManager::getInstance().screenScale;
	screenHeight *= EventManager::getInstance().screenScale;

	std::vector<SDL_Event*> e = EventManager::getInstance().GetEvent(SDL_KEYDOWN);

	size_t bulletSize = bullets.size();
	for (size_t i = 0; i < bulletSize; i++)
	{
		bullets[i]->Update();

		if (bullets[i]->xPos > screenWidth || bullets[i]->yPos > screenHeight || bullets[i]->xPos < 0 - bullets[i]->destRect.w || bullets[i]->yPos < 0 - bullets[i]->destRect.h)
		{
			delete (*(bullets.begin() + i));
			bullets.erase(bullets.begin() + i);
			bulletSize--;
		}
	}


	//iterate through all events in this object, and call their updates so they can check whether they have been triggered.
	for (size_t i = 0; i < events.size(); i++)
	{
		events[i]->Update(this);
	}


	xPos += velocity.x;
	yPos += velocity.y;



	destRect.x = xPos;
	destRect.y = yPos;
}

void EXGameObject::Draw()
{
	if (type == GOType::Bullet)
	{
		int asd = 0;
	}

	size_t bulletSize = bullets.size();
	for (size_t i = 0; i < bulletSize; i++)
		bullets[i]->Draw();

	SDL_Point pos;
	pos.x = (destRect.w / 2);
	pos.y = (destRect.h / 2);
	SDL_RenderCopyEx(renderer, texture, NULL, &destRect, rotation, &pos, SDL_RendererFlip::SDL_FLIP_NONE);
}

void EXGameObject::AddComponent(EXEventComponent* newEvent)
{
	events.emplace_back(newEvent);
}

void EXGameObject::ShootBullet(float bulletSpeed)
{
	SDL_Rect newDest = shootingPoint.destRect;
	newDest.x = xPos + (shootingPoint.x);
	newDest.y = yPos + (shootingPoint.y);
	//EXGOBullet* newBullet = new EXGOBullet(Vector2(xPos + shootingPoint.x, yPos + shootingPoint.y), shootingPoint.bulletDir, window, renderer, Vector2{ 20.0f, 0.0f }, 0);
	EXGOBullet* newBullet = new EXGOBullet(shootingPoint.sourceRect, newDest, shootingPoint.bulletTex, window, renderer, Vector2{ bulletSpeed, 0.0f }, 0);
	bullets.emplace_back(newBullet);
}
