#pragma once

#include <string>
#include <iostream>
#include "GameObject.h"



struct SelectionBoxItem
{
	std::string name;
	bool isGameObject;
	GameObject* gameObject;
};