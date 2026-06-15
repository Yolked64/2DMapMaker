#include "UiElements/App.hpp"

int main()
{
	SetConfigFlags(FLAG_WINDOW_HIGHDPI);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Grid is now centered");
	SetTargetFPS(FPS);
	App TileEditor = App("Atlas.png");
	while (!WindowShouldClose())
	{
		TileEditor.HandleInputs();
		TileEditor.Update();
		BeginDrawing();
		ClearBackground(BACKGROUND);
		TileEditor.Draw();
		DrawFPS(100, 100);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

// Add multiple tilemap seeable at the same time 
// Also make 2 type of tile map,  tile based and pixel based, this would make tiny details such as placing flowers possible
// Add a filked to select the name of map file
// MAKE A PROPER UI
// ADD proper update flow for ui element maybe throught call back funcs how ? I have fking no idea
// FIX THE ERROR WITH UNLAODING NOT LOADED TEXTURE IN ATLAS

/* ----OPTIONAL-------
Dont display characters inside the input box when they are out of it
*/

