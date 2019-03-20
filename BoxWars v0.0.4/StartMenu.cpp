#include "StartMenu.h"

StartMenu::InputVisitor::InputVisitor( StartMenu& _parent )
	:
	parent( _parent )
{}
void StartMenu::InputVisitor::operator()( const KeyboardClient::KeyPress& _event )noexcept
{
	if( _event.code == VK_UP )
	{

	}
	else if( _event.code == VK_DOWN )
	{

	}
	else if( _event.code == VK_RETURN )
	{

	}
}
void StartMenu::InputVisitor::operator()( const KeyboardClient::KeyRelease& _event )noexcept
{

}


StartMenu::StartMenu( Language _lang )noexcept
{
	buttons[ 0 ] = Button( NewGame::GetString( _lang ), 120, 40 );
	buttons[ 0 ] = Button( LoadGame::GetString( _lang ), 120, 40 );
	buttons[ 0 ] = Button( NewGame::GetString( _lang ), 120, 40 );
	buttons[ 0 ] = Button( NewGame::GetString( _lang ), 120, 40 );
	buttons[ 0 ] = Button( NewGame::GetString( _lang ), 120, 40 );
	buttons[ 0 ] = Button( NewGame::GetString( _lang ), 120, 40 );
}

void StartMenu::Update( const KeyboardClient::Event & _event ) noexcept
{
}

void StartMenu::Draw( const Font & _font, D3DGraphics & _graphics ) const noexcept
{
}

