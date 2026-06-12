#include "UiItem.hpp"

Rectangle UiItem::GetArea() const
{
	return this->Area;
}

bool UiItem::IsMouseHovering(Vector2 MousePosition) const
{
	return CheckCollisionPointRec(MousePosition, this->Area);
}

void UiItem::Draw(const Texture& Atlas) const
{
	DrawTexturePro(Atlas, this->AtlasArea, this->Area, this->SpriteOrigin, this->SpriteRotation, WHITE);
}