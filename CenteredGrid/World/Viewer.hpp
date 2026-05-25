#pragma once

#include "../Data.hpp"

class WorldDisplayer
{
	public:
		WorldDisplayer();

		void MultiplyZoom();
		void DivideZoom();
		void Move(int XDirection, int YDirection, float DeltaT);

		void UseCamera();
		void QuitCamera();

		Camera2D& GetCamera();
	private:
		Camera2D Displayer;
};