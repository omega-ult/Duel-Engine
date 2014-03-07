//  [7/16/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelSocket.h"
#include "DuelException.h"
#ifdef DUEL_PLATFORM_WINDOWS
#include <winsock.h>
#endif // DUEL_PLATFORM_WINDOWS


namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DSocket, DObject);


	DSocket::DSocket() :
		mHandle(INVALID_SOCKET)
	{

	}

	DSocket::~DSocket()
	{
		close();
	}

	sockaddr_in DSocket::parseAddress( const DString& addr, uint16 port )
	{
		sockaddr_in ret;
		std::memset(&ret, 0, sizeof(ret));

		if (addr.empty())
		{
			ret.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else
		{
			ret.sin_addr.s_addr = inet_addr(addr.c_str());
		}

		if (INADDR_NONE == ret.sin_addr.s_addr)
		{
			hostent* pHostEnt = gethostbyname(addr.c_str());
			if (pHostEnt != NULL)
			{
				std::memcpy(&ret.sin_addr.s_addr,
					pHostEnt->h_addr_list[0], pHostEnt->h_length);
			}
			else
			{
				DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
					"Invalid address: " + addr,
					"Duel::DSocket::parseAddress");	
			}
		}

		ret.sin_family = AF_INET;
		ret.sin_port = htons(port);

		return ret;
	}

	Duel::DString DSocket::getAddress( const sockaddr_in& addr, uint16& outPort )
	{
		outPort = ntohs(addr.sin_port);
		return DString(inet_ntoa(addr.sin_addr));
	}

	in_addr DSocket::getHostAddress()
	{
		// FROM KLAYGE 4.0
		in_addr	ret;
		memset(&ret, 0, sizeof(in_addr));
		
		char host[256];
		if (0 == gethostname(host, sizeof(host)))
		{
			hostent* pHostEnt = gethostbyname(host);
#ifdef DUEL_PLATFORM_WINDOWS
			std::memcpy(&ret.S_un.S_addr, pHostEnt->h_addr_list[0], pHostEnt->h_length);

#elif // defined (DUEL_PLATFORM_LINUX)
			std::memcpy(&ret.s_addr, pHostEnt->h_addr_list[0], pHostEnt->h_length);

#endif // DUEL_PLATFORM_WINDOWS

		}
		return ret;
	}

	void DSocket::create( int32 socketType /*= SOCK_STREAM*/, int32 protocolType /*= 0*/, int32 addressFormat /*= PF_INET*/ )
	{
		close();
		mHandle = socket(addressFormat, socketType, protocolType);
		if (mHandle == INVALID_SOCKET)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidStatus,
				"Unable to create socket",
				"Duel::DSocket::create");
		}
	}

	void DSocket::close()
	{
		if (mHandle != INVALID_SOCKET)
		{
#ifdef DUEL_PLATFORM_WINDOWS
			closesocket(mHandle);
#else
			DUEL_EXCEPT_BRIEF(DException::ET_NotImplemented,
				"Not implemented",
				"Duel::DSocket");
#endif
			mHandle = INVALID_SOCKET;
		}
	}

	void DSocket::accept( DSocket& connectedSocket )
	{
		connectedSocket.close();

		connectedSocket.mHandle = ::accept(mHandle, NULL, NULL);
	}

	void DSocket::accept( DSocket& connectedSocket, sockaddr_in& addr )
	{
		connectedSocket.close();
		socklen_t len(sizeof(addr));
		connectedSocket.mHandle = ::accept(mHandle, (sockaddr*)&addr, &len);
	}

	void DSocket::bind( const sockaddr_in& addr )
	{
		assert(mHandle != INVALID_SOCKET);
		if (::bind(mHandle, (sockaddr*)(&addr),
			sizeof(addr)) == SOCKET_ERROR)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Unable to bind socket",
				"Duel::DSocket::bind");
		}
	}

	void DSocket::connect( const sockaddr_in& addr )
	{
		assert(mHandle != INVALID_SOCKET);
		if (::connect(mHandle, (sockaddr*)(&addr),
			sizeof(addr)) == SOCKET_ERROR)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Unable to connect socket",
				"Duel::DSocket::connect");
		}
	}

	void DSocket::setIOControl( long command, uint32* argument )
	{
		assert(mHandle != INVALID_SOCKET);
#ifdef DUEL_PLATFORM_WINDOWS
		if (::ioctlsocket(mHandle, command,
			(u_long*)argument) == SOCKET_ERROR)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Unable to setIO control for this socket",
				"Duel::DSocket::setIOControl");
		}
