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

		void Draw(const Texture& Atlas);
		void AddTile(size_t SparseIndex, Vector2 GridPosition, Vector2 AtlasPosition, int AtlasHeight, bool PlayerAction = true);
		void RemoveTile(size_t SparseIndex, int AtlasHeight, bool PlayerAction = true);
		void ToggleLineDisplay();

		void ReverseLastStroke();
		void BeginStrokeMode();
		void QuiteStrokeMode();

		void Save(int AtlasHeight);
		int OpenTileMap(const std::string& FileName, int AtlasHeight);
	private:
		int Size;
		int TileSize;
		int AmountOfTiles;

		bool DisplayLines = false;
		bool StrokeMode = false;

		std::vector<size_t> Sparse;
		std::vector<Tile> Tiles;
		std::vector<std::vector<ActionData>> PastStrokes;
		std::vector<ActionData> StrokeActions;

		void DrawLines();
};