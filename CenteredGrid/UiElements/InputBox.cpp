#include "InputBox.hpp"

InputBox::InputBox(Rectangle ScreenPosition, const std::string& WelcomeText)
{
	this->ScreenPosition = ScreenPosition;
	this->WelcomeText = WelcomeText;
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

bool InputBox::IsHoverred(Vector2 MousePosition)
{
	if (CheckCollisionPointRec(MousePosition, ScreenPosition))
	{
		return true;
	}
	return false;
}

void InputBox::DrawContent()
{
	Vector2 TextOrigin = Vector2(ScreenPosition.x, ScreenPosition.y);
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

void InputBox::Draw()
{
	DrawRectangleRec(ScreenPosition, Background);
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
const std::string& InputBox::GetInput()
{
	return TypedText;
}