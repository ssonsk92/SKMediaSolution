#include "WinSockInitializer.h"
#include <WinSock2.h>
#include <cstdio>
#include <cstdlib>

namespace skson
{
	namespace net
	{
		const WinSockInitializer winSockInitalizer;

		WinSockInitializer::WinSockInitializer()
		{
			WSADATA wsa;
			if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
			{
				printf("WSAStartup Error!\n");
				std::abort();
			}
		}

		WinSockInitializer::~WinSockInitializer()
		{
			WSACleanup();
		}

		void WinSockInitialize()
		{
			WSADATA wsa;
			if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
			{
				throw wsa;
			}
		}
	}
}
