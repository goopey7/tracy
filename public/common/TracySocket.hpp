#ifndef __TRACYSOCKET_HPP__
#define __TRACYSOCKET_HPP__

#include "TracyEndian.hpp"
#include <atomic>
#include <stddef.h>
#include <stdint.h>

#ifdef __wii__
#  include <network.h>
#endif

struct addrinfo;
struct sockaddr;

namespace tracy
{

#ifdef _WIN32
void InitWinSock();
#elif __wii__
bool InitWiiNetwork();
#endif

class Socket
{
public:
    Socket();
    Socket( int sock );
    ~Socket();

    bool Connect( const char* addr, uint16_t port );
    bool ConnectBlocking( const char* addr, uint16_t port );
    void Close();

    int Send( const void* buf, int len );

    template<TrivialInteger T>
    int Send( T value )
    {
        auto network_val = convert_endian( value );
        return Send( &network_val, sizeof( network_val ) );
    }

    template<typename T>
        requires std::is_enum_v<T>
    int Send( T value )
    {
        auto network_val = convert_endian( value );
        return Send( &network_val, sizeof( network_val ) );
    }

    template<FloatingPoint T>
    int Send( T value )
    {
        auto network_val = convert_endian( value );
        return Send( &network_val, sizeof( network_val ) );
    }

    template<TrivialStruct T>
    int Send( const T& value )
    {
        auto network_val = value;
        convert_endian( network_val );
        return Send( &network_val, sizeof( network_val ) );
    }

    int GetSendBufSize();

    int ReadUpTo( void* buf, int len );
    bool Read( void* buf, int len, int timeout );

    template<typename ShouldExit>
    bool Read( void* buf, int len, int timeout, ShouldExit exitCb )
    {
        auto cbuf = (char*)buf;
        while( len > 0 )
        {
            if( exitCb() ) return false;
            if( !ReadImpl( cbuf, len, timeout ) ) return false;
        }
        return true;
    }

    template<TrivialInteger T, typename ShouldExit>
    bool Read( T& value, int timeout, ShouldExit exitCb )
    {
        int len = sizeof( value );
        T network_val;
        char* cBuf = (char*)&network_val;
        while( len > 0 )
        {
            if( exitCb() ) return false;
            if( !ReadImpl( cBuf, len, timeout ) ) return false;
        }
        value = convert_endian( network_val );
        return true;
    }

    template<typename T, typename ShouldExit>
        requires std::is_enum_v<T>
    bool Read( T& value, int timeout, ShouldExit exitCb )
    {
        std::underlying_type_t<T> network_val;
        if( !Read( &network_val, sizeof( network_val ), timeout, exitCb ) )
        {
            return false;
        }
        value = static_cast<T>( convert_endian( network_val ) );
        return true;
    }

    template<FloatingPoint T, typename ShouldExit>
    bool Read( T& value, int timeout, ShouldExit exitCb )
    {
        int len = sizeof( value );
        T network_val;
        char* cBuf = (char*)&network_val;
        while( len > 0 )
        {
            if( exitCb() ) return false;
            if( !ReadImpl( cBuf, len, timeout ) ) return false;
        }
        value = convert_endian( network_val );
        return true;
    }

    template<TrivialStruct T, typename ShouldExit>
    bool Read( T& value, int timeout, ShouldExit exitCb )
    {
        if constexpr( std::endian::native == network_byteorder() )
        {
            return Read( &value, sizeof( value ), timeout );
        }
        else
        {
            int len = sizeof( value );
            char* cBuf = (char*)&value;
            while( len > 0 )
            {
                if( exitCb() ) return false;
                if( !ReadImpl( cBuf, len, timeout ) ) return false;
            }
            convert_endian( value );
            return true;
        }
    }

    bool ReadRaw( void* buf, int len, int timeout );

    template<TrivialInteger T>
    bool Read( T& value, int timeout )
    {
        T network_val;
        if( !Read( &network_val, sizeof( network_val ), timeout ) )
        {
            return false;
        }
        value = convert_endian( network_val );
        return true;
    }

    template<typename T>
        requires std::is_enum_v<T>
    bool Read( T& value, int timeout )
    {
        std::underlying_type_t<T> network_val;
        if( !Read( &network_val, sizeof( network_val ), timeout ) )
        {
            return false;
        }
        value = static_cast<T>( convert_endian( network_val ) );
        return true;
    }

