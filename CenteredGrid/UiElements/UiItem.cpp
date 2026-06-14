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
	DrawRectangleRec(this->Area, BackgroundColor);
}

void UiItem::Update()
{

}