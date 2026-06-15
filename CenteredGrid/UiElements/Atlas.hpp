#pragma once

#include "../Data.hpp"

class Atlas
{
	public:
		Atlas(const std::string& AtlasFileName, Vector2 ScreenPosition);

		void Draw() const;
		void RegisterTilePressed(Vector2 ScreenMousePosition);

		bool IsHoverred(Vector2 ScreenMousePosition) const;
		int LoadNewAtlas(const std::string& FileName);
		
		const Texture GetTexture() const;
		Vector2 GetAtlasTilePressed() const;
		int GetTextureHeight() const;
	private:
		Texture CurrentTexture;
		
		Vector2 LastAtlasTilePressed = Vector2(0, 0);
		Rectangle Area;

		float Rotation = 0.0f;
		Vector2 Origin = Vector2(0, 0);
		Color Tint = WHITE;
		Color Background = ATLAS_BACKGROUND;
};