    template<FloatingPoint T>
    bool Read( T& value, int timeout )
    {
        T network_val;
        if( !Read( &network_val, sizeof( network_val ), timeout ) )
        {
            return false;
        }
        value = convert_endian( network_val );
        return true;
    }

    template<TrivialStruct T>
    bool Read( T& value, int timeout )
    {
        if constexpr( std::endian::native == network_byteorder() )
        {
            return Read( &value, sizeof( value ), timeout );
        }
        else
        {
            T network_val;
            if( !Read( &network_val, sizeof( network_val ), timeout ) )
            {
                return false;
            }
            value = network_val;
            convert_endian( value );
            return true;
        }
    }

    bool HasData();
    bool IsValid() const;

    Socket( const Socket& ) = delete;
    Socket( Socket&& ) = delete;
    Socket& operator=( const Socket& ) = delete;
    Socket& operator=( Socket&& ) = delete;

private:
    int RecvBuffered( void* buf, int len, int timeout );
    int Recv( void* buf, int len, int timeout );

    bool ReadImpl( char*& buf, int& len, int timeout );


    char* m_buf;
    char* m_bufPtr;
    std::atomic<int> m_sock;
    int m_bufLeft;

#ifdef __wii__
    struct sockaddr_in m_serverAddr;
    int m_connSock;
#else
    struct addrinfo* m_res;
    struct addrinfo* m_ptr;
    int m_connSock;
#endif
};

class ListenSocket
{
public:
    ListenSocket();
    ~ListenSocket();

    bool Listen( uint16_t port, int backlog );
    Socket* Accept();
    void Close();

    ListenSocket( const ListenSocket& ) = delete;
    ListenSocket( ListenSocket&& ) = delete;
    ListenSocket& operator=( const ListenSocket& ) = delete;
    ListenSocket& operator=( ListenSocket&& ) = delete;

private:
    int m_sock;
};

class UdpBroadcast
{
public:
    UdpBroadcast();
    ~UdpBroadcast();

    bool Open( const char* addr, uint16_t port );
    void Close();

    int Send( uint16_t port, const void* data, int len );

    template<TrivialInteger T>
    int Send( uint16_t port, T value )
    {
        auto network_val = convert_endian( value );
        return Send( port, &network_val, sizeof( network_val ) );
    }

    template<typename T>
        requires std::is_enum_v<T>
    int Send( uint16_t port, T value )
    {
        auto network_val = convert_endian( value );
        return Send( port, &network_val, sizeof( network_val ) );
    }

    template<FloatingPoint T>
    int Send( uint16_t port, T value )
    {
        auto network_val = convert_endian( value );
        return Send( port, &network_val, sizeof( network_val ) );
    }

    template<TrivialStruct T>
    int Send( const uint16_t port, T& value )
    {
        auto network_val = value;
        convert_endian( network_val );
        return Send( port, &network_val, sizeof( network_val ) );
    }

    UdpBroadcast( const UdpBroadcast& ) = delete;
    UdpBroadcast( UdpBroadcast&& ) = delete;
    UdpBroadcast& operator=( const UdpBroadcast& ) = delete;
    UdpBroadcast& operator=( UdpBroadcast&& ) = delete;

private:
    int m_sock;
    uint32_t m_addr;
};

class IpAddress
{
public:
    IpAddress();
    ~IpAddress();

    void Set( const struct sockaddr& addr );

    uint32_t GetNumber() const { return m_number; }
    const char* GetText() const { return m_text; }

    IpAddress( const IpAddress& ) = delete;
    IpAddress( IpAddress&& ) = delete;
    IpAddress& operator=( const IpAddress& ) = delete;
    IpAddress& operator=( IpAddress&& ) = delete;

private:
    uint32_t m_number;
    char m_text[17];
};

class UdpListen
{
public:
    UdpListen();
    ~UdpListen();

    bool Listen( uint16_t port );
    void Close();

    const char* Read( size_t& len, IpAddress& addr, int timeout );

    UdpListen( const UdpListen& ) = delete;
    UdpListen( UdpListen&& ) = delete;
    UdpListen& operator=( const UdpListen& ) = delete;
    UdpListen& operator=( UdpListen&& ) = delete;

private:
    int m_sock;
};

}

#endif
