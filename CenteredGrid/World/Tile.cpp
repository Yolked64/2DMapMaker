#include "Tile.hpp"

Tile::Tile(Vector2 GridPosition, Vector2 AtlasPixelPosition, int AtlasHeight)
{
	this->GridPosition = GridPosition;
	this->WorldPosition = GridToWorld(GridPosition);
	this->AtlasPixelPosition = AtlasPixelPosition;
	this->Initialized = true;
}

void Tile::Set(Vector2 GridPosition, Vector2 AtlasPixelPosition, int AtlasHeight)
{
	this->GridPosition = GridPosition;
	this->AtlasPixelPosition = AtlasPixelPosition;
	this->WorldPosition = GridToWorld(GridPosition);
	this->Initialized = true;
}

void Tile::Reset()
{
	this->GridPosition = Vector2((float)INVALID_INDEX, (float)INVALID_INDEX);
	this->WorldPosition = Vector2((float)INVALID_INDEX, (float)INVALID_INDEX);
	this->AtlasPixelPosition = Vector2((float)INVALID_INDEX, (float)INVALID_INDEX);
	this->Initialized = false;
}

void Tile::CutPasteTo(Tile& Case, int AtlasHeight)
{
	Case.Set(this->GridPosition, this->AtlasPixelPosition, AtlasHeight);
	this->Reset();
}

bool Tile::IsSameAs(Vector2 TileGridPosition, Vector2 PixelAtlasPosition) const
{
	if (!Vector2Equals(this->GridPosition, TileGridPosition))
	{
		return false;
	}
	if (!Vector2Equals(this->AtlasPixelPosition, PixelAtlasPosition))
	{
		return false;
	}
	return true;
}

void Tile::Draw(const Texture& Atlas) const
{
	if (Initialized)
	{
		Rectangle Source = Rectangle(this->AtlasPixelPosition.x, this->AtlasPixelPosition.y, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
		Rectangle Destination = Rectangle(this->WorldPosition.x, this->WorldPosition.y, TILE_SIZE, TILE_SIZE);
		DrawTexturePro(Atlas, Source, Destination, this->Origin, this->Rotation, this->Tint);
	}
}

Vector2 Tile::GetGridPosition() const
{
	return this->GridPosition;
}

Vector2 Tile::GetAtlasPixelPosition() const 
{
	return this->AtlasPixelPosition;
}

Rectangle Tile::GetTileRectangle() const
{
	return Rectangle(this->WorldPosition.x, this->WorldPosition.y, (float)TILE_SIZE, (float)TILE_SIZE);
}
