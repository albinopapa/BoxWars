#pragma once

#include "Button.h"
#include "D3DGraphics.h"
#include "Keyboard.h"
#include "LanguageStrings.h"

#include <array>

class StartMenu
{
	class InputVisitor
	{
	public:
		InputVisitor( StartMenu& _parent );
		void operator()( const KeyboardClient::KeyPress& _event )noexcept;
		void operator()( const KeyboardClient::KeyRelease& _event )noexcept;

	private:
		StartMenu& parent;
	};
public:
	StartMenu( Language _lang = Language::English )noexcept;

	void Update( const KeyboardClient::Event& _event )noexcept;
	void Draw( const Font& _font, D3DGraphics& _graphics )const noexcept;

private:
	std::array<Button, 7> buttons;
	int curser = 0;

};