#else
		DUEL_EXCEPT_BRIEF(DException::ET_NotImplemented,
			"Not implemented",
			"Duel::DSocket");
#endif // DUEL_PLATFORM_WINDOWS

	}

	void DSocket::listen( int32 connectionBacklog /*= 5*/ )
	{
		assert(mHandle != INVALID_SOCKET);
		if (::listen(mHandle, connectionBacklog) == SOCKET_ERROR)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Unable to listen socket",
				"Duel::DSocket::listen");
		}
	}

	Duel::int32 DSocket::receive( void* buf, uint32 length, int32 flag /*= 0*/ )
	{
		assert(mHandle != INVALID_SOCKET);
		return	::recv(mHandle, static_cast<char*>(buf), length, flag);
	}

	Duel::int32 DSocket::send( void const* buf, uint32 length, int32 flag /*= 0*/ )
	{
		assert(mHandle != INVALID_SOCKET);
		return ::send(mHandle, static_cast<const char*>(buf), length, flag);
	}

	Duel::int32 DSocket::receiveFrom( sockaddr_in& src, void* buf, uint32 length, int32 flag /*= 0*/ )
	{
		assert(mHandle != INVALID_SOCKET);
		socklen_t fromLen(sizeof(src));
		return ::recvfrom(mHandle, static_cast<char*>(buf), length, flag,
			(sockaddr*)(&src), &fromLen);

	}

	Duel::int32 DSocket::sendTo( const sockaddr_in& dest, void const* buf, uint32 length, int32 flag /*= 0*/ )
	{
		assert(mHandle != INVALID_SOCKET);
		return ::sendto(mHandle, static_cast<const char*>(buf), length, flag, (const sockaddr*)(&dest), sizeof(dest));
	}

	void DSocket::shutdown( ShutdownMode mode )
	{
		assert(mHandle != INVALID_SOCKET);
		if (::shutdown(mHandle, mode) == SOCKET_ERROR)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Unable to shutdown socket",
				"Duel::DSocket::shutdown");
		}
	}

	void DSocket::getPeerName( sockaddr_in& addr, socklen_t& len )
	{
		assert(mHandle != INVALID_SOCKET);
		if (::getpeername(mHandle, (sockaddr*)&addr, &len) == SOCKET_ERROR)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Unable to get peer name",
				"Duel::DSocket::getPeerName");
		}
	}

	void DSocket::getSockName( sockaddr_in& addr, socklen_t& len )
	{
		assert(mHandle != INVALID_SOCKET);
		if (::getsockname(mHandle, (sockaddr*)&addr, &len) == SOCKET_ERROR)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Unable to get sock name",
				"Duel::DSocket::getSockName");
		}
	}

	void DSocket::setSockOption( int32 optName, void const* optVal, socklen_t optLen, int32 level /*= SOL_SOCKET*/ )
	{
		assert(mHandle != INVALID_SOCKET);
		if (::setsockopt(mHandle, level, optName, static_cast<char const*>(optVal), optLen) == SOCKET_ERROR)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Unable to set sock option",
				"Duel::DSocket::setSockOption");
		}
	}

	void DSocket::getSockOption( int32 optName, void* optVal, socklen_t& optLen, int32 level /*= SOL_SOCKET*/ )
	{
		assert(mHandle != INVALID_SOCKET);
		if (::getsockopt(mHandle, level, optName, static_cast<char*>(optVal), &optLen) == SOCKET_ERROR)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
				"Unable to get sock option",
				"Duel::DSocket::getSockOption");
		}
	}

	void DSocket::setEnableBlock( bool flag )
	{
		uint32	on(flag);
		setIOControl(FIONBIO, &on);
	}

	void DSocket::setTimeOut( uint32 microSec )
	{
		timeval	timeout;
		timeout.tv_sec	= microSec / 1000;
		timeout.tv_usec = microSec % 1000;
		setSockOption(SO_RCVTIMEO, &timeout, sizeof(timeout));
		setSockOption(SO_SNDTIMEO, &timeout, sizeof(timeout));
	}

	Duel::uint32 DSocket::getTimeOut()
	{
		timeval timeOut;
		socklen_t len(sizeof(timeOut));
		getSockOption(SO_RCVTIMEO, &timeOut, len);
		return timeOut.tv_sec * 1000 + timeOut.tv_usec;
	}

}