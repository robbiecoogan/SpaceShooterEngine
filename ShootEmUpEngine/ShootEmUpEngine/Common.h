#pragma once

#include <string>
#include <iostream>
#include "GameObject.h"
#include "EventManager.h"

struct SelectionBoxItem
{
	//in this struct, the void* cannot be a GameObject*. This is to prevent circular dependency

	std::string name;
	bool isGameObject;
	GameObject* gameObject;//is a pointer to a gameObject

	SelectionBoxItem(string inputName)
	{
		name = inputName;
		isGameObject = false;
		gameObject = nullptr;
	}

	~SelectionBoxItem()
	{
		if (isGameObject)
		{
			delete gameObject;
		}
	}

	SelectionBoxItem(string inputName, bool inputIsGameObject, GameObject* pGameObject)
	{
		name = inputName;
		isGameObject = inputIsGameObject;
		gameObject = pGameObject;
	}
};