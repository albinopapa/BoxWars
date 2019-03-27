#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include <sstream>

#if defined( _UNICODE ) || defined( UINICODE )
namespace std
{
	using tstring = wstring;
	using tistream = wistream;
	using tostream = wostream;
	using tiostream = wiostream;
	using tstringstream = wstringstream;
}

inline std::tstring make_native( const std::string& _str )
{
	return std::tstring( _str.begin(), _str.end() );
}
template<typename T>
std::tstring make_native( const T* _str )
{
	const auto len = std::char_traits<T>::length( _str );
	return std::tstring( _str, std::addressof( _str[ len ] ) );
}
inline std::tstring make_native( std::wstring _str )noexcept
{
	return _str;
}
#else
namespace std
{
	using tstring = string;
	using tistream = istream;
	using tostream = ostream;
	using tiostream = iostream;
	using tstringstream = stringstream;
}

inline std::tstring make_native( const std::string& _str )
{
	return _str;
}
template<typename T>
std::tstring make_native( const T* _str )
{
	const auto len = std::char_traits<T>::length( _str );
	return std::tstring( _str, std::addressof( _str[ len ] ) );
}
inline std::tstring make_native( std::wstring _str )noexcept
{
	return std::tstring( _str.begin(), _str.end() );
}
#endif

#ifdef D3D11LIB_EXPORTS
#define DLLEXPORT __declspec( dllexport )
#else
#define DLLEXPORT __declspec( dllimport )
#endif


struct Point
{
	std::int32_t x = 0, y = 0;
};
constexpr Point operator+( const Point& _a, const Point& _b )noexcept
{
	return { _a.x + _b.x,_a.y + _b.y };
}
constexpr Point operator-( const Point& _a, const Point& _b )noexcept
{
	return { _a.x - _b.x,_a.y - _b.y };
}
constexpr Point operator*( const Point& _a, float _b )noexcept
{
	return { std::int32_t( float( _a.x ) * _b ), std::int32_t( float( _a.y ) * _b ) };
}
constexpr Point operator/( const Point& _a, std::int32_t _b )
{
	if( _b == 0 )
		throw std::domain_error( "Integer division by 0." );

	return { _a.x / _b,_a.y / _b };
}


template<typename FuncLast>
struct do_finally
{
	do_finally( FuncLast&& _onDestruct )
		:
		m_onDestruct( _onDestruct )
	{}
	~do_finally()
	{
		m_onDestruct();
	}

	FuncLast m_onDestruct;
};

template<typename T>
constexpr T mod_abs( const T& _value, const T& _max )noexcept
{
	static_assert( std::disjunction_v< std::is_integral<T>, std::is_floating_point<T>>,
		"T must be an integral or floating point type." );

	if constexpr( std::is_integral_v<T> )
	{
		return std::abs( ( _value + _max ) ) % _max ;
	}
	else if constexpr( std::is_floating_point_v<T> )
	{
		return std::fmod( std::abs( _value + _max ), _max );
	}
}

template<typename Cont1, typename Cont2, typename RectType>
void copy2d( const RectType& _bounds, const Cont1& _source, Cont2& _dest )
{
	using type_1 = typename Cont1::value_type;
	using type_2 = typename Cont1::value_type;

	const auto width = _bounds.width;

	for( int y = _bounds.Top(); y < _bounds.Bottom(); ++y )
	{
		if constexpr( std::is_trivially_copyable_v<type_1> && std::is_same_v<type_1, type_2> )
		{
			memcpy( std::addressof( _dest( 0, y ) ), std::addressof( _source( 0, y ) ), sizeof( type_1 ) * width );
		}
		else
		{
			for( int x = _bounds.Left(); x < _bounds.Right(); ++x )
			{
				_dest( x, y ) = _source( x, y );
			}
		}
	}
}

template<typename Cont1, typename Cont2, typename RectType, typename Pred>
void copy2d_if(
	const RectType& _srcBounds, const RectType& _dstBounds, const Cont1& _source, Cont2& _dest,
	Pred&& _pred )
{
	if( _srcBounds.width != _dstBounds.width &&
		_srcBounds.height != _dstBounds.height )
	{
		throw std::runtime_error( "Source and destination RectTypes must be same dimensions." );
	}

	for( int y = 0; y < _srcBounds.height; ++y )
	{
		const auto srcy = y + _srcBounds.Top();
		const auto dsty = y + _dstBounds.Top();

		for( int x = 0; x < _srcBounds.width; ++x )
		{
			const auto srcx = x + _srcBounds.Left();
			const auto dstx = x + _dstBounds.Left();

			if( const auto& elem = _source( srcx, srcy ); _pred( elem ) )
				_dest( dstx, dsty ) = elem;
		}
	}
}

