#pragma once

#include "../Data.hpp"

class Atlas
{
	public:
		Atlas(const std::string& AtlasFileName, Vector2 ScreenPosition);

		void Draw();
		void RegisterTilePressed(Vector2 ScreenMousePosition);

		bool IsHoverred(Vector2 ScreenMousePosition);
		int LoadNewAtlas(const std::string& FileName);
		
		const Texture& GetTexture();
		Vector2 GetAtlasTilePressed();
		int GetTextureHeight();
	private:
		Texture CurrentTexture;
		
		Vector2 LastAtlasTilePressed = Vector2(0, 0);
		Rectangle Area;

		float Rotation = 0.0f;
		Vector2 Origin = Vector2(0, 0);
		Color Tint = WHITE;
		Color Background = ATLAS_BACKGROUND;
};