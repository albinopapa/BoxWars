#include "UITextBox.h"

void TextBox::Draw( const Vec2i& position, int width, int height, const Font& font, D3DGraphics& gfx )const noexcept
{
	const auto numRows = height / font.CharHeight();
	const auto numCols = width / font.CharWidth();
	const auto lastCol = numCols - 1;
	const auto lastRow = numRows - 1;

	auto ConvertRow = [ & ]( const int _row ) { return _row * font.CharHeight(); };
	auto ConvertCol = [ & ]( const int _col ) { return _col * font.CharWidth(); };

	// Draw top of box outline
	gfx.DrawTextBoxTopLeft( position.x + ConvertCol( 0 ), position.y + ConvertRow( 0 ), font, Colors::white );

	gfx.DrawString( name, position.x + ConvertCol( 1 ), position.y + ConvertRow( 0 ), font, Colors::white );

	for( int col = int( std::char_traits<char>::length( name ) ); col < lastCol; ++col )
	{
		const auto x = position.x + ConvertCol( col );
		const auto y = position.y + ConvertRow( 0 );
		DrawHorizontal( x, y, font, gfx );
	}
	gfx.DrawTextBoxTopRight( position.x + ConvertCol( lastCol ), position.y + ConvertRow( 0 ), font, Colors::white );

	// Draw sides of box outline
	for( int row = 1; row < lastRow; ++row )
	{
		const auto left = position.x + ConvertCol( 0 );
		const auto right = position.x + ConvertCol( lastCol );
		const auto y = position.y + ConvertRow( row );
		DrawVertical( left, y, font, gfx );
		DrawVertical( right, y, font, gfx );
	}

	// Draw bottom of box outline
	gfx.DrawTextBoxBottomLeft( position.x + ConvertCol( 0 ), position.y + ConvertRow( lastRow ), font, Colors::white );
	for( int col = 1; col < lastCol; ++col )
	{
		const auto x = position.x + ConvertCol( col );
		const auto y = position.y + ConvertRow( lastRow );
		DrawHorizontal( x, y, font, gfx );
	}
	gfx.DrawTextBoxBottomRight( position.x + ConvertCol( lastCol ), position.y + ConvertRow( lastRow ), font, Colors::white );

	// Draw text
	const int row = numRows / 2;
	const int col = lastCol - int( text.size() );
	gfx.DrawString( text.c_str(), position.x + ConvertCol( col ), position.y + ConvertRow( row ), font, Colors::white );

}
void TextBox::DrawHorizontal( std::int32_t _x, std::int32_t _y, const Font& _font, D3DGraphics& _gfx )const noexcept
{
	const auto charCenterX = ( _font.CharWidth() / 2 );
	const auto charCenterY = ( _font.CharHeight() / 2 );
	_gfx.DrawLine( _x, _y + charCenterY, _x + _font.CharWidth(), _y + charCenterY, color );
}
void TextBox::DrawVertical( std::int32_t _x, std::int32_t _y, const Font& _font, D3DGraphics& _gfx )const noexcept
{
	const auto charCenterX = ( _font.CharWidth() / 2 );
	const auto charCenterY = ( _font.CharHeight() / 2 );
	_gfx.DrawLine( _x + charCenterX, _y, _x + charCenterX, _y + _font.CharHeight(), color );
}

