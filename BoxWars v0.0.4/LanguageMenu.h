#pragma once

#include "Button.h"
#include "D3DGraphics.h"
#include "Keyboard.h"
#include "LanguageStrings.h"

#include <array>

class LanguageMenu
{
	class InputVisitor
	{
	public:
		InputVisitor( LanguageMenu& _parent );
		void operator()( const KeyboardClient::KeyPress& _event )noexcept;
		void operator()( const KeyboardClient::KeyRelease& _event )noexcept;

	private:
		LanguageMenu& parent;
	};
public:
	LanguageMenu()noexcept;

	void Update( const KeyboardClient::Event& _event )noexcept;
	void Draw( const Font& _font, D3DGraphics& _graphics )const noexcept;
	Language ChosenLanguage()const noexcept;

private:
	std::array<Button, 6> buttons;
	Language language;
	int curser = 0;
};

