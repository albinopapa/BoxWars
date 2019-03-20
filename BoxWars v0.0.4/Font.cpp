#include "Font.h"

Font::Font( const char * filename, int charWidth, int charHeight, int nCharsPerRow )
	:
	m_surface( filename, Color( 255, 255, 255 ) ),
	m_charWidth( charWidth ),
	m_charHeight( charHeight ),
	m_nCharsPerRow( nCharsPerRow )
{
}

const Color& Font::operator()( int _x, int _y )const noexcept
{
	return m_surface( _x, _y );
}

int Font::CharWidth() const noexcept
{
	return m_charWidth;
}

int Font::CharHeight() const noexcept
{
	return m_charHeight;
}

RectI Font::CharRect( char _value ) const noexcept
{
	const int sheetIndex = _value - ' ';
	const int sheetCol = sheetIndex % m_nCharsPerRow;
	const int sheetRow = sheetIndex / m_nCharsPerRow;
	
	return { sheetCol * m_charWidth, sheetRow * m_charHeight, m_charWidth, m_charHeight };
}
