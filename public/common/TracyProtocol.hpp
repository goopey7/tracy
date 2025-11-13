#ifndef __TRACYPROTOCOL_HPP__
#define __TRACYPROTOCOL_HPP__

#include "TracyEndian.hpp"
#include <limits>
#include <stdint.h>

namespace tracy
{

constexpr unsigned Lz4CompressBound( unsigned isize ) { return isize + ( isize / 255 ) + 16; }

enum : uint32_t { ProtocolVersion = 77 };
enum : uint16_t { BroadcastVersion = 4 };

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

	void convert_endian()
	{
		ptr = ::convert_endian(ptr);
		extra = ::convert_endian(extra);
	}
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

    void convert_endian()
    {
        timerMul = ::convert_endian( timerMul );
        initBegin = ::convert_endian( initBegin );
        initEnd = ::convert_endian( initEnd );
        resolution = ::convert_endian( resolution );
        epoch = ::convert_endian( epoch );
        exectime = ::convert_endian( exectime );
        pid = ::convert_endian( pid );
        samplingPeriod = ::convert_endian( samplingPeriod );
        flags = ::convert_endian( flags );
        cpuArch = ::convert_endian( cpuArch );
        cpuId = ::convert_endian( cpuId );
    }
};

enum { WelcomeMessageSize = sizeof( WelcomeMessage ) };


struct OnDemandPayloadMessage
{
    uint64_t frames;
    uint64_t currentTime;
};

enum { OnDemandPayloadMessageSize = sizeof( OnDemandPayloadMessage ) };

enum class NetworkByteOrder : uint8_t
{
    BigEndian,
    LittleEndian
};

struct BroadcastMessage
{
    NetworkByteOrder byteOrder;
    uint16_t broadcastVersion;
    uint16_t listenPort;
    uint32_t protocolVersion;
    uint64_t pid;
    int32_t activeTime;        // in seconds
    char programName[WelcomeMessageProgramNameSize];

    void convert_endian()
    {
        ::convert_endian( broadcastVersion );
        ::convert_endian( listenPort );
        ::convert_endian( protocolVersion );
        ::convert_endian( pid );
        ::convert_endian( activeTime );
    }
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
