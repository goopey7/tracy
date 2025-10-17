#ifndef __TRACYALIGN_HPP__
#define __TRACYALIGN_HPP__

#pragma once

#include <cstdint>
#include <string.h>
#ifdef __Wii__
#define htobe16(x) (x)
#define htobe32(x) (x)
#define htobe64(x) (x)
#define be16toh(x) (x)
#define be32toh(x) (x)
#define be64toh(x) (x)
#else
#include <endian.h>
#endif

#include "TracyForceInline.hpp"

namespace tracy
{

template<typename T>
tracy_force_inline T MemRead( const void* ptr )
{
    T val;
    memcpy( &val, ptr, sizeof( T ) );
    return val;
}

template<typename T>
tracy_force_inline void MemWrite( void* ptr, T val )
{
    memcpy( ptr, &val, sizeof( T ) );
}

static inline void float_to_be(float val, uint8_t out[4])
{
    uint32_t tmp;
    memcpy(&tmp, &val, sizeof(tmp));
    tmp = htobe32(tmp);
    memcpy(out, &tmp, sizeof(tmp));
}

static inline void double_to_be(double val, uint8_t out[8])
{
    uint64_t tmp;
    memcpy(&tmp, &val, sizeof(tmp));
    tmp = htobe64(tmp);
    memcpy(out, &tmp, sizeof(tmp));
}

template<>
tracy_force_inline void MemWrite<float>(void* ptr, float val)
{
	uint8_t tmp4[4];
	float_to_be(val, tmp4);
	memcpy(ptr, tmp4, 4);
}

template<>
tracy_force_inline void MemWrite<double>(void* ptr, double val)
{
	uint8_t tmp8[8];
	double_to_be(val, tmp8);
	memcpy(ptr, tmp8, 8);
}

template<>
tracy_force_inline void MemWrite<uint16_t>(void* ptr, uint16_t val)
{
	uint16_t val_be = htobe16(val);
	memcpy(ptr, &val_be, sizeof(uint16_t));
}

template<>
tracy_force_inline void MemWrite<uint32_t>(void* ptr, uint32_t val)
{
	uint32_t val_be = htobe32(val);
	memcpy(ptr, &val_be, sizeof(uint32_t));
}

template<>
tracy_force_inline void MemWrite<uint64_t>(void* ptr, uint64_t val)
{
	uint64_t val_be = htobe64(val);
	memcpy(ptr, &val_be, sizeof(uint64_t));
}

template<>
tracy_force_inline void MemWrite<int16_t>(void* ptr, int16_t val)
{
	uint16_t val_be = htobe16(static_cast<uint16_t>(val));
	memcpy(ptr, &val_be, sizeof(uint16_t));
}

template<>
tracy_force_inline void MemWrite<int32_t>(void* ptr, int32_t val)
{
	uint32_t val_be = htobe32(static_cast<uint32_t>(val));
	memcpy(ptr, &val_be, sizeof(uint32_t));
}

template<>
tracy_force_inline void MemWrite<int64_t>(void* ptr, int64_t val)
{
	uint64_t val_be = htobe64(static_cast<uint64_t>(val));
	memcpy(ptr, &val_be, sizeof(uint64_t));
}

static inline float float_from_be(const uint8_t in[4])
{
    uint32_t tmp;
    memcpy(&tmp, in, sizeof(tmp));
    tmp = be32toh(tmp);
    float val;
    memcpy(&val, &tmp, sizeof(val));
    return val;
}

static inline double double_from_be(const uint8_t in[8])
{
    uint64_t tmp;
    memcpy(&tmp, in, sizeof(tmp));
    tmp = be64toh(tmp);
    double val;
    memcpy(&val, &tmp, sizeof(val));
    return val;
}

template<>
tracy_force_inline float MemRead(const void* ptr)
{
	return float_from_be(static_cast<const uint8_t*>(ptr));
}

template<>
tracy_force_inline double MemRead(const void* ptr)
{
	return double_from_be(static_cast<const uint8_t*>(ptr));
}

template<>
tracy_force_inline uint16_t MemRead(const void* ptr)
{
	uint16_t tmp;
    memcpy(&tmp, ptr, sizeof(tmp));
    return be16toh(tmp);
}

template<>
tracy_force_inline uint32_t MemRead(const void* ptr)
{
	uint32_t tmp;
    memcpy(&tmp, ptr, sizeof(tmp));
    return be32toh(tmp);
}

template<>
tracy_force_inline uint64_t MemRead(const void* ptr)
{
	uint64_t tmp;
    memcpy(&tmp, ptr, sizeof(tmp));
    return be64toh(tmp);
}

template<>
tracy_force_inline int16_t MemRead(const void* ptr)
{
	uint16_t tmp;
    memcpy(&tmp, ptr, sizeof(tmp));
    return static_cast<int16_t>(be16toh(tmp));
}

template<>
tracy_force_inline int32_t MemRead(const void* ptr)
{
	uint32_t tmp;
    memcpy(&tmp, ptr, sizeof(tmp));
    return static_cast<int32_t>(be32toh(tmp));
}

template<>
tracy_force_inline int64_t MemRead(const void* ptr)
{
	uint64_t tmp;
    memcpy(&tmp, ptr, sizeof(tmp));
    return static_cast<int64_t>(be64toh(tmp));
}

}

#endif
