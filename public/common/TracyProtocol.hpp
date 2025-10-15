#ifndef __TRACYPROTOCOL_HPP__
#define __TRACYPROTOCOL_HPP__

#include <cstring>
#include <endian.h>
#include <limits>
#include <stdint.h>

namespace tracy
{

constexpr unsigned Lz4CompressBound( unsigned isize ) { return isize + ( isize / 255 ) + 16; }

enum : uint32_t { ProtocolVersion = 77 };
enum : uint16_t { BroadcastVersion = 3 };

using lz4sz_t = uint32_t;

enum { TargetFrameSize = 256 * 1024 };
enum { LZ4Size = Lz4CompressBound( TargetFrameSize ) };
static_assert( LZ4Size <= (std::numeric_limits<lz4sz_t>::max)(), "LZ4Size greater than lz4sz_t" );
static_assert( TargetFrameSize * 2 >= 64 * 1024, "Not enough space for LZ4 stream buffer" );

enum { HandshakeShibbolethSize = 8 };
static const char HandshakeShibboleth[HandshakeShibbolethSize] = { 'T', 'r', 'a', 'c', 'y', 'P', 'r', 'f' };

enum HandshakeStatus : uint8_t
{
    HandshakePending,
    HandshakeWelcome,
    HandshakeProtocolMismatch,
    HandshakeNotAvailable,
    HandshakeDropped
};

enum { WelcomeMessageProgramNameSize = 64 };
enum { WelcomeMessageHostInfoSize = 1024 };

#pragma pack( push, 1 )

// Must increase left query space after handling!
enum ServerQuery : uint8_t
{
    ServerQueryTerminate,
    ServerQueryString,
    ServerQueryThreadString,
    ServerQuerySourceLocation,
    ServerQueryPlotName,
    ServerQueryFrameName,
    ServerQueryParameter,
    ServerQueryFiberName,
    ServerQueryExternalName,
    // Items above are high priority. Split order must be preserved. See IsQueryPrio().
    ServerQueryDisconnect,
    ServerQueryCallstackFrame,
    ServerQuerySymbol,
    ServerQuerySymbolCode,
    ServerQuerySourceCode,
    ServerQueryDataTransfer,
    ServerQueryDataTransferPart
};

struct ServerQueryPacket
{
    ServerQuery type;
    uint64_t ptr;
    uint32_t extra;
};

enum { ServerQueryPacketSize = sizeof( ServerQueryPacket ) };


enum CpuArchitecture : uint8_t
{
    CpuArchUnknown,
    CpuArchX86,
    CpuArchX64,
    CpuArchArm32,
    CpuArchArm64
};


struct WelcomeFlag
{
    enum _t : uint8_t
    {
        OnDemand        = 1 << 0,
        IsApple         = 1 << 1,
        CodeTransfer    = 1 << 2,
        CombineSamples  = 1 << 3,
        IdentifySamples = 1 << 4,
    };
};

struct WelcomeMessage
{
    double timerMul;
    int64_t initBegin;
    int64_t initEnd;
    uint64_t resolution;
    uint64_t epoch;
    uint64_t exectime;
    uint64_t pid;
    int64_t samplingPeriod;
    uint8_t flags;
    uint8_t cpuArch;
    char cpuManufacturer[12];
    uint32_t cpuId;
    char programName[WelcomeMessageProgramNameSize];
    char hostInfo[WelcomeMessageHostInfoSize];
};

static inline void double_to_be(double val, uint8_t out[8]) {
    uint64_t tmp;
    std::memcpy(&tmp, &val, sizeof(tmp));
    tmp = htobe64(tmp);
    std::memcpy(out, &tmp, sizeof(tmp));
}

inline void Serialize(const WelcomeMessage& msg, uint8_t* buffer, std::size_t bufferSize)
{
	uint8_t* p = buffer;

    auto write64 = [&](uint64_t v) {
        if (p + 8 > buffer + bufferSize) return false;
        uint64_t be = htobe64(v);
        std::memcpy(p, &be, 8);
        p += 8;
        return true;
    };

    auto write32 = [&](uint32_t v) {
        if (p + 4 > buffer + bufferSize) return false;
        uint32_t be = htobe32(v);
        std::memcpy(p, &be, 4);
        p += 4;
        return true;
    };

    auto write8 = [&](uint8_t v) {
        if (p + 1 > buffer + bufferSize) return false;
        *p++ = v;
        return true;
    };

	uint8_t tmp8[8];
    double_to_be(msg.timerMul, tmp8);
    std::memcpy(p, tmp8, 8);
    p += 8;

	write64(static_cast<uint64_t>(msg.initBegin));
    write64(static_cast<uint64_t>(msg.initEnd));
    write64(msg.resolution);
    write64(msg.epoch);
    write64(msg.exectime);
    write64(msg.pid);
    write64(static_cast<uint64_t>(msg.samplingPeriod));

    write8(msg.flags);
    write8(msg.cpuArch);

    std::memcpy(p, msg.cpuManufacturer, sizeof(msg.cpuManufacturer));
    p += sizeof(msg.cpuManufacturer);

    write32(msg.cpuId);

    std::memcpy(p, msg.programName, sizeof(msg.programName));
    p += sizeof(msg.programName);

    std::memcpy(p, msg.hostInfo, sizeof(msg.hostInfo));
    p += sizeof(msg.hostInfo);
}

static inline double double_from_be(const uint8_t in[8]) {
    uint64_t tmp;
    std::memcpy(&tmp, in, sizeof(tmp));
    tmp = be64toh(tmp);
    double val;
    std::memcpy(&val, &tmp, sizeof(val));
    return val;
}

inline void Deserialize(WelcomeMessage& msg, const uint8_t* buffer, std::size_t bufferSize)
{
    const uint8_t* p = buffer;

    auto read64 = [&](uint64_t& v) {
        if (p + 8 > buffer + bufferSize) return false;
        uint64_t tmp;
        std::memcpy(&tmp, p, 8);
        p += 8;
        v = be64toh(tmp);
        return true;
    };

    auto read32 = [&](uint32_t& v) {
        if (p + 4 > buffer + bufferSize) return false;
        uint32_t tmp;
        std::memcpy(&tmp, p, 4);
        p += 4;
        v = be32toh(tmp);
        return true;
    };

    auto read8 = [&](uint8_t& v) {
        if (p + 1 > buffer + bufferSize) return false;
        v = *p++;
        return true;
    };

    // Read double
    if (p + 8 > buffer + bufferSize) return;
    msg.timerMul = double_from_be(p);
    p += 8;

    uint64_t tmp64;
    read64(tmp64); msg.initBegin = static_cast<int64_t>(tmp64);
    read64(tmp64); msg.initEnd = static_cast<int64_t>(tmp64);
    read64(msg.resolution);
    read64(msg.epoch);
    read64(msg.exectime);
    read64(msg.pid);
    read64(tmp64); msg.samplingPeriod = static_cast<int64_t>(tmp64);

    read8(msg.flags);
    read8(msg.cpuArch);

    if (p + sizeof(msg.cpuManufacturer) > buffer + bufferSize) return;
    std::memcpy(msg.cpuManufacturer, p, sizeof(msg.cpuManufacturer));
    p += sizeof(msg.cpuManufacturer);

    read32(msg.cpuId);

    if (p + sizeof(msg.programName) > buffer + bufferSize) return;
    std::memcpy(msg.programName, p, sizeof(msg.programName));
    p += sizeof(msg.programName);

    if (p + sizeof(msg.hostInfo) > buffer + bufferSize) return;
    std::memcpy(msg.hostInfo, p, sizeof(msg.hostInfo));
    p += sizeof(msg.hostInfo);
}

enum { WelcomeMessageSize = sizeof( WelcomeMessage ) };


struct OnDemandPayloadMessage
{
    uint64_t frames;
    uint64_t currentTime;
};

enum { OnDemandPayloadMessageSize = sizeof( OnDemandPayloadMessage ) };


struct BroadcastMessage
{
    uint16_t broadcastVersion;
    uint16_t listenPort;
    uint32_t protocolVersion;
    uint64_t pid;
    int32_t activeTime;        // in seconds
    char programName[WelcomeMessageProgramNameSize];
};

struct BroadcastMessage_v2
{
    uint16_t broadcastVersion;
    uint16_t listenPort;
    uint32_t protocolVersion;
    int32_t activeTime;
    char programName[WelcomeMessageProgramNameSize];
};

struct BroadcastMessage_v1
{
    uint32_t broadcastVersion;
    uint32_t protocolVersion;
    uint32_t listenPort;
    uint32_t activeTime;
    char programName[WelcomeMessageProgramNameSize];
};

struct BroadcastMessage_v0
{
    uint32_t broadcastVersion;
    uint32_t protocolVersion;
    uint32_t activeTime;
    char programName[WelcomeMessageProgramNameSize];
};

enum { BroadcastMessageSize = sizeof( BroadcastMessage ) };
enum { BroadcastMessageSize_v2 = sizeof( BroadcastMessage_v2 ) };
enum { BroadcastMessageSize_v1 = sizeof( BroadcastMessage_v1 ) };
enum { BroadcastMessageSize_v0 = sizeof( BroadcastMessage_v0 ) };

#pragma pack( pop )

}

#endif
