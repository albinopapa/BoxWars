/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Vec2.h																				  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once

#include "ChiliMath.h"

template <typename T>
class _Vec2
{
public:
	// Constructors
	constexpr _Vec2() = default;
	constexpr _Vec2( T x,T y )noexcept
		:
		x( x ),
		y( y )
	{}

	// Conversion operator
	template <typename T2>
	constexpr explicit operator _Vec2<T2>() const noexcept
	{
		return{ (T2)x,(T2)y };
	}

	// Unary operator
	constexpr _Vec2	operator-() const noexcept
	{
		return _Vec2( -x,-y );
	}

	// Assignment operators
	_Vec2&	operator+=( const _Vec2 &rhs ) noexcept
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	_Vec2&	operator-=( const _Vec2 &rhs ) noexcept
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	_Vec2&	operator*=( const T &rhs ) noexcept
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	_Vec2&	operator/=( const T &rhs ) noexcept
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}

	// Binary operators
	constexpr _Vec2	operator+( const _Vec2 &rhs ) const noexcept
	{
		return _Vec2( *this ) += rhs;
	}
	constexpr _Vec2	operator-( const _Vec2 &rhs ) const noexcept
	{
		return _Vec2( *this ) -= rhs;
	}
	constexpr _Vec2	operator*( const T &rhs ) const noexcept
	{
		return _Vec2( *this ) *= rhs;
	}
	constexpr T		operator*( const _Vec2 &rhs ) const noexcept
	{
		return x * rhs.x + y * rhs.y;
	}
	constexpr _Vec2	operator/( const T &rhs ) const noexcept
	{
		return _Vec2( *this ) /= rhs;
	}

	constexpr bool	operator==( const _Vec2 &rhs ) const noexcept
	{
		return x == rhs.x && y == rhs.y;
	}
	constexpr bool	operator!=( const _Vec2 &rhs ) const noexcept
	{
		return !(*this == rhs);
	}

	constexpr T		LenSq() const noexcept
	{
		return sq( *this );
	}
	constexpr T		Len() const noexcept
	{
		return sqrt( LenSq() );
	}

	_Vec2&	Normalize() noexcept
	{
		const auto len = Len();
		*this *= ( len != static_cast< T >( 0 ) ? 1.f / len : len );
		return *this;
	}
	constexpr _Vec2	Normalize() const noexcept
	{
		const auto len = Len();
		return *this * ( len != static_cast< T >( 0 ) ? 1.f / len : len );
	}

	constexpr _Vec2	InterpolateTo( const _Vec2& dest,T alpha ) const noexcept
	{
		const auto range = dest - ( *this );
		return ( *this ) + ( range * alpha );
	}
	_Vec2 &InterpolateTo( const _Vec2 &dest, T alpha ) noexcept
	{
		const auto range = dest - ( *this );
		( *this ) += ( range * alpha );
		return *this;
	}

public:
	T x;
	T y;
};

template<class T>
_Vec2<T> operator*( const float S, const _Vec2<T>& V ) noexcept
{
	return V * S;
}
template<class T>
_Vec2<T> Reflect( const _Vec2<T> &Direction, const _Vec2<T> &Normal ) noexcept
{
	return Direction - ( ( 2.f * ( Direction * Normal ) ) * Normal );
}

using Vec2f = _Vec2<float>;
using Vec2d = _Vec2<double>;
using Vec2i = _Vec2<int>;
