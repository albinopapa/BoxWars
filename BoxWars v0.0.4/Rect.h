#pragma once

template<typename T>
struct Rect
{
	Rect() = default;
	template<typename U>
	explicit constexpr Rect( const Rect<U>& _other )noexcept
		:
		x( static_cast< T >( _other.x ) ),
		y( static_cast< T >( _other.y ) ),
		width( static_cast< T >( _other.width ) ),
		height( static_cast< T >( _other.height ) )
	{}
	constexpr Rect( T _x, T _y, T _width, T _height )noexcept
		:
		x( _x ),
		y( _y ),
		width( _width ),
		height( _height )
	{}

	constexpr bool Overlaps( const Rect& _other )const noexcept
	{
		return
			( x < _other.x + _other.width && x + width >= _other.x ) &&
			( y < _other.y + _other.height && y + height >= _other.y );
	}
	constexpr bool Contains( T _x, T _y )const noexcept
	{
		return
			( _x >= Left() && _x < Right() ) &&
			( _y >= Top() && _y < Bottom() );
	}

	constexpr T Left()const noexcept { return x; }
	constexpr T Top()const noexcept { return y; }
	constexpr T Right()const noexcept { return x + width; }
	constexpr T Bottom()const noexcept { return y + height; }

	// TODO: Implement Vec2<T>
	/*Rect& operator+=( const Rect& _other )noexcept
	{
		x + _other.x;
		y + _other.y;
		return *this;
	}*/
	
	T x = T( 0 ), y = T( 0 ), width = T( 0 ), height = T( 0 );
};

using RectI = Rect<int>;
using RectF = Rect<float>;