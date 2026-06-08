#pragma once

#include "../Data.hpp"
#include "../World/Grid.hpp"
#include "../World/Viewer.hpp"
#include "Atlas.hpp"
#include "Button.hpp"
#include "InputBox.hpp"

#include <unordered_map>

enum INPUT
{
	LEFT_CLICKED,
	RIGHT_CLICKED,
	DELETE_CHAR,
	SEND_STRING,
	TOGGLE_LINE_DISPLAY,
	GO_UP,
	GO_LEFT,
	GO_DOWN,
	GO_RIGHT,
	UNDO_ACTION,
	DO_ACTION
};

enum INPUT_TYPE
{
	PRESSED,
	DOWN,
	RELEASED,
	UNTOUCHED
};

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

		std::unordered_map<INPUT, INPUT_TYPE> Keys;

		float UiSreenPaddingX = 0.85f;
		Color UiColor = UI_COLOR;
		Rectangle UiPosition;

		Vector2 MouseCoordinatesPosition;
		int MouseTextSize = 24;
		Color MouseTextColor = ORANGE;

		Vector2 AtlasPixelUnderUse = Vector2(0, 0);

		std::unique_ptr<Grid> GridMap;
		std::unique_ptr<WorldDisplayer> MyCamera;
		std::unique_ptr<Atlas> TextureManager;
		std::unique_ptr<Button> SaveButton;
		std::unique_ptr<InputBox> TextureOpener;
		std::unique_ptr<InputBox> TilemapLoader;

		void DisplayUiElements();
		void DisplayMouseWorldCoordinates();
		void RegisterInputs();
};