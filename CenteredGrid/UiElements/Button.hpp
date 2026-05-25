#pragma once

#include "../Data.hpp"

class Button
{
	public:
		Button(Rectangle ScreenPosition, const std::string& TextToDisplay);

		void Draw();

		bool IsHoverred(Vector2 MousePosition);
	private:
		Rectangle ScreenPosition;
		std::string TextToDisplay;

		Color Background = ORANGE;

		Vector2 TextOffset = Vector2(16, 16);
		Color TextColor = BLACK;
		int TextSize = 16;

		void DrawContent();
};