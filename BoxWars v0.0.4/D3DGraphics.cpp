#include "ChiliMath.h"
#include "D3DGraphics.h"
#include "../../D3D11Lib/D3D11Lib/Def.h"
#include "Bitmap.h"
#include <cassert>
#pragma comment(lib, "d3d9.lib")

D3DGraphics::D3DGraphics( HWND hWnd )
	:
	pDirect3D( Direct3DCreate9( D3D_SDK_VERSION ) ),
	surface( TOTALSCREENWIDTH, TOTALSCREENHEIGHT )
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
	assert( x < TOTALSCREENWIDTH );
	assert( y < TOTALSCREENHEIGHT );

	surface( x, y ) = color;
}
void D3DGraphics::BeginFrame()noexcept
{	
	fill2d( Colors::gray, RectI{ 0, 0, TOTALSCREENWIDTH, TOTALSCREENHEIGHT }, surface );
}
void D3DGraphics::EndFrame()
{
	{
		D3D9RenderTarget target( pBackBuffer.Get() );

		copy2d( RectI{ 0,0,TOTALSCREENWIDTH,TOTALSCREENHEIGHT }, surface, target );
	}

	const auto result = pDevice->Present( nullptr, nullptr, nullptr, nullptr );
	assert( !FAILED( result ) );
}

void D3DGraphics::DrawSprite( int xoff, int yoff, const Sprite& sprite )noexcept
{	
	auto isAlpha = [ & ]( const Color& _color ) { return !sprite.IsKey( _color ); };

	const auto srcRect = RectI{ 0,0,int( sprite.Width() ),int( sprite.Height() ) };
	const auto dstRect = RectI{ xoff,yoff,int( sprite.Width() ),int( sprite.Height() ) };

	copy2d_if( srcRect, dstRect, sprite, surface, isAlpha );
}
void D3DGraphics::DrawChar( char c, int xoff, int yoff, const Font& font, Color color )noexcept
{
	const auto srcRect = font.CharRect( c );
	const auto dstRect = RectI{ 
		xoff - srcRect.Left(),
		yoff - srcRect.Top(),
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
void D3DGraphics::DrawCircle( int centerX, int centerY, int radius, Color color )noexcept
{
	auto Square = []( auto _value ) {return _value * _value; };
	auto fill4 = [&]( int _x, int _y )
	{
		PutPixel( centerX + _x, centerY + _y, color );
		PutPixel( centerX - _x, centerY + _y, color );
		PutPixel( centerX + _x, centerY - _y, color );
		PutPixel( centerX - _x, centerY - _y, color );
	};

	const int rSquared = sq( radius );
	const int xPivot = ( int )( radius * 0.707107f + 0.5f );
	for( int x = 0; x <= xPivot; x++ )
	{
		const auto ySq = ( float )( rSquared - Square( x ) );
		const int y = ( int )( std::sqrt( ySq ) + 0.5f );
		fill4( x, y );
		fill4( y, x );
	}
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
