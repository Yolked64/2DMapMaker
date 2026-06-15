#include "App.hpp"

App::App(const std::string& AtlasFilePath)
{
	this->UiPosition.x = UiSreenPaddingX * SCREEN_WIDTH;
	this->UiPosition.y = 0.0f;
	this->UiPosition.width = SCREEN_WIDTH - UiPosition.x;
	this->UiPosition.height = SCREEN_HEIGHT;

	this->GridMap = std::make_unique<Grid>(WORLD_SIZE, TILE_SIZE);
	this->MyCamera = std::make_unique<WorldDisplayer>();

	Vector2 AtlasPosition;
	AtlasPosition.x = this->UiSreenPaddingX * SCREEN_WIDTH;
	AtlasPosition.y = 0;
	this->TextureManager = std::make_unique<Atlas>(AtlasFilePath, AtlasPosition);

	Rectangle SaveButtonPosition;
	SaveButtonPosition.x = this->UiSreenPaddingX * SCREEN_WIDTH;
	SaveButtonPosition.y = AtlasPosition.y + this->TextureManager->GetTextureHeight() + 0.02f * SCREEN_HEIGHT;
	SaveButtonPosition.height = 64;
	SaveButtonPosition.width = (1 - this->UiSreenPaddingX) * SCREEN_WIDTH / 2;
	this->SaveButton = std::make_unique<Button>(SaveButtonPosition, "Save", GREEN);

	Rectangle TextureOpenerPosition;
	TextureOpenerPosition.x = this->UiSreenPaddingX * SCREEN_WIDTH;
	TextureOpenerPosition.y = SaveButtonPosition.y + SaveButtonPosition.height + 0.02f * SCREEN_HEIGHT;
	TextureOpenerPosition.height = 64;
	TextureOpenerPosition.width = (1 - this->UiSreenPaddingX) * SCREEN_WIDTH / 2;
	this->TextureOpener = std::make_unique<InputBox>(TextureOpenerPosition, "Load texture :", GREEN);

	Rectangle TilemapLoaderPosition;
	TilemapLoaderPosition.x = this->UiSreenPaddingX * SCREEN_WIDTH;
	TilemapLoaderPosition.y = TextureOpenerPosition.y + TextureOpenerPosition.height + 0.02f * SCREEN_HEIGHT;
	TilemapLoaderPosition.height = 64;
	TilemapLoaderPosition.width = (1 - this->UiSreenPaddingX) * SCREEN_WIDTH / 2;
	this->TilemapLoader = std::make_unique<InputBox>(TilemapLoaderPosition, "Open tilemap :", GREEN);

	this->MouseCoordinatesPosition.x = this->UiSreenPaddingX * SCREEN_WIDTH;
	this->MouseCoordinatesPosition.y = 0.95f * SCREEN_HEIGHT;
}

void App::Update()
{
	Vector2 MousePosition = GetMousePosition();
	this->InsideUi = CheckCollisionPointRec(MousePosition, this->UiPosition);
	this->InsideAtlas = this->TextureManager->IsHoverred(MousePosition);
	this->InsideTextureOpener = this->TextureOpener->IsMouseHovering(MousePosition);
	this->InsideTilemapLoader = this->TilemapLoader->IsMouseHovering(MousePosition);
	this->InsideSave = this->SaveButton->IsMouseHovering(MousePosition);

	Vector2 ViewUperLeftCorner = GetScreenToWorld2D(Vector2(0 - VIEW_OFFSET, 0 - VIEW_OFFSET), this->MyCamera->GetCamera());
	Vector2 ViewBottomRightCorner = GetScreenToWorld2D(Vector2(SCREEN_WIDTH + VIEW_OFFSET, SCREEN_HEIGHT + VIEW_OFFSET), this->MyCamera->GetCamera());

	float RegionWidth = ViewBottomRightCorner.x - ViewUperLeftCorner.x;
	float RegionHeight = ViewBottomRightCorner.y - ViewUperLeftCorner.y;

	this->WorldRegionDisplayed = Rectangle(ViewUperLeftCorner.x, ViewUperLeftCorner.y, RegionWidth, RegionWidth);

	if (this->InsideTextureOpener)
	{
		this->TextureOpener->Update();
	}
	if (this->InsideTilemapLoader)
	{
		this->TilemapLoader->Update();
	}
}

void App::Draw()
{
	const Texture& TextureUsed = this->TextureManager->GetTexture();
	this->MyCamera->UseCamera();
	this->GridMap->Draw(TextureUsed, this->WorldRegionDisplayed);
	this->MyCamera->QuitCamera();
	this->DisplayUiElements();
}

void App::DisplayUiElements() const
{
	DrawRectangleRec(this->UiPosition, this->UiColor);
	this->TextureManager->Draw();
	this->SaveButton->Draw();
	this->TextureOpener->Draw();
	this->TilemapLoader->Draw();
	DisplayMouseWorldCoordinates();
}

void App::DisplayMouseWorldCoordinates() const
{
	Vector2 MousePosition = GetMousePosition();
	Vector2 WorldMousePosition = Vector2AddValue(GetScreenToWorld2D(MousePosition, this->MyCamera->GetCamera()), SAVING_OFFSET);
	std::string DisplayedText = "(" + std::to_string((int)WorldMousePosition.x) + ", " + std::to_string((int)WorldMousePosition.y) + ")";
	DrawText(DisplayedText.c_str(), (int)MouseCoordinatesPosition.x, (int)MouseCoordinatesPosition.y, this->MouseTextSize, this->MouseTextColor);
}

