#include "Button.h"

Button::Button( const char * _name, int _x, int _y ) noexcept
	:
	name( _name ),
	x( _x ), y( _y )
{}

bool Button::IsActive() const noexcept
{
	return isActive;
}

void Button::SetActive() noexcept
{
	isActive = true;
	bgColor = activeColor;
	fontColor = activeFont;
	borderColor = activeBorder;
}

void Button::SetInactive() noexcept
{
	isActive = false;
	bgColor = inactiveColor;
	fontColor = inactiveFont;
	borderColor = inactiveBorder;
}

void Button::Draw( const Font& _font, D3DGraphics & _graphics )const noexcept
{
	_graphics.DrawEmptyRectangle( { x, y, width, height }, borderColor );
	_graphics.DrawRectangle( { x + 1, y + 1, width - 1, height - 1 }, bgColor );
	_graphics.DrawString( name.c_str(), x + 20, y - 10, _font, fontColor );
}
