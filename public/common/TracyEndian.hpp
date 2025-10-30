#pragma once

#include <bit>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <type_traits>

constexpr std::endian network_byteorder()
{
#if TRACY_BIGENDIAN
    return std::endian::big;
#else
    return std::endian::little;
#endif
}

template<typename T>
concept TrivialInteger = std::is_integral_v<T> && std::is_trivially_copyable_v<T>;

template<typename T>
concept FloatingPoint = std::is_floating_point_v<T>;

// trival to copy struct with no padding
template<typename T>
concept TrivialStruct = requires( T val ) {{val.convert_endian()}; } || ( std::is_class_v<T> && std::is_trivially_copyable_v<T> && std::has_unique_object_representations_v<T> );

template<typename T>
concept NetworkSerializable = TrivialInteger<T> || std::is_enum_v<T> || FloatingPoint<T> || TrivialStruct<T>;

template<TrivialInteger T>
constexpr T convert_endian( T value ) noexcept
{
    if constexpr( std::endian::native == network_byteorder() || sizeof( T ) == 1 )
    {
        return value;
    }
    else if constexpr( sizeof( T ) == 2 )
    {
        return static_cast<T>( __builtin_bswap16( static_cast<uint16_t>( value ) ) );
    }
    else if constexpr( sizeof( T ) == 4 )
    {
        return static_cast<T>( __builtin_bswap32( static_cast<uint32_t>( value ) ) );
    }
    else if constexpr( sizeof( T ) == 8 )
    {
        return static_cast<T>( __builtin_bswap64( static_cast<uint64_t>( value ) ) );
    }
}

template<typename T>
    requires std::is_enum_v<T>
constexpr auto convert_endian( T value ) noexcept
{
    return convert_endian( static_cast<std::underlying_type_t<T>>( value ) );
}

template<FloatingPoint T>
constexpr T convert_endian( T value ) noexcept
{
    if constexpr( std::endian::native == network_byteorder() )
    {
        return value;
    }
    else if constexpr( sizeof( T ) == sizeof( float ) )
    {
        static_assert( sizeof( float ) == sizeof( uint32_t ), "float must be 4 bytes" );
        uint32_t tmp;
        std::memcpy( &tmp, &value, sizeof( float ) );
        tmp = convert_endian( tmp );
        float result;
        std::memcpy( &result, &tmp, sizeof( float ) );
        return result;
    }
    else if constexpr( sizeof( T ) == sizeof( double ) )
    {
        static_assert( sizeof( double ) == sizeof( uint64_t ), "double must be 8 bytes" );
        uint64_t tmp;
        std::memcpy( &tmp, &value, sizeof( double ) );
        tmp = convert_endian( tmp );
        double result;
        std::memcpy( &result, &tmp, sizeof( double ) );
        return result;
    }
}

template<TrivialStruct T>
static void convert_endian( T& value )
{
    if constexpr( std::endian::native == network_byteorder() )
    {
        return;
    }
    else
    {
        if constexpr( requires { value.convert_endian(); } )
        {
            value.convert_endian();
        }
        else
        {
            static_assert( std::is_trivially_copyable_v<T>,
                           "Struct must be trivially copyable or implement convert_endian()" );
        }
    }
}
