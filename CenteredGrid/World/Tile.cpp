#include "Tile.hpp"

Tile::Tile(Vector2 GridPosition, Vector2 AtlasPixelPosition, int AtlasHeight)
{
	this->GridPosition = GridPosition;
	this->WorldPosition = GridToWorld(GridPosition);
	this->AtlasPixelPosition = AtlasPixelPosition;
	Initialized = true;
}

void Tile::Set(Vector2 GridPosition, Vector2 AtlasPixelPosition, int AtlasHeight)
{
	this->GridPosition = GridPosition;
	this->AtlasPixelPosition = AtlasPixelPosition;
	this->WorldPosition = GridToWorld(GridPosition);
	Initialized = true;
}

void Tile::Reset()
{
	GridPosition = Vector2((float)INVALID_INDEX, (float)INVALID_INDEX);
	WorldPosition = Vector2((float)INVALID_INDEX, (float)INVALID_INDEX);
	AtlasPixelPosition = Vector2((float)INVALID_INDEX, (float)INVALID_INDEX);
	Initialized = false;
}

void Tile::CutPasteTo(Tile& Case, int AtlasHeight)
{
	Case.Set(GridPosition, AtlasPixelPosition, AtlasHeight);
	Reset();
}

bool Tile::IsSameAs(Vector2 TileGridPosition, Vector2 PixelAtlasPosition)
{
	if (!Vector2Equals(GridPosition, TileGridPosition))
	{
		return false;
	}
	if (!Vector2Equals(AtlasPixelPosition, PixelAtlasPosition))
	{
		return false;
	}
	return true;
}

void Tile::Draw(const Texture& Atlas)
{
	if (Initialized)
	{
		Rectangle Source = Rectangle(AtlasPixelPosition.x, AtlasPixelPosition.y, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
		Rectangle Destination = Rectangle(WorldPosition.x, WorldPosition.y, TILE_SIZE, TILE_SIZE);
		DrawTexturePro(Atlas, Source, Destination, Origin, Rotation, Tint);
	}
}

Vector2 Tile::GetGridPosition()
{
	return GridPosition;
}

Vector2 Tile::GetAtlasPixelPosition()
{
	return AtlasPixelPosition;
}

Rectangle Tile::GetTileRectangle()
{
	return Rectangle(WorldPosition.x, WorldPosition.y, (float)TILE_SIZE, (float)TILE_SIZE);
}
