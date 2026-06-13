#pragma once

#include <raylib.h>

class UiItem
{
public:
	UiItem() = default;
	virtual ~UiItem() = default;

	bool IsMouseHovering(Vector2 MousePosition) const;
	Rectangle GetArea() const;
	virtual void Draw() const;
	virtual void Update() = 0;
protected:
	Rectangle Area = Rectangle(0, 0, 0, 0);
	
	Color BackgroundColor = BLACK;
	int BorderWidth = 0;
};