void App::HandleInputs()
{
	Vector2 MousePosition = GetMousePosition();
	Vector2 MouseWorldPosition = GetScreenToWorld2D(MousePosition, this->MyCamera->GetCamera());
	int CurrentTextureHeight = this->TextureManager->GetTextureHeight();
	float DeltaT = GetFrameTime();

	bool LeftMouseButtonPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
	bool CtrlPressed = IsKeyPressed(KEY_LEFT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL);
	bool BackSpacePressed = IsKeyPressed(KEY_BACKSPACE);
	bool EnterPressed = IsKeyPressed(KEY_ENTER);
	float MouseScrollAmount = GetMouseWheelMove();

	if (this->InsideUi)
	{
		if (this->InsideAtlas && LeftMouseButtonPressed)
		{
			this->TextureManager->RegisterTilePressed(MousePosition);
			Vector2 AtlasTileUnderUse = this->TextureManager->GetAtlasTilePressed();
			this->AtlasPixelUnderUse = TileAtlasToPixelImage(AtlasTileUnderUse, CurrentTextureHeight);
		}
		if (this->InsideSave && LeftMouseButtonPressed)
		{
			this->GridMap->Save(CurrentTextureHeight);
		}
		if (this->InsideTextureOpener)
		{
			if (BackSpacePressed)
			{
				this->TextureOpener->DeleteLastCharacter();
			}
			else if (EnterPressed)
			{
				const std::string& UserInput = this->TextureOpener->GetInput();
				int Success = this->TextureManager->LoadNewAtlas(UserInput);
				if (Success == -1)
				{
					std::cout << "ERROR::TEXTURE LOADING::COULDN'T OPEN TEXTURE " << UserInput << std::endl;
				}
				this->TextureOpener->ResetContentAndDisplayWelcome();
				this->InsideTextureOpener = false;
			}
			else
			{
				int AscciCode = GetCharPressed();
				if (AscciCode != 0)
				{
					this->TextureOpener->AddCharacter(AscciCode);
				}
			}
		}
		if (this->InsideTilemapLoader)
		{
			if (BackSpacePressed)
			{
				this->TilemapLoader->DeleteLastCharacter();
			}
			else if (EnterPressed)
			{
				const std::string& UserInput = this->TilemapLoader->GetInput();
				int Success = this->GridMap->OpenTileMap(UserInput, CurrentTextureHeight);
				if (Success == -1)
				{
					std::cout << "ERROR::TILEMAP LOADING:: FAILED LOADING " << UserInput << std::endl;
				}
				this->TilemapLoader->ResetContentAndDisplayWelcome();
				this->InsideTilemapLoader = false;
			}
			else
			{
				int AscciCode = GetCharPressed();
				if (AscciCode != 0)
				{
					this->TilemapLoader->AddCharacter(AscciCode);
				}
			}
		}
	}
	else
	{
		if (IsKeyPressed(KEY_F))
		{
			this->GridMap->ToggleLineDisplay();
		}
		if (LeftMouseButtonPressed || IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			Vector2 ClickedCase = WorldToGrid(MouseWorldPosition);
			if (ClickedCase.x >= 0 && ClickedCase.y >= 0 && ClickedCase.x <= GRID_WORLD_SIZE && ClickedCase.y <= GRID_WORLD_SIZE)
			{
				size_t SparseIndex = GridToIndex(ClickedCase);
				this->GridMap->AddTile(SparseIndex, ClickedCase, this->AtlasPixelUnderUse, CurrentTextureHeight);
			}
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			this->GridMap->RegisterStroke();
		}
		if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
		{
			Vector2 ClickedCase = WorldToGrid(MouseWorldPosition);
			size_t SparseIndex = GridToIndex(ClickedCase);
			this->GridMap->RemoveTile(SparseIndex, CurrentTextureHeight);
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
		{
			this->GridMap->RegisterStroke();
		}
		if (CtrlPressed && IsKeyPressed(KEY_W))
		{
			this->GridMap->ReverseLastStroke();
		}
		if (CtrlPressed && IsKeyPressed(KEY_Y))
		{
			this->GridMap->RedoLastStroke();
		}
		if (MouseScrollAmount > 0)
		{
			this->MyCamera->MultiplyZoom();
		}
		else if (MouseScrollAmount < 0)
		{
			this->MyCamera->DivideZoom();
		}
		if ((IsKeyPressed(KEY_W) || IsKeyDown(KEY_W)) && !CtrlPressed)
		{
			this->MyCamera->Move(0, -1, DeltaT);
		}
		if ((IsKeyPressed(KEY_A) || IsKeyDown(KEY_A)) && !CtrlPressed)
		{
			this->MyCamera->Move(-1, 0, DeltaT);
		}
		if ((IsKeyPressed(KEY_S) || IsKeyDown(KEY_S)) && !CtrlPressed)
		{
			this->MyCamera->Move(0, 1, DeltaT);
		}
		if ((IsKeyPressed(KEY_D) || IsKeyDown(KEY_D)) && !CtrlPressed)
		{
			this->MyCamera->Move(1, 0, DeltaT);
		}
	}
}