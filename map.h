#pragma once
#include "raylib.h"

class Map
{
public:
	Map();
	~Map();

	void loadMap(int arr[64][64]);
	void drawMap(Texture tex0, Texture tex1, Texture tex2);

private:
	int map[64][64];
};
