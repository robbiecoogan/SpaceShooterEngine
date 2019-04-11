#include "EXComponent.h"
#include "EventManager.h"
#include "EXGameObject.h"

void EXEventComponent::AddAction(EXActionComponent* type)
{
	actions.emplace_back(type);
}

void EXEventComponent::RemoveAction(int index)
{
	actions.erase(actions.begin() + index);
}

EXOnThisHitByBullet::EXOnThisHitByBullet()
{
	type = EXEventComponentType::EXThisHitByBullet;
}

void EXOnThisHitByBullet::Update(EXGameObject* pGO)
{
}

EXOnThisHitByplayer::EXOnThisHitByplayer()
{
	type = EXEventComponentType::EXThisHitByPlayer;
}

void EXOnThisHitByplayer::Update(EXGameObject* pGO)
{
}

EXOnThisHitByEnemy::EXOnThisHitByEnemy()
{
	type = EXEventComponentType::EXThisHitByEnemy;
}

void EXOnThisHitByEnemy::Update(EXGameObject* pGO)
{
}

EXOnMoveUp::EXOnMoveUp()
{
	type = EXEventComponentType::EXWhenMoveUp;
	prevXPos = -1;
	prevYPos = -1;
}

void EXOnMoveUp::Update(EXGameObject* pGO)
{
	int currYPos = pGO->yPos;

	if (prevYPos != -1 && prevYPos > currYPos)
	{
		int actionCount = actions.size();
		for (size_t i = 0; i < actionCount; i++)
		{
			actions[i]->DoAction(pGO);
		}
	}

	prevXPos = pGO->xPos;
	prevYPos = pGO->yPos;
}

EXOnMoveDown::EXOnMoveDown()
{
	type = EXEventComponentType::EXWhenMoveDown;
	prevXPos = -1;
	prevYPos = -1;
}

void EXOnMoveDown::Update(EXGameObject* pGO)
{
	int currYPos = pGO->yPos;

	if (prevYPos != -1 && prevYPos < currYPos)
	{
		int actionCount = actions.size();
		for (size_t i = 0; i < actionCount; i++)
		{
			actions[i]->DoAction(pGO);
		}
	}

	prevXPos = pGO->xPos;
	prevYPos = pGO->yPos;
}

EXOnMoveLeft::EXOnMoveLeft()
{
	type = EXEventComponentType::EXWhenMoveLeft;
	prevXPos = -1;
	prevYPos = -1;
}

void EXOnMoveLeft::Update(EXGameObject* pGO)
{
	int currXPos = pGO->xPos;

	if (prevXPos != -1 && prevXPos > currXPos)
	{
		int actionCount = actions.size();
		for (size_t i = 0; i < actionCount; i++)
		{
			actions[i]->DoAction(pGO);
		}
	}

	prevXPos = pGO->xPos;
	prevYPos = pGO->yPos;
}

EXOnMoveRight::EXOnMoveRight()
{
	type = EXEventComponentType::EXWhenMoveRight;
	prevXPos = -1;
	prevYPos = -1;
}
void EXOnMoveRight::Update(EXGameObject* pGO)
{
	int currXPos = pGO->xPos;

	if (prevXPos != -1 && prevXPos < currXPos)
	{
		int actionCount = actions.size();
		for (size_t i = 0; i < actionCount; i++)
		{
			actions[i]->DoAction(pGO);
		}
	}

	prevXPos = pGO->xPos;
	prevYPos = pGO->yPos;
}


