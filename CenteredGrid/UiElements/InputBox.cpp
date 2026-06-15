#include "InputBox.hpp"

InputBox::InputBox(Rectangle ScreenPosition, const std::string& WelcomeText, Color Background) : UiItem()
{
	this->Area = ScreenPosition;
	this->WelcomeText = WelcomeText;
	this->BackgroundColor = Background;
}

void InputBox::AddCharacter(int AsciiCode)
{
	this->TypedText.push_back(AsciiCode);
}

void InputBox::DeleteLastCharacter()
{
	if (this->TypedText.size() != 0)
	{
		this->TypedText.pop_back();
	}
}

void InputBox::ResetContentAndDisplayWelcome()
{
	this->TypedText.clear();
	this->DisplayWelcomeText = true;
}

void InputBox::DrawContent() const
{
	Vector2 TextOrigin = Vector2(this->Area.x, this->Area.y);
	Vector2 TextPosition = Vector2Add(TextOrigin, this->TextOffset);
	if (this->DisplayWelcomeText)
	{
		DrawText(this->WelcomeText.c_str(), (int)TextPosition.x, (int)TextPosition.y, this->TextSize, this->TextColor);
	}
	else
	{
		DrawText(this->TypedText.c_str(), (int)TextPosition.x, (int)TextPosition.y, this->TextSize, this->TextColor);
	}
}

void InputBox::Draw() const
{
	UiItem::Draw();
	this->DrawContent();
}

void InputBox::Update()
{
	if (this->TypedText.size() != 0)
	{
		this->DisplayWelcomeText = false;
	}
	else
	{
		this->DisplayWelcomeText = true;
	}
}
const std::string& InputBox::GetInput() const
{
	return this->TypedText;
}