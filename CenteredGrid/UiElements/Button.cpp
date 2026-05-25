#include "Button.hpp"

Button::Button(Rectangle ScreenPosition, const std::string& TextToDisplay)
{
	this->ScreenPosition = ScreenPosition;
	this->TextToDisplay = TextToDisplay;
}

bool Button::IsHoverred(Vector2 MousePosition)
{
	if (CheckCollisionPointRec(MousePosition, ScreenPosition))
	{
		return true;
	}
	return false;
}

void Button::DrawContent()
{
	Vector2 TextOrigin = Vector2(ScreenPosition.x, ScreenPosition.y);
	Vector2 TextPosition = Vector2Add(TextOrigin, TextOffset);
	DrawText(TextToDisplay.c_str(), (int)TextPosition.x, (int)TextPosition.y, TextSize, TextColor);
}

void Button::Draw()
{
	DrawRectangleRec(ScreenPosition, Background);
	DrawContent();
}