#pragma once

#include <fstream>

#include "../Data.hpp"
#include "Tile.hpp"
#include "../json.hpp"

struct ActionData
{
	size_t SparseIndex;
	Vector2 GridPosition;
	Vector2 AtlasPosition;
	int AtlasHeight;

	bool TileAddtion;
};

using json = nlohmann::json;

class Grid
{
	public:
		Grid(int WorldSize, int TileSize);

		void Draw(const Texture& Atlas, Rectangle& DisplayedRegion);
		void AddTile(size_t SparseIndex, Vector2 GridPosition, Vector2 AtlasPosition, int AtlasHeight, bool PlayerAction = true);
		void RemoveTile(size_t SparseIndex, int AtlasHeight, bool PlayerAction = true);
		void ToggleLineDisplay();

		void ReverseLastStroke();
		void RedoLastStroke();
		void RegisterStroke();

		void Save(int AtlasHeight) const;
		int OpenTileMap(const std::string& FileName, int AtlasHeight);
	private:
		int Size;
		int TileSize;
		int AmountOfTiles;

		bool DisplayLines = false;

		std::vector<size_t> Sparse;
		std::vector<Tile> Tiles;
		std::vector<std::vector<ActionData>> PastStrokes;
		std::vector<std::vector<ActionData>> ReversedStrokes;
		std::vector<ActionData> StrokeActions;

		void DrawLines() const;
		void AddTileToCurrentStroke(size_t SparseIndex, int AtlasHeight, Vector2 GridPosition = Vector2Zero(), Vector2 AtlasPosition = Vector2Zero());
};