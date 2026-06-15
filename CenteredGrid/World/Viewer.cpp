#include "Viewer.hpp"

WorldDisplayer::WorldDisplayer()
{
	this->Displayer = Camera2D();
	this->Displayer.offset = Vector2(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
	this->Displayer.target = Vector2(0, 0);
	this->Displayer.zoom = 1.0f;
	this->Displayer.rotation = 0.0f;
}

void WorldDisplayer::Move(int XDirection, int YDirection, float DeltaT)
{
	this->Displayer.target.x += XDirection * MOVEMENT_SPEED * DeltaT / this->Displayer.zoom;
	this->Displayer.target.y += YDirection * MOVEMENT_SPEED * DeltaT / this->Displayer.zoom;
}

void WorldDisplayer::MultiplyZoom()
{
	if (this->Displayer.zoom <= MAX_ZOOM)
	{
		this->Displayer.zoom *= ZOOM_FACTOR;
	}
}

void WorldDisplayer::DivideZoom()
{
	if (this->Displayer.zoom >= MIN_ZOOM)
	{
		this->Displayer.zoom /= ZOOM_FACTOR;
	}
}

void WorldDisplayer::UseCamera()
{
	BeginMode2D(this->Displayer);
}

void WorldDisplayer::QuitCamera()
{
	EndMode2D();
}

Camera2D WorldDisplayer::GetCamera() const
{
	return this->Displayer;
}