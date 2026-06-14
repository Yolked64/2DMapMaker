#include "Button.hpp"

Button::Button(Rectangle ScreenPosition, const std::string& TextToDisplay, Color Background) : UiItem()
{
	this->Area = ScreenPosition;
	this->TextToDisplay = TextToDisplay;
	this->BackgroundColor = Background;
}

void Button::DrawContent() const
{
	Vector2 TextOrigin = Vector2(Area.x, Area.y);
	Vector2 TextPosition = Vector2Add(TextOrigin, TextOffset);
	DrawText(TextToDisplay.c_str(), (int)TextPosition.x, (int)TextPosition.y, TextSize, TextColor);
}

void Button::Draw() const
{
	UiItem::Draw();
	DrawContent();
}