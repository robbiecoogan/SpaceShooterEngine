#include "SDL.h"
#include "SDL_TTF.h"
#include "EngineManager.h"
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

class Vector2
{
public:
	float x;
	float y;

	Vector2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2(float x, float y)
	{
		this->x = x;
		this->x = y;
	}
};

class ShootingPoint
{
public:
	bool active;
	Vector2 pos;
	float shootingSpeed;
	SHOOTING_TYPE shootingType;
	//shot sprite
	//shot sound
	float shotDmg;
};

class Ship
{


private:


public:
	float health;
	ShootingPoint shootingPoint;
	//sprite array for death animation here
	//death sound file here
	//shooting sound file here
	//ship sprite here
	//shot sprite here

};

class Player : public Ship
{
public:
	float speed;
};

class Enemy : public Ship
{
public:
	pair <float, float> quantity;
	FORMATION formation;
	int pattern;
	pair <float, float> speed;
	float crashDmg;
	float startPos;
	float points;
};

class Boss : public Enemy
{
	enum PATTERN
	{
		slow,
		fast
	};
	ShootingPoint shootingPoint[10];
};

class background
{
	//sprite
	int layerNumber;
	float speed;
};

class ui
{
	//need more info on how the ui in sdl works
};

int main(int argc, char* args[])
{

	EngineManager *engineMain = new EngineManager();
	while (engineMain->IsRunning())
	{
		engineMain->HandleEvents();
		engineMain->Update();
		engineMain->Draw();
	}

	engineMain->Clean();
	SDL_Quit();
	TTF_Quit();

	return 0;

}