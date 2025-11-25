#pragma once

#include <bit>
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <type_traits>

#ifdef _WIN32
#define bswap16(x) _byteswap_ushort(x)
#define bswap32(x) _byteswap_ulong(x)
#define bswap64(x) _byteswap_uint64(x)
#else
#define bswap16(x) __builtin_bswap16(x)
#define bswap32(x) __builtin_bswap32(x)
#define bswap64(x) __builtin_bswap64(x)
#endif

constexpr std::endian network_byteorder()
{
#if TRACY_BIGENDIAN
    return std::endian::big;
#else
    return std::endian::little;
#endif
}

template<typename T>
concept StructWithConvertEndianMethod = requires( T val ) {{ val.convert_endian()}; };

template<typename T>
    requires std::is_integral_v<T>
constexpr T convert_endian( T value ) noexcept
{
    if constexpr( std::endian::native == network_byteorder() || sizeof( T ) == 1 )
    {
        return value;
    }
    else if constexpr( sizeof( T ) == 2 )
    {
        return static_cast<T>( bswap16( static_cast<uint16_t>( value ) ) );
    }
    else if constexpr( sizeof( T ) == 4 )
    {
        return static_cast<T>( bswap32( static_cast<uint32_t>( value ) ) );
    }
    else if constexpr( sizeof( T ) == 8 )
    {
        return static_cast<T>( bswap64( static_cast<uint64_t>( value ) ) );
    }
}

template<typename T>
    requires std::is_enum_v<T>
constexpr T convert_endian( T value ) noexcept
{
    return static_cast<T>( convert_endian( static_cast<std::underlying_type_t<T>>( value ) ) );
}

template<typename T>
    requires std::is_floating_point_v<T>
constexpr T convert_endian( T value ) noexcept
{
    if constexpr( std::endian::native == network_byteorder() )
    {
        return value;
    }
    else if constexpr( sizeof( T ) == sizeof( float ) )
    {
        static_assert( sizeof( float ) == sizeof( uint32_t ), "float must be 4 bytes" );
        uint32_t tmp = std::bit_cast<uint32_t>( value );
        tmp = convert_endian( tmp );
        return std::bit_cast<float>( tmp );
    }
    else if constexpr( sizeof( T ) == sizeof( double ) )
    {
        static_assert( sizeof( double ) == sizeof( uint64_t ), "double must be 8 bytes" );
        uint64_t tmp = std::bit_cast<uint64_t>( value );
        tmp = convert_endian( tmp );
        return std::bit_cast<double>( tmp );
    }
}

template<StructWithConvertEndianMethod T>
constexpr void convert_endian( T& value )
{
    if constexpr( std::endian::native == network_byteorder() )
    {
        return;
    }
    else
    {
        value.convert_endian();
    }
}
