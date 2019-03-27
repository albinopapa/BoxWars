#pragma once

#include "Vec2.h"
#include <random>

template<typename T>
auto GenerateNumber( T _min, T _range )
{
	static_assert( std::disjunction_v<std::is_integral<T>, std::is_floating_point<T>>,
		"T must be integral ( excluding char ) type or floating point type." );

	std::mt19937 rng( std::random_device{}() );
	if constexpr( std::is_integral_v<T> )
	{
		std::uniform_int_distribution<T> dist( _min, _min + _range );
		return dist( rng );
	}
	else if constexpr( std::is_floating_point_v<T> )
	{
		std::uniform_real_distribution<T> dist( _min, _min + _range );
		return dist( rng );
	}
}

template<typename T>
auto GenerateVector( Vec2<T> _min, Vec2<T> _range )
{
	return Vec2<T>{
		GenerateNumber( _min.x, _range.x ), GenerateNumber( _min.y, _range.y )
	};
}