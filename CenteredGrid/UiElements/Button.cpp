#include "Button.hpp"

Button::Button(Rectangle ScreenPosition, const std::string& TextToDisplay, Color Background) : UiItem()
{
	this->Area = ScreenPosition;
	this->TextToDisplay = TextToDisplay;
	this->BackgroundColor = Background;
}

void Button::DrawContent() const
{
	Vector2 TextOrigin = Vector2(this->Area.x, this->Area.y);
	Vector2 TextPosition = Vector2Add(TextOrigin, this->TextOffset);
	DrawText(this->TextToDisplay.c_str(), (int)TextPosition.x, (int)TextPosition.y, this->TextSize, this->TextColor);
}

void Button::Draw() const
{
	UiItem::Draw();
	this->DrawContent();
}