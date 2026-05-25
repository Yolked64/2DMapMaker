#pragma once

#include "../Data.hpp"

class InputBox
{
	public:
		InputBox(Rectangle ScreenPosition, const std::string& WelcomeText);

		void Draw();
		void Update();

		void AddCharacter(int AsciiCode);
		void DeleteLastCharacter();
		void ResetContentAndDisplayWelcome();

		bool IsHoverred(Vector2 MousePosition);
		const std::string& GetInput();
	private:
		bool DisplayWelcomeText = true;

		std::string WelcomeText = "";
		std::string TypedText = "";

		Rectangle ScreenPosition;
		Color Background = GREEN;

		int TextSize = 16;
		Vector2 TextOffset = Vector2(16, 16);
		Color TextColor = BLACK;

		void DrawContent();
};