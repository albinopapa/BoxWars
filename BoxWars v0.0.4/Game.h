#pragma once

#include "D3DGraphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Sound.h"
#include "LanguageStrings.h"
#include "RandomNumber.h"

#include "LanguageMenu.h"
#include <variant>



class Game
{
	using Menu = std::variant<LanguageMenu>;

public:
	Game( HWND hWnd, KeyboardServer& kServer, const MouseServer& mServer );
	void Go();
private:
	void UpdateModel();
	void ComposeFrame();

private:
	//framework stuff
	D3DGraphics gfx;
	KeyboardClient kbd;
	MouseClient mouse;
	DSound audio;


};