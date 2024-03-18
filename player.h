#pragma once

#include <string>

class Player
{
public:
	float speed = 0.f;
	int framesX = 0;
	int framesY = 0;
	Vector2 location = { 0.f, 0.f };
	Vector2 direction = { 0.f, 0.f };
	std::string name = "";
	bool isNameGiven = true;
};