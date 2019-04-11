#pragma once

#include <vector>
#include <string>

enum EventComponentType
{
	ThisHitByBullet,
	ThisHitByPlayer,
	ThisHitByEnemy,
	WhenMoveUp,
	WhenMoveDown,
	WhenMoveLeft,
	WhenMoveRight,
	WKeyPress,
	SKeyPress,
	AKeyPress,
	DKeyPress,
	SpaceKeyPress,
	EveryFrame,
};
enum ActionComponentType
{
	ADoAnimation,
	AMoveRelative,
	ASoundEffect,
	AScoreLives,
	AShoot,
	AAddVelocity,
	ASubtractVelocity,
	AMultiplyVelocity
};

class ActionComponent
{
public:
	ActionComponentType type;
};

class EventComponent
{
	
public:
	EventComponentType type;
	void AddAction(ActionComponent* type);
	void RemoveAction(int ind);
	std::vector<ActionComponent*> actions;
};

class OnThisHitByBullet : public EventComponent
{
public:
	OnThisHitByBullet();
};

class OnThisHitByplayer : public EventComponent
{
public:
	OnThisHitByplayer();
};

class OnThisHitByEnemy : public EventComponent
{
public:
	OnThisHitByEnemy();
};

class OnMoveUp : public EventComponent
{
public:
	OnMoveUp();
};

class OnMoveDown : public EventComponent
{
public:
	OnMoveDown();
};

class OnMoveLeft : public EventComponent
{
public:
	OnMoveLeft();
};

class OnMoveRight : public EventComponent
{
public:
	OnMoveRight();
private:
};
class OnKeyPress : public EventComponent
{
public:
	OnKeyPress(char keyPress);
	char keyPressed;
private:

};
class OnEveryFrame : public EventComponent
{
public:
	OnEveryFrame();

private:
};

class DoAnimation : public ActionComponent
{
public:
	DoAnimation(std::string fileDirectory);
	std::string fileDir;
	int frames;
private:
};

class MoveRelative : public ActionComponent
{
public:
	MoveRelative(int xMove, int yMove);
	int X, Y;
private:

};
class  AddSoundEffect : public ActionComponent
{
public:
	AddSoundEffect(std::string fileDirectory);
	std::string fileDir;
private:
};

class ScoreAndLives : public ActionComponent
{
public:
	ScoreAndLives(int score, int lives);
	int playerScore, playerLives;
private:

};
class Shoot : public ActionComponent
{
public:
	Shoot(float shootSpeed);
	float bulletSpeed;
private:

};
class AddVelocity : public ActionComponent
{
public:
	AddVelocity(float inputXVel, float inputYVel);
	float xVel, yVel;
private:

};
class SubtractVelocity : public ActionComponent
{
public:
	SubtractVelocity(int velocity);
	int velocity;
private:

};
class MultiplyVelocity : public ActionComponent
{
public:
	MultiplyVelocity(int velocity);
	int velocity;
private:

};
