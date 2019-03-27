#include "Game.h"


Game::Game( Window& _window )
	:
	gfx( _window ),
	audio( _window ),
	kbd( _window ),
	mouse( _window ),
	state( *this ),
	win( _window )
{}

void Game::Go()noexcept
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()noexcept
{
	while( kbd.PeekEvent() )
	{
		const auto keyevent = kbd.ReadEvent();
		state.HandleKeyEvent( *keyevent );
	}
#if defined(DEBUG) || defined(_DEBUG)
	state.Update( .016f );
#else
	state.Update( timer.Mark() );
#endif
}

void Game::ComposeFrame()noexcept
{
	state.Draw( gfx );
}
