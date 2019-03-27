#include "ChiliMath.h"
#include "Constants.h"
#include "D3DGraphics.h"
#include "Window.h"
#include <cassert>
#pragma comment(lib, "d3d9.lib")

D3DGraphics::D3DGraphics( HWND hWnd )
	:
	pDirect3D( Direct3DCreate9( D3D_SDK_VERSION ) ),
	surface( ScreenWidth, ScreenHeight )
{
	assert( pDirect3D != nullptr );

	D3DPRESENT_PARAMETERS d3dpp{};
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	auto result = pDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, &d3dpp, &pDevice );
	assert( !FAILED( result ) );

	result = pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
	assert( !FAILED( result ) );
}

void D3DGraphics::PutPixel( int x, int y, Color color )noexcept
{
	assert( x >= 0 );
	assert( y >= 0 );
	assert( x < ScreenWidth );
	assert( y < ScreenHeight );

	surface( x, y ) = color;
}
void D3DGraphics::BeginFrame()noexcept
{
	surface.Clear();
}
void D3DGraphics::EndFrame()
{
	{
		D3D9RenderTarget target( pBackBuffer.Get() );
		copy2d( RectI{ 0, 0, ScreenWidth, ScreenHeight }, surface, target );
	}

	const auto result = pDevice->Present( nullptr, nullptr, nullptr, nullptr );
	assert( !FAILED( result ) );
}

void D3DGraphics::DrawBezier( const Point & _p0, const Point & _p1, const Point & _p2, Color _color ) noexcept
{
	constexpr auto step = .1f;

	const auto range0 = ( _p1 - _p0 );
	const auto range1 = ( _p2 - _p1 );
	const auto range2 = ( range1 - range0 );
	const auto doubleRange0 = ( range0 * 2 );

	constexpr auto end = ( 1.f + std::numeric_limits<float>::epsilon() );
	auto p0 = _p0;
	for( float t = step; t <= end; t += step )
	{
		const auto p1 = _p0 + ( doubleRange0 + ( range2 * t ) ) * t;

		DrawLine( p0.x, p0.y, p1.x, p1.y, _color );
		p0 = p1;
	}
}

void D3DGraphics::DrawSprite( int xoff, int yoff, const Sprite& sprite )noexcept
{	
	auto notIsAlpha = [ & ]( const Color& _color ) { return !sprite.IsKey( _color ); };

	const auto srcRect = RectI{ 0,0,int( sprite.Width() ),int( sprite.Height() ) };
	const auto dstRect = RectI{ xoff,yoff,int( sprite.Width() ),int( sprite.Height() ) };

	copy2d_if( srcRect, dstRect, sprite, surface, notIsAlpha );
}
void D3DGraphics::DrawChar( char c, int xoff, int yoff, const Font& font, Color color )noexcept
{
	const auto srcRect = font.CharRect( c );
	const auto dstRect = RectI{ 
		xoff,
		yoff,
		srcRect.width,
		srcRect.height 
	};

	replace2d( Colors::black, color, srcRect, dstRect, font, surface );
}
void D3DGraphics::DrawString( const char* string, int xoff, int yoff, const Font& font, Color color )noexcept
{
	for( int index = 0; string[ index ] != '\0'; index++ )
	{
		DrawChar( string[ index ], xoff + index * font.CharWidth(), yoff, font, color );
	}
}
void D3DGraphics::DrawDisc( int cx, int cy, int r, Color color )noexcept
{
	auto Square = []( auto _value ) {return _value * _value; };

	const auto rSq = Square( r );
	const auto dstRect = RectI{ cx - r,cy - r,r * 2,r * 2 };
	int index = 0;

	auto generate_disc = [ & ]()
	{
		const auto y = ( index / dstRect.width ) - r;
		const auto x = ( index % dstRect.width ) - r;

		++index;

		const auto distSq = Square( x ) + Square( y );

		return ( distSq < rSq ) ? color : surface( x + cx, y + cy );
	};

	generate2d( dstRect, surface, generate_disc );
}
void D3DGraphics::DrawLine( int x1, int y1, int x2, int y2, Color color )noexcept
{
	const int dx = x2 - x1;
	const int dy = y2 - y1;

	if( dy == 0 && dx == 0 )
	{
		PutPixel( x1, y1, color );
	}
	else if( abs( dy ) > abs( dx ) )
	{
		if( dy < 0 )
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		const float m = ( float )dx / ( float )dy;
		const float b = x1 - m * y1;
		for( int y = y1; y <= y2; y = y + 1 )
		{
			const int x = ( int )( m*y + b + 0.5f );
			PutPixel( x, y, color );
		}
	}
	else
	{
		if( dx < 0 )
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		const float m = ( float )dy / ( float )dx;
		const float b = y1 - m * x1;
		for( int x = x1; x <= x2; x = x + 1 )
		{
			const int y = ( int )( m*x + b + 0.5f );
			PutPixel( x, y, color );
		}
	}
}
void D3DGraphics::DrawCircle( int cx, int cy, int radius, Color color )noexcept
{
	if( radius == 0 )return;

	const auto iRadSq = sq( radius - 1 );
	const auto oRadSq = sq( radius );
	const auto bounds =
		ClipToScreen( cx - radius, cy - radius, radius * 2, radius * 2 ) + Vec2i( cx, cy );

	int i = 0;
	for_each2d( bounds, surface, [ & ]( Color& dest )
	{
		const int x = ( i % bounds.width ) - radius;
		const int y = ( i / bounds.width ) - radius;
		const auto distSq = sq( x ) + sq( y );

		++i;
		if( distSq >= iRadSq && distSq < oRadSq )
		{
			dest = color;
		}
	} );
}