template<typename Cont, typename RectType, typename Func>
void for_each2d( const RectType& _bounds, Cont& _container, Func&& _func )
{
	for( std::int32_t y = _bounds.Top(); y < _bounds.Bottom(); ++y )
	{
		for( std::int32_t x = _bounds.Left(); x < _bounds.Right(); ++x )
		{
			_func( _container( x, y ) );
		}
	}
}

template<typename Cont, typename RectType>
void fill2d( const typename Cont::value_type& _value, const RectType& _bounds, Cont& _container )
{
	for( int y = _bounds.Top(); y < _bounds.Bottom(); ++y )
	{
		for( int x = _bounds.Left(); x < _bounds.Right(); ++x )
		{
			_container( x, y ) = _value;
		}
	}
}

template<typename Cont, typename RectType, typename Generator>
void generate2d( const RectType& _bounds, Cont& _cont, Generator&& _generate )
{
	for( int y = _bounds.Top(); y < _bounds.Bottom(); ++y )
	{
		for( int x = _bounds.Left(); x < _bounds.Right(); ++x )
		{
			_cont( x, y ) = _generate();
		}
	}
}

template<typename Cont1, typename Cont2, typename RectType>
void replace2d(
	const typename Cont1::value_type& _replace_this, 
	const typename Cont2::value_type& _with_that, 
	const RectType& _srcBounds, 
	const RectType& _dstBounds, 
	const Cont1& _src,
	Cont2& _dst)
{	
	if( _srcBounds.width != _dstBounds.width &&
		_srcBounds.height != _dstBounds.height )
	{
		throw std::runtime_error( "Source and destination RectTypes must be same dimensions." );
	}

	for( int y = 0; y < _srcBounds.height; ++y )
	{
		const auto srcy = y + _srcBounds.Top();
		const auto dsty = y + _dstBounds.Top();

		for( int x = 0; x < _srcBounds.width; ++x )
		{
			const auto srcx = x + _srcBounds.Left();
			const auto dstx = x + _dstBounds.Left();

			if( _src( srcx, srcy ) == _replace_this )
				_dst( dstx, dsty ) = _with_that;
		}
	}
}

template<typename Cont1, typename RectType, typename Pred>
void replace2d_if(
	const typename Cont1::value_type& _new_value,
	const RectType& _srcBounds,
	const RectType& _dstBounds,
	const Cont1& _src, 
	Cont1& _dst,
	Pred&& _pred )
{
	if( _srcBounds.width != _dstBounds.width &&
		_srcBounds.height != _dstBounds.height )
	{
		throw std::runtime_error( "Source and destination RectTypes must be same dimensions." );
	}

	for( int y = 0; y < _srcBounds.height; ++y )
	{
		const auto srcy = y + _srcBounds.Top();
		const auto dsty = y + _dstBounds.Top();

		for( int x = 0; x < _srcBounds.width; ++x )
		{
			const auto srcx = x + _srcBounds.Left();
			const auto dstx = x + _dstBounds.Left();
			const auto& elem0 = _src( srcx, srcy );
			auto& elem1 = _dst( dstx, dsty );
			if( _pred( elem0, elem1 ) )
			{
				elem1 = _new_value;
			}
		}
	}
}

template<typename Cont1, typename Cont2, typename RectType, typename Func>
void transform2d(
	const RectType& _srcBounds, const RectType& _dstBounds,
	const Cont1& _src, Cont2& _dst, Func&& _func )
{
	if( _srcBounds.width != _dstBounds.width &&
		_srcBounds.height != _dstBounds.height )
	{
		throw std::runtime_error( "Source and destination RectTypes must be same dimensions." );
	}

	for( int y = 0; y < _srcBounds.height; ++y )
	{
		const auto srcy = y + _srcBounds.Top();
		const auto dsty = y + _dstBounds.Top();

		for( int x = 0; x < _srcBounds.width; ++x )
		{
			const auto srcx = x + _srcBounds.Left();
			const auto dstx = x + _dstBounds.Left();

			_dst( dstx, dsty ) = _func( _src( srcx, srcy ) );
		}
	}
}
