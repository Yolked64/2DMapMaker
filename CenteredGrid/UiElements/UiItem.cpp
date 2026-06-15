#include "UiItem.hpp"

Rectangle UiItem::GetArea() const
{
	return this->Area;
}

bool UiItem::IsMouseHovering(Vector2 MousePosition) const
{
	return CheckCollisionPointRec(MousePosition, this->Area);
}

void UiItem::Draw() const
{
	DrawRectangleRec(this->Area, this->BackgroundColor);
}

void UiItem::Update()
{

}

void UiItem::DrawContent() const 
{

}