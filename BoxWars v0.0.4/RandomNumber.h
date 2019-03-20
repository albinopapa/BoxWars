#pragma once

#include <random>

template<typename T>
auto GenerateNumber( T _min, T _range, std::random_device& _rd )
	->std::enable_if_t<std::is_integral_v<T>,T>
{
	std::mt19937 rng( _rd() );
	std::uniform_int_distribution<int> dist( _min, _min + _range );
	return dist( rng );
}

template<typename T>
auto GenerateNumber( T _min, T _range, std::random_device& _rd )
	->std::enable_if_t<std::is_floating_point_v<T>,T>
{
	std::mt19937 rng( _rd() );
	std::uniform_real_distribution<float> dist( _min, _min + _range );
	return dist( rng );
}