#pragma once

#include <raylib.h>

class UiItem
{
public:
	UiItem() = default;
	virtual ~UiItem() = default;

	bool IsMouseHovering(Vector2 MousePosition) const;
	Rectangle GetArea() const;
	virtual void Update();
	virtual void Draw(const Texture& Atlas);
protected:
	Rectangle Area;
};