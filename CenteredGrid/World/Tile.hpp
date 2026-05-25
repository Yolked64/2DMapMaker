#pragma once

#include "../Data.hpp"

class Tile
{
	public:
		Tile() = default;
		Tile(Vector2 GridPosition, Vector2 AtlasPosition, int AtlasHeight);

		void Set(Vector2 GridPosition, Vector2 AtlasPosition, int AtlasHeight);
		void Reset();

		void Draw(const Texture& Atlas);

		void CutPasteTo(Tile& Case, int AtlasHeight);
		bool IsSameAs(Vector2 TileGridPosition, Vector2 TileAtlasPosition);

		Vector2 GetGridPosition();
		Vector2 GetAtlasTilePosition();
	private:
		Vector2 GridPosition = Vector2((float)INVALID_INDEX, (float)INVALID_INDEX);
		Vector2 WorldPosition = Vector2((float)INVALID_INDEX, (float)INVALID_INDEX);
		Vector2 AtlasTilePosition = Vector2((float)INVALID_INDEX, (float)INVALID_INDEX);
		Vector2 AtlasPixelPosition = Vector2((float)INVALID_INDEX, (float)INVALID_INDEX);
		bool Initialized = false;

		float Rotation = 0.0f;
		Vector2 Origin = Vector2(0, 0);
		Color Tint = WHITE;
};