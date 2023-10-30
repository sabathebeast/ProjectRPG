#include "raylib.h"
#include "logic.h"


int main()
{
	InitWindow(1024, 1024, "ProjectRPG");

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