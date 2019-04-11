#pragma once

struct Vector2
{
	float x;
	float y;

	Vector2()
	{
		x = 0;
		y = 0;
	}

	Vector2(float inputX, float inputY)
	{
		x = inputX;
		y = inputY;
	}

	Vector2 operator += (Vector2 &other)	{	return Vector2(x + other.x, y + other.y);			}

	static Vector2 Zero()					{	return Vector2(0, 0);								}

	Vector2 operator *(const float &ref)	{	return Vector2((float)(x * ref), (float)(y * ref));	}
};

enum GOType
{
	Player,
	Enemy,
	Background,
	Bullet,
};