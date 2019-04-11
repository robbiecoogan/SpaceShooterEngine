#include "Component.h"

void EventComponent::AddAction(ActionComponent* type)
{
	actions.emplace_back(type);
}

void EventComponent::RemoveAction(int index)
{
	actions.erase(actions.begin() + index);
}


OnThisHitByBullet::OnThisHitByBullet()
{
	type = EventComponentType::ThisHitByBullet;
}

OnThisHitByplayer::OnThisHitByplayer()
{
	type = EventComponentType::ThisHitByPlayer;
}

OnThisHitByEnemy::OnThisHitByEnemy()
{
	type = EventComponentType::ThisHitByEnemy;
}

OnMoveUp::OnMoveUp()
{
	type = EventComponentType::WhenMoveUp;
}

OnMoveDown::OnMoveDown()
{
	type = EventComponentType::WhenMoveDown;
}

OnMoveLeft::OnMoveLeft()
{
	type = EventComponentType::WhenMoveLeft;
}

OnMoveRight::OnMoveRight()
{
	type = EventComponentType::WhenMoveRight;
}
OnKeyPress::OnKeyPress(char keyPress)
{
	if (keyPress == 'w')
		type = EventComponentType::WKeyPress;
	else if (keyPress == 's')
		type = EventComponentType::SKeyPress;
	else if (keyPress == 'a')
		type = EventComponentType::AKeyPress;
	else if (keyPress == 'd')
		type = EventComponentType::DKeyPress;
	else if (keyPress == ' ')
		type = EventComponentType::SpaceKeyPress;
}
OnEveryFrame::OnEveryFrame()
{
	type = EventComponentType::EveryFrame;
}


DoAnimation::DoAnimation(std::string fileDirectory)
{
	type = ADoAnimation;
	fileDir = fileDirectory;
}

MoveRelative::MoveRelative(int xMove, int yMove)
{
	type = AMoveRelative;
}

AddSoundEffect::AddSoundEffect(std::string fileDirectory)
{
	type = ASoundEffect;
	fileDir = fileDirectory;
}
ScoreAndLives::ScoreAndLives(int score, int lives)
{
	type = AScoreLives;
}
Shoot::Shoot(float shootSpeed)
{
	type = AShoot;
}
AddVelocity::AddVelocity(float inputXVel, float inputYVel)
{
	type = AAddVelocity;
	xVel = inputXVel;
	yVel = inputYVel;
}
SubtractVelocity::SubtractVelocity(int velocity)
{
	type = ASubtractVelocity;
}
MultiplyVelocity::MultiplyVelocity(int velocity)
{
	type = AMultiplyVelocity;
}