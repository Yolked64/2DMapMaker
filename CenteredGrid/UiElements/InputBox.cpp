#include "InputBox.hpp"

InputBox::InputBox(Rectangle ScreenPosition, const std::string& WelcomeText, Color Background) : UiItem()
{
	this->Area = ScreenPosition;
	this->WelcomeText = WelcomeText;
	this->BackgroundColor = Background;
}

void InputBox::AddCharacter(int AsciiCode)
{
	TypedText.push_back(AsciiCode);
}

void InputBox::DeleteLastCharacter()
{
	if (TypedText.size() != 0)
	{
		TypedText.pop_back();
	}
}

void InputBox::ResetContentAndDisplayWelcome()
{
	TypedText.clear();
	DisplayWelcomeText = true;
}

void InputBox::DrawContent() const
{
	Vector2 TextOrigin = Vector2(Area.x, Area.y);
	Vector2 TextPosition = Vector2Add(TextOrigin, TextOffset);
	if (DisplayWelcomeText)
	{
		DrawText(WelcomeText.c_str(), (int)TextPosition.x, (int)TextPosition.y, TextSize, TextColor);
	}
	else
	{
		DrawText(TypedText.c_str(), (int)TextPosition.x, (int)TextPosition.y, TextSize, TextColor);
	}
}

void InputBox::Draw() const
{
	UiItem::Draw();
	DrawContent();
}

void InputBox::Update()
{
	if (TypedText.size() != 0)
	{
		DisplayWelcomeText = false;
	}
	else
	{
		DisplayWelcomeText = true;
	}
}
const std::string& InputBox::GetInput() const
{
	return TypedText;
}