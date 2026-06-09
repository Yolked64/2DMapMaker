#pragma once

#include <raylib.h>
#include <raymath.h>

#include <iostream>
#include <vector>
#include <memory>

// CHANGE THIS TO GET YOUR OWN UI

constexpr Color BACKGROUND = Color(12, 20, 28, 255);
constexpr Color GRID_COLOR = Color(128, 128, 0, 255);
constexpr Color UI_COLOR = Color(56, 56, 56, 255);
constexpr Color ATLAS_BACKGROUND = Color(20, 22, 24, 255);

// FEEL FREE TO CHANGE THIS TO ADAPT TO YOUR TEXTURES AND NEEDS

constexpr int TILE_SIZE = 16;
constexpr int ATLAS_TILE_SIZE = 16;
constexpr int WORLD_SIZE = 5000;
constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;

// MIGHT BE FUNNY TO MESS AROUND WITH

constexpr float ZOOM_FACTOR = 1.08f;
constexpr float MAX_ZOOM = 50.0f;
constexpr float MIN_ZOOM = 0.05f;
constexpr float MOVEMENT_SPEED = 50 * TILE_SIZE;

// CHANGE THIS AT YOUR OWN RISKS 

constexpr int INVALID_INDEX = std::numeric_limits<int>::max();
constexpr int FPS = 0;

// YOU SHOULDN'T CHANGE THIS 

constexpr int GRID_WORLD_SIZE = (2 * WORLD_SIZE) / TILE_SIZE;
constexpr int SAVING_OFFSET = TILE_SIZE / 2;
constexpr int VIEW_OFFSET = 2 * TILE_SIZE;

/*
Convert a grid position to world position.
GridPosition ranges from 0 to GRID_WORLD_SIZE.		(Tile units)
WorldPosition ranges from -WORLD_SIZE to WORLD_SIZE (Pixel units)
*/
inline Vector2 GridToWorld(Vector2 GridPosition)
{
	int PixelGridPositionX = (int)GridPosition.x * TILE_SIZE;
	int PixelGridPositionY = (int)GridPosition.y * TILE_SIZE;

	int PixelWorldPositionX = PixelGridPositionX - WORLD_SIZE;
	int PixelWorldPositionY = PixelGridPositionY - WORLD_SIZE;

	Vector2 PixelWorldPosition = Vector2((float)PixelWorldPositionX, (float)PixelWorldPositionY);
	return PixelWorldPosition;
}

/*
Convert a World Position to a grid Position
WorldPosition ranges from -WORLD_SIZE to WORLD_SIZE (Pixel units)
GridPosition ranges from 0 to GRID_WORLD_SIZE.		(Tile units)
*/
inline Vector2 WorldToGrid(Vector2 WorldPosition)
{
	Vector2 PixelGridPosition = Vector2AddValue(WorldPosition, WORLD_SIZE);
	Vector2 TileGridPosition = PixelGridPosition / TILE_SIZE;
	return TileGridPosition;
}

/*
Convert a Grid Position to an index in the sparse array of a grid.
GridPosition ranges from 0 to GRID_WORLD_SIZE.		(Tile units)
SparseIndex ranges from 0 to WORLD_SIZE * WORLD_SIZE * 4 / TILE_SIZE * TILE_SIZE (Index)
*/
inline size_t GridToIndex(Vector2 GridPosition)
{
	size_t SparseIndex = 0;
	SparseIndex += (int)GridPosition.x;
	SparseIndex += (int)GridPosition.y * GRID_WORLD_SIZE;
	return SparseIndex;
}

/*
Convert a Sparse Index to a position in the grid space.
SparseIndex ranges from 0 to WORLD_SIZE * WORLD_SIZE * 4 / TILE_SIZE * TILE_SIZE (Index)
GridPosition ranges from 0 to GRID_WORLD_SIZE.		(Tile units)
*/
inline Vector2 IndexToGrid(size_t SparseIndex)
{
	Vector2 GridPosition = Vector2(0, 0);
	GridPosition.x = (float)(SparseIndex % GRID_WORLD_SIZE);
	GridPosition.y = (float)(SparseIndex / GRID_WORLD_SIZE);
	return GridPosition;
}

/*
Convert a ScreenPosition to it's tile position inside the Atlas.
Only use it if the position is in the atlas already.
ScreenPosition ranges from 0 to SCREEN_WIDTH and 0 to SCREEN_HEIGHT (Pixel units)
TileAtlasSpace ranges from 0 to ATLAS_SIZE / ATLAS_TILE_SIZE (Atlas Tile units)
*/
inline Vector2 ScreenToTileAtlas(Vector2 ScreenPosition, Vector2 AtlasOrigin, int AtlasHeight)
{
	float PixelDeltaX = ScreenPosition.x - AtlasOrigin.x;
	int TileDeltaX = (int)(PixelDeltaX);

	float PixelDeltaY = ScreenPosition.y - AtlasOrigin.y;
	float NegatedPixelDeltaY = PixelDeltaY - AtlasHeight;
	float PositivePixelDeltaY = NegatedPixelDeltaY * -1;
	int TileDeltaY = (int)(PositivePixelDeltaY);

	Vector2 TileInAtlas;
	TileInAtlas.x = (float)(TileDeltaX / ATLAS_TILE_SIZE);
	TileInAtlas.y = (float)(TileDeltaY / ATLAS_TILE_SIZE);
	return TileInAtlas;
}

/*
Convert a Tile in Atlas To a pixel in Image space.
TileAtlasSpace ranges from 0 to ATLAS_SIZE / ATLAS_TILE_SIZE (Atlas Tile units)
Pixel in images space ranges from 0 to ATLAS_SIZE.
Needed since the image space origin is at the top left corner and atlas space origin is at the bottom left corner.
*/
inline Vector2 TileAtlasToPixelImage(Vector2 TileAtlasSpace, int AtlasHeight)
{
	int ImageSpaceCoordinateX = (int)(TileAtlasSpace.x * ATLAS_TILE_SIZE);

	int AtlasScreenSpacePixelCoordinateY = (int)(TileAtlasSpace.y * ATLAS_TILE_SIZE);
	int NegatedImageSpacePixelCoordinateY = AtlasScreenSpacePixelCoordinateY - (AtlasHeight - ATLAS_TILE_SIZE);
	int ImageSpaceCoordinateY = NegatedImageSpacePixelCoordinateY * -1;

	Vector2 ImageSpaceCoordinates = Vector2((float)ImageSpaceCoordinateX, (float)ImageSpaceCoordinateY);
	return ImageSpaceCoordinates;
}

/*
Switch a pixel coordinate from atlas space to image space or vice versa.
Both ranges from 0 to atlas width and height.
Image space origin is at bootom left corner, atlas space one is at top left corner.
*/
inline Vector2 SwitchTextureSpace(Vector2 PixelCoordinate, int AtlasHeight)
{
	int NewX = (int)PixelCoordinate.x;

	int ExtractedY = (int)PixelCoordinate.y;
	int ReversedY = ExtractedY - (AtlasHeight - ATLAS_TILE_SIZE);
	int NewY = ReversedY * -1;

	Vector2 NewCoordinates = Vector2((float)NewX, (float)NewY);
	return NewCoordinates;
}