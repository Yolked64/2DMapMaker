#include "Grid.hpp"

Grid::Grid(int WorldSize, int TileSize)
{
	Size = WorldSize;
	this->TileSize = TileSize;

	AmountOfTiles = ((Size * 2) * (Size * 2) / (this->TileSize * this->TileSize));
	Sparse.resize(AmountOfTiles);
	std::fill(Sparse.begin(), Sparse.end(), INVALID_INDEX);
}

void Grid::ToggleLineDisplay()
{
	DisplayLines = !DisplayLines;
}

void Grid::AddTile(size_t SparseIndex, Vector2 GridPosition, Vector2 AtlasPosition, int AtlasHeight, bool PlayerAction)
{
	if (SparseIndex >= AmountOfTiles)
	{
		return;
	}
	size_t TileIndex = Sparse[SparseIndex];
	if (TileIndex == INVALID_INDEX)
	{
		Sparse[SparseIndex] = Tiles.size();
		Tiles.push_back(Tile(GridPosition, AtlasPosition, AtlasHeight));
	}
	else
	{
		Tile& PreviousTile = Tiles[TileIndex];
		if (!PreviousTile.IsSameAs(GridPosition, AtlasPosition))
		{
			PreviousTile.Set(GridPosition, AtlasPosition, AtlasHeight);
		}
	}
	if (PlayerAction)
	{
		ActionData LastAction;
		LastAction.SparseIndex = SparseIndex;
		LastAction.GridPosition = GridPosition;
		LastAction.AtlasPosition = AtlasPosition;
		LastAction.AtlasHeight = AtlasHeight;
		LastAction.TileAddtion = true;
		PastActions.push_back(LastAction);
	}

}

void Grid::RemoveTile(size_t SparseIndex, int AtlasHeight, bool PlayerAction)
{
	if (SparseIndex > AmountOfTiles || Sparse[SparseIndex] == INVALID_INDEX)
	{
		return;
	}
	size_t TileIndex = Sparse[SparseIndex];
	size_t LastTileIndex = Tiles.size() - 1;
	if (PlayerAction)
	{
		ActionData LastAction;
		LastAction.SparseIndex = SparseIndex;
		LastAction.GridPosition = Tiles[TileIndex].GetGridPosition();
		LastAction.AtlasPosition = Tiles[TileIndex].GetAtlasPixelPosition();
		LastAction.AtlasHeight = AtlasHeight;
		LastAction.TileAddtion = false;
		PastActions.push_back(LastAction);
	}
	if (TileIndex == LastTileIndex)
	{
		Tiles.pop_back();
		Sparse[SparseIndex] = INVALID_INDEX;
	}
	else
	{
		Tile& LastTile = Tiles.back();
		Tile& DeletedTile = Tiles[TileIndex];
		LastTile.CutPasteTo(DeletedTile, AtlasHeight);
		Tiles.pop_back();
		size_t MovedTileIdx = GridToIndex(Tiles[TileIndex].GetGridPosition());
		Sparse[MovedTileIdx] = TileIndex;
		Sparse[SparseIndex] = INVALID_INDEX;
	}
}

void Grid::Draw(const Texture& Atlas)
{
	for (Tile& Case : Tiles)
	{
		Case.Draw(Atlas);
	}
	if (DisplayLines)
	{
		DrawLines();
	}
}

void Grid::DrawLines()
{
	for (int Column = -Size; Column <= Size; Column += TileSize)
	{
		DrawLine(Column, -Size, Column, Size, GRID_COLOR);
	}
	for (int Row = -Size; Row <= Size; Row += TileSize)
	{
		DrawLine(-Size, Row, Size, Row, GRID_COLOR);
	}
}

void Grid::ReverseLastAction()
{
	if (PastActions.size() == 0)
	{
		return;
	}
	ActionData LastAction = PastActions.back();
	PastActions.pop_back();
	if (LastAction.TileAddtion)
	{
		RemoveTile(LastAction.SparseIndex, LastAction.AtlasHeight, false);
	}
	else
	{
		AddTile(LastAction.SparseIndex, LastAction.GridPosition, LastAction.AtlasPosition, LastAction.AtlasHeight, false);
	}
}

void Grid::Save(int AtlasHeight)
{
	std::string PosXField = "PosX";
	std::string PosYField = "PosY";
	std::string AtlasPosXField = "AtlasX";
	std::string AtlasPosYField = "AtlasY";
	std::string Indentation = "    ";

	std::string Content = "{\n";

	for (int i = 0; i < Tiles.size(); i++)
	{
		Vector2 TileGridPosition = Tiles[i].GetGridPosition();
		Vector2 WorldPosition = Vector2AddValue(GridToWorld(TileGridPosition), SAVING_OFFSET);
		Vector2 AtlasPosition = Tiles[i].GetAtlasPixelPosition();
		Vector2 ImageSpacePosition = SwitchTextureSpace(AtlasPosition, AtlasHeight);
		Vector2 Dimensions = Vector2((float)TILE_SIZE, (float)TILE_SIZE);
		std::string TileId = std::to_string(GridToIndex(TileGridPosition));

		Content += Indentation + '"' + "Tile" + TileId + '"' + " :\n";
		Content += Indentation + "{\n";
		Content += Indentation + Indentation + '"' + PosXField + '"' + " : " + std::to_string((int)WorldPosition.x) + ",\n";
		Content += Indentation + Indentation + '"' + PosYField + '"' + " : " + std::to_string((int)WorldPosition.y) + ",\n";
		Content += Indentation + Indentation + '"' + AtlasPosXField + '"' + " : " + std::to_string((int)ImageSpacePosition.x) + ",\n";
		Content += Indentation + Indentation + '"' + AtlasPosYField + '"' + " : " + std::to_string((int)ImageSpacePosition.y) + "\n";
		if (i == Tiles.size() - 1)
		{
			Content += Indentation + "}\n";
		}
		else
		{
			Content += Indentation + "},\n";
		}
	}
	Content += "}";

	std::ofstream JSONMap("Tilemaps/Map.json");
	JSONMap << Content;
	JSONMap.close();
}

int Grid::OpenTileMap(const std::string& FileName, int AtlasHeight)
{
	std::fill(Sparse.begin(), Sparse.end(), INVALID_INDEX);
	Tiles.clear();
	Tiles.shrink_to_fit();

	const std::string FilePath = "Tilemaps/" + FileName;
	std::fstream Data(FilePath);

	if (!Data.is_open())
	{
		std::cout << "ERROR::FILE NOT OPENED\n";
		return -1;
	}
	json ReadData;
	Data >> ReadData;

	Data.close();

	if (ReadData.empty())
	{
		return -1;
	}
	for (const auto& TileData : ReadData)
	{
		int WorldPosX = TileData.at("PosX") - SAVING_OFFSET;
		int WorldPosY = TileData.at("PosY") - SAVING_OFFSET;
		int ImagePosX = TileData.at("AtlasX");
		int ImagePosY = TileData.at("AtlasY");
		Vector2 ImagePosition = Vector2((float)ImagePosX, (float)ImagePosY);
		Vector2 AtlasPosition = SwitchTextureSpace(ImagePosition, AtlasHeight);
		Vector2 GridPosition = WorldToGrid(Vector2((float)WorldPosX, (float)WorldPosY));
		size_t SparseIndex = GridToIndex(GridPosition);
		
		AddTile(SparseIndex, GridPosition, AtlasPosition, AtlasHeight);
	}
	return 0;
}