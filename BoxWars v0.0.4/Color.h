#pragma once
class Color
{
public:
	Color() = default;
	constexpr Color( unsigned int _red, unsigned int _grn, unsigned int _blu, unsigned int _alpha = 255u )noexcept
		:
		value( ( _alpha << 24 ) | ( _red << 16 ) | ( _grn << 8 ) | _blu )
	{}
	constexpr unsigned int GetA()const noexcept
	{
		return ( value & 0xFF000000 ) >> 24;
	}
	constexpr unsigned int GetR()const noexcept
	{
		return ( value & 0xFF0000 ) >> 16;
	}
	constexpr unsigned int GetG()const noexcept
	{
		return ( value & 0xFF00 ) >> 8;
	}
	constexpr unsigned int GetB()const noexcept
	{
		return ( value & 0xFF );
	}
	constexpr void SetA( unsigned int _value )noexcept
	{
		constexpr auto _RGB = 0x00FFFFFF;
		value = ( value & _RGB ) | ( ( _value << 24 ) & ~_RGB);
	}
	constexpr void SetR( unsigned int _value )noexcept
	{
		constexpr auto A_GB = 0xFF00FFFF;
		value = ( value & A_GB ) | ( ( _value << 16 ) & ~A_GB );
	}
	constexpr void SetG( unsigned int _value )noexcept
	{
		constexpr auto AR_B = 0xFFFF00FF;
		value = ( value & AR_B ) | ( ( _value << 8 ) & ~AR_B );
	}
	constexpr void SetB( unsigned int _value )noexcept
	{
		constexpr auto ARG_ = 0xFFFFFF00;
		value = ( value & ARG_ ) | ( _value & ~ARG_ );
	}
private:
	unsigned int value = ( 255 << 24 );
};

constexpr bool operator==( Color _left, Color _right )noexcept
{
	return
		_left.GetA() == _right.GetA() &&
		_left.GetR() == _right.GetR() &&
		_left.GetG() == _right.GetG() &&
		_left.GetB() == _right.GetB();
}

constexpr bool operator!=( Color _left, Color _right )noexcept
{
	return !( _left == _right );
}

namespace Colors
{
	constexpr auto white = Color( 255, 255, 255 );
	constexpr auto black = Color( 0, 0, 0 );
	constexpr auto gray = Color{ 128, 128, 128 };

	constexpr auto red = Color( 255, 0, 0 );
	constexpr auto dark_red = Color{ 128,0,0 };

	constexpr auto green = Color( 0, 255, 0 );
	constexpr auto dark_green = Color{ 0, 128, 0 };

	constexpr auto blue = Color( 0, 0, 255 );
	constexpr auto dark_blue = Color{ 0, 0, 128 };

	constexpr auto yellow = Color( 255, 255, 0 );
	
	constexpr auto cyan = Color( 0, 255, 255 );
	
	constexpr auto magenta = Color( 255, 0, 255 );
}