//  [7/16/2013 OMEGA] created

#ifndef _DUELSOCKET_H_
#define _DUELSOCKET_H_

#include "DuelCommon.h"
#ifdef DUEL_PLATFORM_WINDOWS
#include <winsock.h>
typedef	int socklen_t;
#define SOCKETHANDLE	SOCKET
#else	// defined (DUEL_PLATROM_LINUX)
// 
// #define INVALID_SOCKET (~0)
// #define SOCKET_ERROR (-1)
#define SOCKETHANDLE	int

#endif // DUEL_PLATFORM_WINDOWS


namespace Duel
{
	// todo : 你懂的.
	// from KLAYGE 4.0 版权为原作者所有
	class DUEL_API DSocket : public DObject
	{
	DUEL_DECLARE_RTTI(DSocket)
	public:
		DSocket();
		virtual ~DSocket();

		void	create(int32 socketType = SOCK_STREAM, int32 protocolType = 0, int32 addressFormat = PF_INET);
		void	close();
		
		void	accept(DSocket& connectedSocket);
		void	accept(DSocket& connectedSocket, sockaddr_in& addr);
		void	bind(const sockaddr_in& addr);

		void	connect(const sockaddr_in& addr);

		// use standard command word.
		void	setIOControl(long command, uint32* argument);
		void	listen(int32 connectionBacklog = 5);

		int32	receive(void* buf, uint32 length, int32 flag = 0);
		int32	send(void const* buf, uint32 length, int32 flag = 0);

		int32	receiveFrom(sockaddr_in& src, void* buf, uint32 length, int32 flag = 0);
		int32	sendTo(const sockaddr_in& dest, void const* buf, uint32 length, int32 flag = 0);

		enum ShutdownMode
		{
			SM_Receive	= 0,
			SM_Send		= 1,
			SM_Both		= 2
		};
		// force close
		void	shutdown(ShutdownMode mode);
		
		void	getPeerName(sockaddr_in& addr, socklen_t& len);
		void	getSockName(sockaddr_in& addr, socklen_t& len);

		void	setSockOption(int32 optName, void const* optVal,
			socklen_t optLen, int32 level = SOL_SOCKET);
		void	getSockOption(int32 optName, void* optVal,
			socklen_t& optLen, int32 level = SOL_SOCKET);

		void	setEnableBlock(bool flag);

		void	setTimeOut(uint32 microSec);
		uint32	getTimeOut();

		static	sockaddr_in	parseAddress(const DString& addr, uint16 port);
		static	DString		getAddress(const sockaddr_in& addr, uint16& outPort);
		static	in_addr		getHostAddress();

	protected:
		SOCKETHANDLE	mHandle;
	};
}

#endif