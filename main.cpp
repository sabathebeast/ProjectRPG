#include "raylib.h"
#include "logic.h"

int main()
{
	InitWindow(Logic::windowWidth, Logic::windowHeight, "ProjectRPG");

	Logic logic = Logic();

	while (!WindowShouldClose())
	{
		BeginDrawing();
		logic.Render();
		ClearBackground(WHITE);
		logic.Update();
		EndDrawing();
	}

	CloseWindow();
	return 0;
};