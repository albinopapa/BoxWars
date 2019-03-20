#pragma once

#include "Rect.h"
#include "Sprite.h"

class Font
{
public:
	using value_type = Color;
public:
	Font( const char* filename, int charWidth, int charHeight, int nCharsPerRow );
	const Color& operator()( int _x, int _y )const noexcept;
	int CharWidth()const noexcept;
	int CharHeight()const noexcept;
	RectI CharRect( char _value )const noexcept;
private:
	int m_charWidth = 0;
	int m_charHeight = 0;
	int m_nCharsPerRow = 0;
	Sprite m_surface;
};

