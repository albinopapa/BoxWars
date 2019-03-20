//inclusions & definitions
#include "Game.h"
#include "Timer.h"

constexpr const char* titleString = "Welcome to [BoxWar], brought to you by Clodi";

Game::Game( HWND hWnd, KeyboardServer& kServer, const MouseServer& mServer )
	:
	gfx( hWnd ),
	audio( hWnd ),
	kbd( kServer ),
	mouse( mServer )
{
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	while( kbd.PeekEvent() )
	{

	}
}

void Game::ComposeFrame()
{
	constexpr auto x = ( TOTALSCREENWIDTH -200 ) / 2;
	constexpr auto y = ( TOTALSCREENHEIGHT -200 ) / 2;
	gfx.DrawRectangle( { x,y,200,200 }, Colors::red );
}

