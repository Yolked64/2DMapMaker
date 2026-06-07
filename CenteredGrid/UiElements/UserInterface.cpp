#include "UserInterface.hpp"

UserInterface::UserInterface(const std::string& AtlasFilePath)
{
	UiPosition.x = UiSreenPaddingX * SCREEN_WIDTH;
	UiPosition.y = 0.0f;
	UiPosition.width = SCREEN_WIDTH - UiPosition.x;
	UiPosition.height = SCREEN_HEIGHT;

	GridMap = std::make_unique<Grid>(WORLD_SIZE, TILE_SIZE);
	MyCamera = std::make_unique<WorldDisplayer>();

	Vector2 AtlasPosition;
	AtlasPosition.x = UiSreenPaddingX * SCREEN_WIDTH;
	AtlasPosition.y = 0;
	TextureManager = std::make_unique<Atlas>(AtlasFilePath, AtlasPosition);

	Rectangle SaveButtonPosition;
	SaveButtonPosition.x = UiSreenPaddingX * SCREEN_WIDTH;
	SaveButtonPosition.y = AtlasPosition.y + TextureManager->GetTextureHeight() + 0.02f * SCREEN_HEIGHT;
	SaveButtonPosition.height = 64;
	SaveButtonPosition.width = (1 - UiSreenPaddingX) * SCREEN_WIDTH / 2;
	SaveButton = std::make_unique<Button>(SaveButtonPosition, "Save");

	Rectangle TextureOpenerPosition;
	TextureOpenerPosition.x = UiSreenPaddingX * SCREEN_WIDTH;
	TextureOpenerPosition.y = SaveButtonPosition.y + SaveButtonPosition.height + 0.02f * SCREEN_HEIGHT;
	TextureOpenerPosition.height = 64;
	TextureOpenerPosition.width = (1 - UiSreenPaddingX) * SCREEN_WIDTH / 2;
	TextureOpener = std::make_unique<InputBox>(TextureOpenerPosition, "Load texture :");

	Rectangle TilemapLoaderPosition;
	TilemapLoaderPosition.x = UiSreenPaddingX * SCREEN_WIDTH;
	TilemapLoaderPosition.y = TextureOpenerPosition.y + TextureOpenerPosition.height + 0.02f * SCREEN_HEIGHT;
	TilemapLoaderPosition.height = 64;
	TilemapLoaderPosition.width = (1 - UiSreenPaddingX) * SCREEN_WIDTH / 2;
	TilemapLoader = std::make_unique<InputBox>(TilemapLoaderPosition, "Open tilemap :");

	MouseCoordinatesPosition.x = UiSreenPaddingX * SCREEN_WIDTH;
	MouseCoordinatesPosition.y = 0.95f * SCREEN_HEIGHT;
}

void UserInterface::Update()
{
	Vector2 MousePosition = GetMousePosition();
	InsideUi = CheckCollisionPointRec(MousePosition, UiPosition);
	InsideAtlas = TextureManager->IsHoverred(MousePosition);
	InsideTextureOpener = TextureOpener->IsHoverred(MousePosition);
	InsideTilemapLoader = TilemapLoader->IsHoverred(MousePosition);
	InsideSave = SaveButton->IsHoverred(MousePosition);

	if (InsideTextureOpener)
	{
		TextureOpener->Update();
	}
	if (InsideTilemapLoader)
	{
		TilemapLoader->Update();
	}
	
}

void UserInterface::Draw()
{
	const Texture& TextureUsed = TextureManager->GetTexture();
	MyCamera->UseCamera();
	GridMap->Draw(TextureUsed);
	MyCamera->QuitCamera();
	DisplayUiElements();
}

void UserInterface::DisplayUiElements()
{
	DrawRectangleRec(UiPosition, UiColor);
	TextureManager->Draw();
	SaveButton->Draw();
	TextureOpener->Draw();
	TilemapLoader->Draw();
	DisplayMouseWorldCoordinates();
}

void UserInterface::DisplayMouseWorldCoordinates()
{
	Vector2 MousePosition = GetMousePosition();
	Vector2 WorldMousePosition = Vector2AddValue(GetScreenToWorld2D(MousePosition, MyCamera->GetCamera()), SAVING_OFFSET);
	std::string DisplayedText = "(" + std::to_string((int)WorldMousePosition.x) + ", " + std::to_string((int)WorldMousePosition.y) + ")";
	DrawText(DisplayedText.c_str(), (int)MouseCoordinatesPosition.x, (int)MouseCoordinatesPosition.y, MouseTextSize, MouseTextColor);
}

