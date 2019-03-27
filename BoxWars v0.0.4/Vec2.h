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
class Vec2
{
public:
	// Constructors
	constexpr Vec2() = default;
	constexpr Vec2( T x,T y )noexcept
		:
		x( x ),
		y( y )
	{}

	// Conversion operator
	template <typename T2>
	constexpr explicit operator Vec2<T2>() const noexcept
	{
		return{ (T2)x,(T2)y };
	}

	// Unary operator
	constexpr Vec2	operator-() const noexcept
	{
		return Vec2( -x,-y );
	}

	// Assignment operators
	Vec2&	operator+=( const Vec2 &rhs ) noexcept
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vec2&	operator-=( const Vec2 &rhs ) noexcept
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	Vec2&	operator*=( const T &rhs ) noexcept
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	Vec2&	operator/=( const T &rhs ) noexcept
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}

	// Binary operators
	constexpr Vec2	operator+( const Vec2 &rhs ) const noexcept
	{
		return Vec2( *this ) += rhs;
	}
	constexpr Vec2	operator-( const Vec2 &rhs ) const noexcept
	{
		return Vec2( *this ) -= rhs;
	}
	constexpr Vec2	operator*( const T &rhs ) const noexcept
	{
		return Vec2( *this ) *= rhs;
	}
	constexpr T		operator*( const Vec2 &rhs ) const noexcept
	{
		return x * rhs.x + y * rhs.y;
	}
	constexpr Vec2	operator/( const T &rhs ) const noexcept
	{
		return Vec2( *this ) /= rhs;
	}

	constexpr bool	operator==( const Vec2 &rhs ) const noexcept
	{
		return x == rhs.x && y == rhs.y;
	}
	constexpr bool	operator!=( const Vec2 &rhs ) const noexcept
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

	Vec2&	Normalize() noexcept
	{
		const auto len = Len();
		*this *= ( len != static_cast< T >( 0 ) ? 1.f / len : len );
		return *this;
	}
	constexpr Vec2	Normalize() const noexcept
	{
		const auto len = Len();
		return *this * ( len != static_cast< T >( 0 ) ? 1.f / len : len );
	}

	constexpr Vec2	InterpolateTo( const Vec2& dest,T alpha ) const noexcept
	{
		const auto range = dest - ( *this );
		return ( *this ) + ( range * alpha );
	}
	Vec2 &InterpolateTo( const Vec2 &dest, T alpha ) noexcept
	{
		const auto range = dest - ( *this );
		( *this ) += ( range * alpha );
		return *this;
	}

public:
	T x = T( 0 );
	T y = T( 0 );
};

template<class T>
Vec2<T> operator*( const float S, const Vec2<T>& V ) noexcept
{
	return V * S;
}
template<class T>
Vec2<T> Reflect( const Vec2<T> &Direction, const Vec2<T> &Normal ) noexcept
{
	return Direction - ( ( 2.f * ( Direction * Normal ) ) * Normal );
}

using Vec2f = Vec2<float>;
using Vec2d = Vec2<double>;
using Vec2i = Vec2<int>;
