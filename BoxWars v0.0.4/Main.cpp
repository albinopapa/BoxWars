#include "Game.h"
#include "Window.h"

int WINAPI WinMain( HINSTANCE hInst,HINSTANCE,LPSTR,INT )
{
	try
	{
		Window win;
		Game theGame( win );

		while( !win.ProcessMessages() )
		{
			theGame.Go();
		}
	}
	catch( const std::exception& e)
	{
		MessageBox( nullptr, e.what(), "Exception thrown", MB_OK );
	}

    return 0;
}