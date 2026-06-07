#include "UiElements/UserInterface.hpp"

int main()
{
	SetConfigFlags(FLAG_WINDOW_HIGHDPI);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Grid is now centered");
	SetTargetFPS(FPS);
	UserInterface App = UserInterface("Atlas.png");
	while (!WindowShouldClose())
	{
		App.HandleInputs();
		App.Update();
		BeginDrawing();
		ClearBackground(BACKGROUND);
		App.Draw();
		DrawFPS(100, 100);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

// Add multiple tilemap seeable at the same time 
// Also make 2 type of tile map,  tile based and pixel based, this would make tiny details such as placing flowers possible
// Add a filked to select the name of map file

/* ----OPTIONAL-------
Dont display characters inside the input box when they are out of it
*/

// CONTROL Z WORKS BUT IT STORES MULTIPLE TILES MAYBE DO IT ONCE PER STROKE
/*
Do a func to begin a pencil mode when mouse is pressed
add the tiles changed to a vector if the mode is true
when the button is no longer held, add that vector to actions
then clear the previous vector
to detetec when to add check if the mode is false and if the vector is not empty
*/