void UserInterface::HandleInputs()
{
	Vector2 MousePosition = GetMousePosition();
	Vector2 MouseWorldPosition = GetScreenToWorld2D(MousePosition, MyCamera->GetCamera());
	float DeltaT = GetFrameTime();
	bool LeftMouseButtonPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
	int CurrentTextureHeight = TextureManager->GetTextureHeight();
	if (InsideUi)
	{
		if (InsideAtlas && LeftMouseButtonPressed)
		{
			TextureManager->RegisterTilePressed(MousePosition);
			Vector2 AtlasTileUnderUse = TextureManager->GetAtlasTilePressed();
			AtlasPixelUnderUse = TileAtlasToPixelImage(AtlasTileUnderUse, CurrentTextureHeight);
		}
		if (InsideSave && LeftMouseButtonPressed)
		{
			GridMap->Save(CurrentTextureHeight);
		}
		if (InsideTextureOpener)
		{
			if (IsKeyPressed(KEY_BACKSPACE))
			{
				TextureOpener->DeleteLastCharacter();
			}
			else if (IsKeyPressed(KEY_ENTER))
			{
				const std::string& UserInput = TextureOpener->GetInput();
				int Success = TextureManager->LoadNewAtlas(UserInput);
				if (Success == -1)
				{
					std::cout << "ERROR::TEXTURE LOADING::COULDN'T OPEN TEXTURE " << UserInput << std::endl;
				}
				TextureOpener->ResetContentAndDisplayWelcome();
				InsideTextureOpener = false;
			}
			else
			{
				int AscciCode = GetCharPressed();
				if (AscciCode != 0)
				{
					TextureOpener->AddCharacter(AscciCode);
				}
			}
		}
		if (InsideTilemapLoader)
		{
			if (IsKeyPressed(KEY_BACKSPACE))
			{
				TilemapLoader->DeleteLastCharacter();
			}
			else if (IsKeyPressed(KEY_ENTER))
			{
				const std::string& UserInput = TilemapLoader->GetInput();
				int Success = GridMap->OpenTileMap(UserInput, CurrentTextureHeight);
				if (Success == -1)
				{
					std::cout << "ERROR::TILEMAP LOADING:: FAILED LOADING " << UserInput << std::endl;
				}
				TilemapLoader->ResetContentAndDisplayWelcome();
				InsideTilemapLoader = false;
			}
			else
			{
				int AscciCode = GetCharPressed();
				if (AscciCode != 0)
				{
					TilemapLoader->AddCharacter(AscciCode);
				}
			}
		}
	}
	else
	{
		float MouseScrollAmount = GetMouseWheelMove();
		int CurrentTextureheight = TextureManager->GetTextureHeight();
		if (MouseScrollAmount > 0)
		{
			MyCamera->MultiplyZoom();
		}
		else if (MouseScrollAmount < 0)
		{
			MyCamera->DivideZoom();
		}
		if (IsKeyPressed(KEY_W) || IsKeyDown(KEY_W))
		{
			MyCamera->Move(0, -1, DeltaT);
		}
		if (IsKeyPressed(KEY_A) || IsKeyDown(KEY_A))
		{
			MyCamera->Move(-1, 0, DeltaT);
		}
		if (IsKeyPressed(KEY_S) || IsKeyDown(KEY_S))
		{
			MyCamera->Move(0, 1, DeltaT);
		}
		if (IsKeyPressed(KEY_D) || IsKeyDown(KEY_D))
		{
			MyCamera->Move(1, 0, DeltaT);
		}
		if (IsKeyPressed(KEY_F))
		{
			GridMap->ToggleLineDisplay();
		}
		if (LeftMouseButtonPressed)
		{
			GridMap->BeginStrokeMode();
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			Vector2 ClickedCase = WorldToGrid(MouseWorldPosition);
			if (ClickedCase.x >= 0 && ClickedCase.y >= 0 && ClickedCase.x <= GRID_WORLD_SIZE && ClickedCase.y <= GRID_WORLD_SIZE)
			{
				size_t SparseIndex = GridToIndex(ClickedCase);
				GridMap->AddTile(SparseIndex, ClickedCase, AtlasPixelUnderUse, CurrentTextureheight);
			}
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			GridMap->QuiteStrokeMode();
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
		{
			GridMap->BeginStrokeMode();
		}
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			Vector2 ClickedCase = WorldToGrid(MouseWorldPosition);
			size_t SparseIndex = GridToIndex(ClickedCase);
			GridMap->RemoveTile(SparseIndex, CurrentTextureheight);
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
		{
			GridMap->QuiteStrokeMode();
		}
		if ((IsKeyPressed(KEY_LEFT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL)) && IsKeyPressed(KEY_W))
		{
			GridMap->ReverseLastStroke();
		}
		if ((IsKeyPressed(KEY_LEFT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL)) && IsKeyPressed(KEY_Y))
		{
			GridMap->RedoLastStroke();
		}
	}
}