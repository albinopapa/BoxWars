#include "UIButton.h"
#include "Constants.h"

Button::Button( const char* _name )noexcept
	:
	name( _name ),
	hilighted( false )
{}

void Button::Draw( const Vec2i& pos, const Font& font, D3DGraphics& gfx )const noexcept
{
	if( hilighted )
		gfx.DrawRectangle( menuBox + pos, color );
	else
		gfx.DrawEmptyRectangle( menuBox + pos, color );

	gfx.DrawString( name, pos.x, pos.y, font, color );
}
