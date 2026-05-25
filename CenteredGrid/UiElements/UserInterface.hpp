#pragma once

#include "../Data.hpp"
#include "../World/Grid.hpp"
#include "../World/Viewer.hpp"
#include "Atlas.hpp"
#include "Button.hpp"
#include "InputBox.hpp"

class UserInterface
{
	public:
		UserInterface(const std::string& AtlasFilePath);

		void Update();
		void Draw();
		void HandleInputs();
	private:
		bool InsideUi = false;
		bool InsideAtlas = false;
		bool InsideTextureOpener = false;
		bool InsideTilemapLoader = false;
		bool InsideSave = false;

		float UiSreenPaddingX = 0.85f;
		Color UiColor = UI_COLOR;
		Rectangle UiPosition;

		Vector2 MouseCoordinatesPosition;
		int MouseTextSize = 24;
		Color MouseTextColor = ORANGE;

		Vector2 AtlasTileUnderUse = Vector2(0, 0);

		std::unique_ptr<Grid> GridMap;
		std::unique_ptr<WorldDisplayer> MyCamera;
		std::unique_ptr<Atlas> TextureManager;
		std::unique_ptr<Button> SaveButton;
		std::unique_ptr<InputBox> TextureOpener;
		std::unique_ptr<InputBox> TilemapLoader;

		void DisplayUiElements();
		void DisplayMouseWorldCoordinates();
};