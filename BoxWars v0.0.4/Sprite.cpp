#include "Sprite.h"
#ifdef LoadBitmap
#undef LoadBitmap
#endif

Sprite::Sprite( const char * filename, Color _key )
	:
	key( _key )
{
	auto image = Bitmap::LoadBitmap( filename );
	width = image.width;
	height = image.height;
	surface = std::move( image.pixels );
}

Sprite::Sprite( unsigned int _width, unsigned int _height )
	:
	width( _width ),
	height( _height ),
	surface( std::make_unique<Color[]>( _width * _height ) )
{
}

Sprite::Sprite( Sprite && _other ) noexcept
{
	*this = std::move( _other );
}

Sprite & Sprite::operator=( Sprite && _other )
{
	if( this != std::addressof( _other ) )
	{
		width = _other.width;
		height = _other.height;
		key = _other.key;
		surface = std::move( _other.surface );

		_other.width = 0;
		_other.height = 0;
		_other.key = Color{};
	}

	return *this;
}

Sprite::~Sprite()noexcept
{
	width = 0;
	height = 0;
	key = Color{};
}

Color & Sprite::operator()( unsigned int _x, unsigned int _y ) noexcept
{
	assert( _x >= 0 );
	assert( _x < width );
	assert( _y >= 0 );
	assert( _y < height );

	return surface[ _x + ( _y * width ) ];
}

const Color& Sprite::operator()( unsigned int _x, unsigned int _y ) const noexcept
{
	assert( _x >= 0 );
	assert( _x < width );
	assert( _y >= 0 );
	assert( _y < height );

	return surface[ _x + ( _y * width ) ];
}

unsigned int Sprite::Width() const noexcept
{
	return width;
}

unsigned int Sprite::Height() const noexcept
{
	return height;
}

bool Sprite::IsKey( Color color ) const noexcept
{
	return color == key;
}