void D3DGraphics::DrawRectangle( const RectI& _rect, Color color )noexcept
{
	fill2d( color, _rect, surface );
}
void D3DGraphics::DrawEmptyRectangle( const RectI& _rect, Color color )noexcept
{
	for( int y = _rect.Top(); y <= _rect.Bottom(); y++ )
	{
		PutPixel( _rect.Left(), y, color );
		PutPixel( _rect.Right(), y, color );
	}

	for( int x = _rect.Left(); x <= _rect.Right(); x++ )
	{
		PutPixel( x, _rect.Top(), color );
		PutPixel( x, _rect.Bottom(), color );
	}
}

void D3DGraphics::DrawTextBoxTopLeft( int _x, int _y, const Font& _font, Color _color )noexcept
{
	const auto charCenterX = ( _font.CharWidth() / 2 );
	const auto charCenterY = ( _font.CharHeight() / 2 );

	const auto right = _x + _font.CharWidth();
	const auto bottom = _y + _font.CharHeight();
	const auto center = _x + charCenterX;
	const auto middle = _y + charCenterY;

	DrawLine( center, middle, right, middle, _color );
	DrawLine( center, middle, center, bottom, _color );
}
void D3DGraphics::DrawTextBoxTopRight( int _x, int _y, const Font& _font, Color _color )noexcept
{
	const auto charCenterX = ( _font.CharWidth() / 2 );
	const auto charCenterY = ( _font.CharHeight() / 2 );

	const auto left = _x;
	const auto center = _x + charCenterX;
	const auto middle = _y + charCenterY;
	const auto bottom = _y + _font.CharHeight();

	DrawLine( left, middle, center, middle, _color );
	DrawLine( center, middle, center, bottom, _color );
}
void D3DGraphics::DrawTextBoxBottomLeft( int _x, int _y, const Font& _font, Color _color )noexcept
{
	const auto charCenterX = ( _font.CharWidth() / 2 );
	const auto charCenterY = ( _font.CharHeight() / 2 );

	const auto center = _x + charCenterX;
	const auto top = _y;
	const auto right = _x + _font.CharWidth();
	const auto middle = _y + charCenterY;

	DrawLine( center, top, center, middle, _color );
	DrawLine( center, middle, right, middle, _color );
}
void D3DGraphics::DrawTextBoxBottomRight( int _x, int _y, const Font& _font, Color _color )noexcept
{
	const auto charCenterX = ( _font.CharWidth() / 2 );
	const auto charCenterY = ( _font.CharHeight() / 2 );

	const auto left = _x;
	const auto top = _y;
	const auto center = _x + charCenterX;
	const auto middle = _y + charCenterY;

	DrawLine( center, top, center, middle, _color );
	DrawLine( left, middle, center, middle, _color );
}

RectI D3DGraphics::ClipToScreen( int x, int y, int width, int height ) const noexcept
{
	return {
		max( -x, 0 ),
		max( -y,0 ),
		min( ScreenWidth - x,width ),
		min( ScreenHeight - y,height )
	};
}
