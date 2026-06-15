#pragma once

#include "../Data.hpp"
#include "UiItem.hpp"

class Button : public UiItem
{
	public:
		Button(Rectangle ScreenPosition, const std::string& TextToDisplay, Color Background);
		void Draw() const override;
	private:
		std::string TextToDisplay;
		Vector2 TextOffset = Vector2(16, 16);
		Color TextColor = BLACK;
		int TextSize = 16;

		void DrawContent() const override;
};