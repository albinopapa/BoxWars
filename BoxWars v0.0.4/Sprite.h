#pragma once

#include "Bitmap.h"
#include "Color.h"
#include <cassert>
#include <fstream>
#include <memory>

class Sprite
{
public:
	using value_type = Color;
	using pointer = Color * ;
	using const_pointer = const Color*;
	using reference = Color & ;
	using const_reference = const Color&;
public:
	Sprite() = default;
	Sprite( const char* filename, Color _key = Color{ 255,0,255 } );
	Sprite( unsigned int _width, unsigned int _height );
	Sprite( const Sprite& ) = delete;
	Sprite( Sprite&& _other )noexcept;

	Sprite& operator=( const Sprite& ) = delete;
	Sprite& operator=( Sprite&& _other );
	~Sprite()noexcept;

	Color& operator()( unsigned int _x, unsigned int _y )noexcept;
	const Color& operator()( unsigned int _x, unsigned int _y )const noexcept;

	unsigned int Width()const noexcept;
	unsigned int Height()const noexcept;

	const Color* GetBuffer()const noexcept;
	void Clear()noexcept;
	bool IsKey( Color color )const noexcept;
private:
	unsigned int width = 0u, height = 0u;
	Color key = Color{ 255,0,255 };
	std::unique_ptr<Color[]> surface;
};

#include <d3d9.h>
class D3D9RenderTarget
{
public:
	using value_type = Color;
	using pointer = Color * ;
	using const_pointer = const Color*;
	using reference = Color & ;
	using const_reference = const Color&;
public:
	D3D9RenderTarget( IDirect3DSurface9* _backBuffer )
		:
		m_backBuffer( _backBuffer )
	{
		D3DLOCKED_RECT backRect{};
		if( HRESULT hr = m_backBuffer->LockRect( &backRect, nullptr, 0 ); FAILED( hr ) )
		{
			throw std::runtime_error( "Failed to lock back buffer." );
		}
		stride = static_cast< size_t >( backRect.Pitch ) / sizeof( Color );
		buffer = reinterpret_cast< Color* >( backRect.pBits );
	}
	reference operator()( size_t x, size_t y )
	{
		return buffer[ x + ( y * stride ) ];
	}
	void Copy( const Color* pSource, int width, int height )
	{
		const auto count = width * sizeof( Color );
		for( int y = 0; y < height; ++y )
		{
			memcpy( std::addressof( buffer[ y * stride ] ), std::addressof( pSource[ y * width ] ), count );
		}
	}

	~D3D9RenderTarget()
	{
		m_backBuffer->UnlockRect();
	}
private:
	IDirect3DSurface9* m_backBuffer = nullptr;
	Color* buffer = nullptr;
	size_t stride = 0;
};