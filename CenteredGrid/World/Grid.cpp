#include "Grid.hpp"

Grid::Grid(int WorldSize, int TileSize)
{
	this->Size = WorldSize;
	this->TileSize = TileSize;

	this->AmountOfTiles = ((this->Size * 2) * (this->Size * 2) / (this->TileSize * this->TileSize));
	this->Sparse.resize(this->AmountOfTiles);
	std::fill(this->Sparse.begin(), this->Sparse.end(), INVALID_INDEX);
}

void Grid::ToggleLineDisplay()
{
	this->DisplayLines = !this->DisplayLines;
}

void Grid::AddTileToCurrentStroke(size_t SparseIndex, int AtlasHeight, Vector2 GridPosition, Vector2 AtlasPosition)
{
	ActionData LastAction;
	LastAction.SparseIndex = SparseIndex;
	LastAction.GridPosition = GridPosition;
	LastAction.AtlasPosition = AtlasPosition;
	LastAction.AtlasHeight = AtlasHeight;
	LastAction.TileAddtion = true;
	this->StrokeActions.push_back(LastAction);
}

void Grid::AddTile(size_t SparseIndex, Vector2 GridPosition, Vector2 AtlasPosition, int AtlasHeight, bool PlayerAction)
{
	if (SparseIndex >= this->AmountOfTiles)
	{
		return;
	}
	size_t TileIndex = this->Sparse[SparseIndex];
	if (TileIndex == INVALID_INDEX)
	{
		this->Sparse[SparseIndex] = this->Tiles.size();
		this->Tiles.push_back(Tile(GridPosition, AtlasPosition, AtlasHeight));
	}
	else
	{
		Tile& PreviousTile = this->Tiles[TileIndex];
		if (!PreviousTile.IsSameAs(GridPosition, AtlasPosition))
		{
			PreviousTile.Set(GridPosition, AtlasPosition, AtlasHeight);
		}
	}
	if (PlayerAction)
	{
		this->AddTileToCurrentStroke(SparseIndex, AtlasHeight, GridPosition, AtlasPosition);
	}

}

void Grid::RemoveTile(size_t SparseIndex, int AtlasHeight, bool PlayerAction)
{
	if (SparseIndex > this->AmountOfTiles || this->Sparse[SparseIndex] == INVALID_INDEX)
	{
		return;
	}
	size_t TileIndex = this->Sparse[SparseIndex];
	size_t LastTileIndex = this->Tiles.size() - 1;
	if (PlayerAction)
	{
		this->AddTileToCurrentStroke(SparseIndex, AtlasHeight, this->Tiles[TileIndex].GetGridPosition(), this->Tiles[TileIndex].GetAtlasPixelPosition());
	}
	if (TileIndex == LastTileIndex)
	{
		this->Tiles.pop_back();
		this->Sparse[SparseIndex] = INVALID_INDEX;
	}
	else
	{
		Tile& LastTile = this->Tiles.back();
		Tile& DeletedTile = this->Tiles[TileIndex];
		LastTile.CutPasteTo(DeletedTile, AtlasHeight);
		this->Tiles.pop_back();
		size_t MovedTileIdx = GridToIndex(this->Tiles[TileIndex].GetGridPosition());
		this->Sparse[MovedTileIdx] = TileIndex;
		this->Sparse[SparseIndex] = INVALID_INDEX;
	}
}

void Grid::Draw(const Texture& Atlas, Rectangle& DisplayedRegion)
{
	for (Tile& Case : this->Tiles)
	{
		if (CheckCollisionRecs(DisplayedRegion, Case.GetTileRectangle()))
		{
			Case.Draw(Atlas);
		}
	}
	if (DisplayLines)
	{
		this->DrawLines();
	}
}

void Grid::DrawLines() const
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

void Grid::RegisterStroke()
{

	this->PastStrokes.push_back(StrokeActions);
	this->StrokeActions.clear();
	this->StrokeActions.shrink_to_fit();
}

void Grid::ReverseLastStroke()
{
	if (this->PastStrokes.size() == 0)
	{
		return;
	}
	std::vector<ActionData> LastStroke = this->PastStrokes.back();
	this->PastStrokes.pop_back();
	for (ActionData& Action : LastStroke)
	{
		if (Action.TileAddtion)
		{
			this->RemoveTile(Action.SparseIndex, Action.AtlasHeight, false);
		}
		else
		{
			this->AddTile(Action.SparseIndex, Action.GridPosition, Action.AtlasPosition, Action.AtlasHeight, false);
		}
	}
	this->ReversedStrokes.push_back(LastStroke);
}

void Grid::RedoLastStroke()
{
	if (this->ReversedStrokes.size() == 0)
	{
		return;
	}
	std::vector<ActionData> LastStroke = this->ReversedStrokes.back();
	this->ReversedStrokes.pop_back();
	for (ActionData& Action : LastStroke)
	{
		if (Action.TileAddtion)
		{
			this->AddTile(Action.SparseIndex, Action.GridPosition, Action.AtlasPosition, Action.AtlasHeight, false);
		}
		else
		{
			this->RemoveTile(Action.SparseIndex, Action.AtlasHeight, false);
		}
	}
	this->PastStrokes.push_back(LastStroke);
}

void Grid::Save(int AtlasHeight) const
{
	std::string PosXField = "PosX";
	std::string PosYField = "PosY";
	std::string AtlasPosXField = "AtlasX";
	std::string AtlasPosYField = "AtlasY";
	std::string Indentation = "    ";

	std::string Content = "{\n";

	for (int i = 0; i < this->Tiles.size(); i++)
	{
		Vector2 TileGridPosition = this->Tiles[i].GetGridPosition();
		Vector2 WorldPosition = Vector2AddValue(GridToWorld(TileGridPosition), SAVING_OFFSET);
		Vector2 AtlasPosition = this->Tiles[i].GetAtlasPixelPosition();
		Vector2 ImageSpacePosition = SwitchTextureSpace(AtlasPosition, AtlasHeight);
		Vector2 Dimensions = Vector2((float)TILE_SIZE, (float)TILE_SIZE);
		std::string TileId = std::to_string(GridToIndex(TileGridPosition));

		Content += Indentation + '"' + "Tile" + TileId + '"' + " :\n";
		Content += Indentation + "{\n";
		Content += Indentation + Indentation + '"' + PosXField + '"' + " : " + std::to_string((int)WorldPosition.x) + ",\n";
		Content += Indentation + Indentation + '"' + PosYField + '"' + " : " + std::to_string((int)WorldPosition.y) + ",\n";
		Content += Indentation + Indentation + '"' + AtlasPosXField + '"' + " : " + std::to_string((int)ImageSpacePosition.x) + ",\n";
		Content += Indentation + Indentation + '"' + AtlasPosYField + '"' + " : " + std::to_string((int)ImageSpacePosition.y) + "\n";
		if (i == this->Tiles.size() - 1)
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
	std::fill(this->Sparse.begin(), this->Sparse.end(), INVALID_INDEX);
	this->Tiles.clear();
	this->Tiles.shrink_to_fit();

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
		
		this->AddTile(SparseIndex, GridPosition, AtlasPosition, AtlasHeight);
	}
	return 0;
}