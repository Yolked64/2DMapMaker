#include "Atlas.hpp"

Atlas::Atlas(const std::string& AtlasFileName, Vector2 Position)
{
	this->Area.x = Position.x;
	this->Area.y = Position.y;
	int Success = LoadNewAtlas(AtlasFileName);
	if (Success == -1)
	{
		std::cout << "ERROR::TEXTURE LOADING::FAILED " << AtlasFileName << std::endl;
	}
}

bool Atlas::IsHoverred(Vector2 MousePosition) const
{;
	if (CheckCollisionPointRec(MousePosition, this->Area))
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
	UnloadTexture(this->CurrentTexture);				// Beware of this thingy cuz when the object is constructed it unload an undefined data
	this->CurrentTexture = NewTexture;
	this->Area.width = (float)this->CurrentTexture.width;
	this->Area.height = (float)this->CurrentTexture.height;
	return 0;
}

void Atlas::Draw() const
{
	DrawRectangleRec(this->Area, this->Background);
	Rectangle Source = Rectangle(0, 0, (float)this->CurrentTexture.width, (float)this->CurrentTexture.height);
	DrawTexturePro(this->CurrentTexture, Source, this->Area, this->Origin, this->Rotation, this->Tint);
}

void Atlas::RegisterTilePressed(Vector2 ScreenMousePosition)
{
	Vector2 AtlasOrigin = Vector2(this->Area.x, this->Area.y);
	int AtlasHeight = this->CurrentTexture.height;
	this->LastAtlasTilePressed = ScreenToTileAtlas(ScreenMousePosition, AtlasOrigin, AtlasHeight);
}

Vector2 Atlas::GetAtlasTilePressed() const
{
	return this->LastAtlasTilePressed;
}

const Texture Atlas::GetTexture() const
{
	return this->CurrentTexture;
}

int Atlas::GetTextureHeight() const
{
	return this->CurrentTexture.height;
}