#pragma once

#include <vector>
#include <string>

class EXGameObject;
class EXGOBullet;
class EXGOEnemy;


enum EXEventComponentType
{
	EXThisHitByBullet,
	EXThisHitByPlayer,
	EXThisHitByEnemy,
	EXWhenMoveUp,
	EXWhenMoveDown,
	EXWhenMoveLeft,
	EXWhenMoveRight,
	EXWKeyPress,
	EXSKeyPress,
	EXAKeyPress,
	EXDKeyPress,
	EXSpaceKeyPress,
	EXEveryFrame,
};
enum EXActionComponentType
{
	EXA_DoAnimation,
	EXA_MoveRelative,
	EXA_SoundEffect,
	EXA_ScoreLives,
	EXA_Shoot,
	EXA_AddVelocity,
	EXA_SubtractVelocity,
	EXA_MultiplyVelocity
};

class EXActionComponent
{
public:
	EXActionComponentType type;
	virtual void DoAction(EXGameObject* pGO) = 0;
};

class EXEventComponent
{

public:
	EXEventComponentType type;
	void AddAction(EXActionComponent* type);
	void RemoveAction(int index);

	virtual void Update(EXGameObject* pGO) = 0;

	std::vector<EXActionComponent*> actions;
};

class EXOnThisHitByBullet : public EXEventComponent
{
public:
	EXOnThisHitByBullet();
	void Update(EXGameObject* pGO) final override;

	EXGameObject* myGO;
};

class EXOnThisHitByplayer : public EXEventComponent
{
public:
	EXOnThisHitByplayer();
	void Update(EXGameObject* pGO) final override;
};

class EXOnThisHitByEnemy : public EXEventComponent
{
public:
	EXOnThisHitByEnemy();
	void Update(EXGameObject* pGO) final override;
};

class EXOnMoveUp : public EXEventComponent
{
public:
	EXOnMoveUp();
	void Update(EXGameObject* pGO) final override;
private:
	int prevXPos, prevYPos;
};

class EXOnMoveDown : public EXEventComponent
{
public:
	EXOnMoveDown();
	void Update(EXGameObject* pGO) final override;
private:
	int prevXPos, prevYPos;
};

class EXOnMoveLeft : public EXEventComponent
{
public:
	EXOnMoveLeft();
	void Update(EXGameObject* pGO) final override;
private:
	int prevXPos, prevYPos;
};

class EXOnMoveRight : public EXEventComponent
{
public:
	EXOnMoveRight();
	void Update(EXGameObject* pGO) final override;
private:
	int prevXPos, prevYPos;
};
class EXOnKeyPress : public EXEventComponent
{
public:
	EXOnKeyPress(char keyPress);
	char keyPressed;
	void Update(EXGameObject* pGO) final override;
private:

};
class EXOnEveryFrame : public EXEventComponent
{
public:
	EXOnEveryFrame();

	void Update(EXGameObject* pGO) final override;
private:
};

////////////////////////////////////////////////////////////////////////////////

class EXDoAnimation : public EXActionComponent
{
public:
	EXDoAnimation(std::string fileDirectory, int inputFrames, int inputDestWidth, int inputDestHeight);
	std::string fileDir;
	int frames = 0;
	int destWidth = 0, destHeight = 0;
	void DoAction(EXGameObject* pGO) final override;
private:
};

class EXMoveRelative : public EXActionComponent
{
public:
	EXMoveRelative(int xMove, int yMove);
	int X, Y;
	void DoAction(EXGameObject* pGO) final override;
private:

};
class  EXAddSoundEffect : public EXActionComponent
{
public:
	EXAddSoundEffect(std::string fileDirectory);
	std::string fileDir;
	void DoAction(EXGameObject* pGO) final override;
private:
};

class EXScoreAndLives : public EXActionComponent
{
public:
	EXScoreAndLives(int score, int lives);
	int playerScore, playerLives;
	void DoAction(EXGameObject* pGO) final override;
private:

};
class EXShoot : public EXActionComponent
{
public:
	EXShoot(float shootSpeed, float IshootFrequency);
	float bulletSpeed = 0;
	float shootFrequency = 0;
	float timer = 0;
	void DoAction(EXGameObject* pGO) final override;
private:

};
class EXAddVelocity : public EXActionComponent
{
public:
	EXAddVelocity(float inputXVel, float inputYVel);
	float xVel;
	float yVel;
	void DoAction(EXGameObject* pGO) final override;
private:

};
class EXSubtractVelocity : public EXActionComponent
{
public:
	EXSubtractVelocity(float inputXVel, float inputYVel);
	float xVel;
	float yVel;
	void DoAction(EXGameObject* pGO) final override;
private:

};
class EXMultiplyVelocity : public EXActionComponent
{
public:
	EXMultiplyVelocity(float inputXVel, float inputYVel);
	float xVel;
	float yVel;
	void DoAction(EXGameObject* pGO) final override;
private:

};
