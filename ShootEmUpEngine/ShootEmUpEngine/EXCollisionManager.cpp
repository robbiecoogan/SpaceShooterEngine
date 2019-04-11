#include "EXCollisionManager.h"

EXCollisionManager::EXCollisionManager(std::vector<EXGameObject*> _players, std::vector<EXGOBullet*> _bullets, std::vector<EXGOEnemy*> _enemies)
{
	players = _players;
	bullets = _bullets;
	enemies = _enemies;
}

EXCollisionManager::~EXCollisionManager()
{
}

void EXCollisionManager::Update()
{
	//check through everything to see if it is colliding with anything else (bullets dont collide with each other however)

	int playerSize = players.size();
	int bulletSize = bullets.size();
	int enemySize = enemies.size();

	////////////////////////LAMBDA/////////////////////////////////
	auto ActionBulletCollision = [](EXGameObject* pGO)
	{
		int eventSize = pGO->events.size();
		for (size_t i = 0; i < eventSize; i++)
		{
			if (pGO->events[i]->type == EXEventComponentType::EXThisHitByBullet)
			{
				int actionSize = pGO->events[i]->actions.size();
				for (size_t j = 0; j < actionSize; j++)
					pGO->events[i]->actions[j]->DoAction(pGO);
			}
		}
	};

	auto ActionEnemyCollision = [](EXGameObject* pGO)
	{
		int eventSize = pGO->events.size();
		for (size_t i = 0; i < eventSize; i++)
		{
			if (pGO->events[i]->type == EXEventComponentType::EXThisHitByEnemy)
			{
				int actionSize = pGO->events[i]->actions.size();
				for (size_t j = 0; j < actionSize; j++)
					pGO->events[i]->actions[j]->DoAction(pGO);
			}
		}
	};

	auto ActionPlayerCollision = [](EXGameObject* pGO)
	{
		int eventSize = pGO->events.size();
		for (size_t i = 0; i < eventSize; i++)
		{
			if (pGO->events[i]->type == EXEventComponentType::EXThisHitByPlayer)
			{
				int actionSize = pGO->events[i]->actions.size();
				for (size_t j = 0; j < actionSize; j++)
					pGO->events[i]->actions[j]->DoAction(pGO);
			}
		}
	};
	///////////////////////////////////////////////////////////////

	//Check for player colliding with stuff
	for (size_t i = 0; i < playerSize; i++)
	{
		SDL_Rect thisRect = players[i]->destRect;

		for (size_t j = 0; j < bulletSize; j++)
		{
			SDL_Rect otherRect = bullets[j]->destRect;
			if (otherRect.x > thisRect.x && otherRect.x < thisRect.x + thisRect.w
				&& otherRect.y > thisRect.y && otherRect.y < thisRect.y + thisRect.h)
			{
				ActionBulletCollision(players[i]);
			}
		}

		for (size_t j = 0; j < enemySize; j++)
		{
			SDL_Rect otherRect = enemies[j]->destRect;
			if (otherRect.x > thisRect.x && otherRect.x < thisRect.x + thisRect.w
				&& otherRect.y > thisRect.y && otherRect.y < thisRect.y + thisRect.h)
			{
				ActionEnemyCollision(players[i]);
			}
		}

		for (size_t j = 0; j < playerSize; j++)
		{
			if (j != i)//don't want to compare the current player to itself
			{
				SDL_Rect otherRect = players[j]->destRect;
				if (otherRect.x > thisRect.x && otherRect.x < thisRect.x + thisRect.w
					&& otherRect.y > thisRect.y && otherRect.y < thisRect.y + thisRect.h)
				{
					ActionPlayerCollision(players[i]);
				}
			}
		}
	}

	//Check for enemy colliding with stuff
	for (size_t i = 0; i < enemySize; i++)
	{
		SDL_Rect thisRect = enemies[i]->destRect;

		for (size_t j = 0; j < bulletSize; j++)
		{
			SDL_Rect otherRect = bullets[j]->destRect;
			if (otherRect.x > thisRect.x && otherRect.x < thisRect.x + thisRect.w
				&& otherRect.y > thisRect.y && otherRect.y < thisRect.y + thisRect.h)
			{
				ActionBulletCollision(enemies[i]);
			}
		}

		for (size_t j = 0; j < playerSize; j++)
		{
			SDL_Rect otherRect = players[j]->destRect;
			if (otherRect.x > thisRect.x && otherRect.x < thisRect.x + thisRect.w
				&& otherRect.y > thisRect.y && otherRect.y < thisRect.y + thisRect.h)
			{
				ActionPlayerCollision(enemies[i]);
			}
		}

		for (size_t j = 0; j < enemySize; j++)
		{
			if (j != i)//don't want to compare the current enemy to itself
			{
				SDL_Rect otherRect = enemies[j]->destRect;
				if (otherRect.x > thisRect.x && otherRect.x < thisRect.x + thisRect.w
					&& otherRect.y > thisRect.y && otherRect.y < thisRect.y + thisRect.h)
				{
					ActionEnemyCollision(enemies[i]);
				}
			}
		}
	}
}
