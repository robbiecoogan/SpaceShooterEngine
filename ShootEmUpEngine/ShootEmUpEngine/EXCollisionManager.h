#pragma once

#include "EXGameObject.h"
#include "SDL.h"

class EXCollisionManager
{
public:
	EXCollisionManager(std::vector<EXGameObject*> _players, std::vector<EXGOBullet*> _bullets, std::vector<EXGOEnemy*> _enemies);
	~EXCollisionManager();

	void Update();//this is called once per frame, in which all collisions will be checked

private:

	std::vector<EXGameObject*> players;
	std::vector<EXGOBullet*> bullets;
	std::vector<EXGOEnemy*> enemies;
};