#pragma once

#include "D3DGraphics.h"
#include "Keyboard.h"
#include "States.h"
#include "Mouse.h"
#include "Sound.h"
#include "Timer.h"
#include "Window.h"


class Game
{
public:
	Game( Window& _window );
	Game( const Game& ) = delete;
	Game( Game&& ) = default;

	Game& operator=( const Game& ) = delete;
	Game& operator=( Game&& ) = default;

	void Go()noexcept;
	KeyboardClient& GetKeyboard()noexcept { return kbd; }
	void Quit()const noexcept { win.Kill(); }
private:
	void UpdateModel()noexcept;
	void ComposeFrame()noexcept;
	

private:
	D3DGraphics gfx;
	KeyboardClient kbd;
	MouseClient mouse;
	DSound audio;
	FrameTimer timer;
	Window& win;
	State state;
};
