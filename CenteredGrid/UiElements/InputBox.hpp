#pragma once

#include "../Data.hpp"
#include "UiItem.hpp"

class InputBox : public UiItem
{
	public:
		InputBox(Rectangle ScreenPosition, const std::string& WelcomeText, Color Background);

		void Draw() const override;
		void Update() override;

		void AddCharacter(int AsciiCode);
		void DeleteLastCharacter();
		void ResetContentAndDisplayWelcome();

		const std::string& GetInput() const;
	private:
		bool DisplayWelcomeText = true;

		std::string WelcomeText = "";
		std::string TypedText = "";

		int TextSize = 16;
		Vector2 TextOffset = Vector2(16, 16);
		Color TextColor = BLACK;

		void DrawContent() const override;
};