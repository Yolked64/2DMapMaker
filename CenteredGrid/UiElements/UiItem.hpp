#pragma once

#include <raylib.h>

class UiItem
{
public:
	UiItem() = default;
	virtual ~UiItem() = default;

	bool IsMouseHovering(Vector2 MousePosition) const;
	Rectangle GetArea() const;
	virtual void Draw(const Texture& Atlas) const;
	virtual void Update() = 0;
protected:
	Rectangle Area = Rectangle(0, 0, 0, 0);
	Rectangle AtlasArea = Rectangle(0, 0, 0, 0);

	Vector2 SpriteOrigin = Vector2(0, 0);
	float SpriteRotation = 0.0f;
};