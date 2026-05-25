#include "Viewer.hpp"

WorldDisplayer::WorldDisplayer()
{
	Displayer = Camera2D();
	Displayer.offset = Vector2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
	Displayer.target = Vector2(0, 0);
	Displayer.zoom = 1.0f;
	Displayer.rotation = 0.0f;
}

void WorldDisplayer::Move(int XDirection, int YDirection, float DeltaT)
{
	Displayer.target.x += XDirection * MOVEMENT_SPEED * DeltaT / Displayer.zoom;
	Displayer.target.y += YDirection * MOVEMENT_SPEED * DeltaT / Displayer.zoom;
}

void WorldDisplayer::MultiplyZoom()
{
	if (Displayer.zoom <= MAX_ZOOM)
	{
		Displayer.zoom *= ZOOM_FACTOR;
	}
}

void WorldDisplayer::DivideZoom()
{
	if (Displayer.zoom >= MIN_ZOOM)
	{
		Displayer.zoom /= ZOOM_FACTOR;
	}
}

void WorldDisplayer::UseCamera()
{
	BeginMode2D(Displayer);
}

void WorldDisplayer::QuitCamera()
{
	EndMode2D();
}

Camera2D& WorldDisplayer::GetCamera()
{
	return Displayer;
}