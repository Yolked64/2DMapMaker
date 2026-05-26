#include "Atlas.hpp"

Atlas::Atlas(const std::string& AtlasFileName, Vector2 Position)
{
	Area.x = Position.x;
	Area.y = Position.y;
	int Success = LoadNewAtlas(AtlasFileName);
	if (Success == -1)
	{
		std::cout << "ERROR::TEXTURE LOADING::FAILED " << AtlasFileName << std::endl;
	}
}

bool Atlas::IsHoverred(Vector2 MousePosition)
{;
	if (CheckCollisionPointRec(MousePosition, Area))
	{
		return true;
	}
	return false;
}

int Atlas::LoadNewAtlas(const std::string& FileName)
{
	std::string FilePath = "Textures/" + FileName;
	Texture NewTexture = LoadTexture(FilePath.c_str());
	if (NewTexture.id <= 0)
	{
		return -1;
	}
	UnloadTexture(CurrentTexture);				// Beware of this thingy cuz when the object is constructed it unload an undefined data
	CurrentTexture = NewTexture;
	Area.width = (float)CurrentTexture.width;
	Area.height = (float)CurrentTexture.height;
	return 0;
}

void Atlas::Draw()
{
	DrawRectangleRec(Area, Background);
	Rectangle Source = Rectangle(0, 0, (float)CurrentTexture.width, (float)CurrentTexture.height);
	DrawTexturePro(CurrentTexture, Source, Area, Origin, Rotation, Tint);
}

void Atlas::RegisterTilePressed(Vector2 ScreenMousePosition)
{
	Vector2 AtlasOrigin = Vector2(Area.x, Area.y);
	int AtlasHeight = CurrentTexture.height;
	LastAtlasTilePressed = ScreenToTileAtlas(ScreenMousePosition, AtlasOrigin, AtlasHeight);
}

Vector2 Atlas::GetAtlasTilePressed()
{
	return LastAtlasTilePressed;
}

const Texture& Atlas::GetTexture()
{
	return CurrentTexture;
}

int Atlas::GetTextureHeight()
{
	return CurrentTexture.height;
}