EXOnKeyPress::EXOnKeyPress(char keyPress)
{
	if (keyPress == 'w')
		type = EXEventComponentType::EXWKeyPress;
	else if (keyPress == 's')
		type = EXEventComponentType::EXSKeyPress;
	else if (keyPress == 'a')
		type = EXEventComponentType::EXAKeyPress;
	else if (keyPress == 'd')
		type = EXEventComponentType::EXDKeyPress;
	else if (keyPress == ' ')
		type = EXEventComponentType::EXSpaceKeyPress;
}
void EXOnKeyPress::Update(EXGameObject* pGO)
{
	//if the appropriate key is down, perform all actions in this object's action list.
	std::vector<SDL_Event*> e = EventManager::getInstance().GetEvent(SDL_KEYDOWN);
	int eSize = e.size();
	

	static bool wIsDown = false;
	static bool sIsDown = false;
	static bool aIsDown = false;
	static bool dIsDown = false;
	static bool spaceIsDown = false;

	for (size_t i = 0; i < eSize; i++)
	{
		switch (type)
		{
		case EXWKeyPress:
			if (e[i]->key.keysym.sym == SDLK_w)
				wIsDown = true;
			break;
		case EXSKeyPress:
			if (e[i]->key.keysym.sym == SDLK_s)
				sIsDown = true;
			break;
		case EXAKeyPress:
			if (e[i]->key.keysym.sym == SDLK_a)
				aIsDown = true;
			break;
		case EXDKeyPress:
			if (e[i]->key.keysym.sym == SDLK_d)
				dIsDown = true;
		case EXSpaceKeyPress:
			if (e[i]->key.keysym.sym == SDLK_SPACE)
				spaceIsDown = true;
			break;
		default:
			break;
		}
	}

	if (wIsDown || sIsDown || aIsDown || dIsDown || spaceIsDown)
	{
		int actionCount = 0;
		switch (type)
		{
		case EXWKeyPress:
			actionCount = actions.size();
			for (size_t k = 0; k < actionCount; k++)
				if (wIsDown)
					actions[k]->DoAction(pGO);
			break;
		case EXSKeyPress:
			actionCount = actions.size();
			for (size_t k = 0; k < actionCount; k++)
				if (sIsDown)
					actions[k]->DoAction(pGO);
			break;
		case EXAKeyPress:
			actionCount = actions.size();
			for (size_t k = 0; k < actionCount; k++)
				if (aIsDown)
					actions[k]->DoAction(pGO);
			break;
		case EXDKeyPress:
			actionCount = actions.size();
			for (size_t k = 0; k < actionCount; k++)
				if (dIsDown)
					actions[k]->DoAction(pGO);
			break;
		case EXSpaceKeyPress:
			actionCount = actions.size();
			for (size_t k = 0; k < actionCount; k++)
				if (spaceIsDown)
					actions[k]->DoAction(pGO);
			break;
		default:
			break;
		}

		//std::cout << "DOING ACTIONS\n";
	}

	//if (wIsDown) std::cout << "W is down\n";
	//if (sIsDown) std::cout << "S is down\n";
	//if (aIsDown) std::cout << "A is down\n";
	//if (dIsDown) std::cout << "D is down\n";
	//if (spaceIsDown) std::cout << "D is down\n";

	e = EventManager::getInstance().GetEvent(SDL_KEYUP);
	eSize = e.size();

	for (size_t i = 0; i < eSize; i++)
	{
		switch (type)
		{
		case EXWKeyPress:
			if (e[i]->key.keysym.sym == SDLK_w)
				wIsDown = false;
			break;
		case EXSKeyPress:
			if (e[i]->key.keysym.sym == SDLK_s)
				sIsDown = false;
			break;
		case EXAKeyPress:
			if (e[i]->key.keysym.sym == SDLK_a)
				aIsDown = false;
			break;
		case EXDKeyPress:
			if (e[i]->key.keysym.sym == SDLK_d)
				dIsDown = false;
			break;
		case EXSpaceKeyPress:
			if (e[i]->key.keysym.sym == SDLK_SPACE)
				spaceIsDown = false;
			break;
		default:
			break;
		}
	}

}

EXOnEveryFrame::EXOnEveryFrame()
{
	type = EXEventComponentType::EXEveryFrame;
}

void EXOnEveryFrame::Update(EXGameObject* pGO)
{
	int actionCount = actions.size();
	for (size_t i = 0; i < actionCount; i++)
	{
		actions[i]->DoAction(pGO);
	}
}


EXDoAnimation::EXDoAnimation(std::string fileDirectory, int inputFrames, int inputDestWidth, int inputDestHeight)
{
	type = EXA_DoAnimation;
	fileDir = fileDirectory;
	frames = inputFrames;
	destWidth = inputDestWidth;
	destHeight = inputDestHeight;
}

void EXDoAnimation::DoAction(EXGameObject* pGO)
{

}


EXMoveRelative::EXMoveRelative(int xMove, int yMove)
{
	type = EXA_MoveRelative;
	X = xMove;
	Y = yMove;
}


void EXMoveRelative::DoAction(EXGameObject* pGO)
{
	pGO->xPos += X;
	pGO->yPos += Y;
}


EXAddSoundEffect::EXAddSoundEffect(std::string fileDirectory)
{
	type = EXA_SoundEffect;
	fileDir = fileDirectory;
}
void EXAddSoundEffect::DoAction(EXGameObject* pGO)
{

}


EXScoreAndLives::EXScoreAndLives(int score, int lives)
{
	type = EXA_ScoreLives;
}
void EXScoreAndLives::DoAction(EXGameObject* pGO)
{

}


EXShoot::EXShoot(float shootSpeed, float iShootFrequency)//shootfrequency is in seconds
{
	type = EXA_Shoot;
	bulletSpeed = shootSpeed;
	shootFrequency = iShootFrequency;
	timer = 0;
}
void EXShoot::DoAction(EXGameObject* pGO)
{
	if (shootFrequency == 0)
		pGO->ShootBullet(bulletSpeed);
	else
	{
		float freqConvert = shootFrequency;
		timer += EventManager::dTime;
		if (timer >= freqConvert)
		{
			pGO->ShootBullet(bulletSpeed);
			timer = 0;
		}
	}
}


EXAddVelocity::EXAddVelocity(float inputXVel, float inputYVel)
{
	type = EXA_AddVelocity;
	xVel = inputXVel;
	yVel = inputYVel;
}
void EXAddVelocity::DoAction(EXGameObject* pGO)
{
	pGO->velocity.x += xVel;
	pGO->velocity.y += yVel;
}


EXSubtractVelocity::EXSubtractVelocity(float inputXVel, float inputYVel)
{
	type = EXA_SubtractVelocity;
	xVel = inputXVel;
	yVel = inputYVel;
}
void EXSubtractVelocity::DoAction(EXGameObject* pGO)
{
	pGO->velocity.x -= xVel;
	pGO->velocity.y -= yVel;
}


EXMultiplyVelocity::EXMultiplyVelocity(float inputXVel, float inputYVel)
{
	type = EXA_MultiplyVelocity;
	xVel = inputXVel;
	yVel = inputYVel;
}

void EXMultiplyVelocity::DoAction(EXGameObject* pGO)
{
	pGO->velocity.x *= xVel;
	pGO->velocity.y *= yVel;
}
