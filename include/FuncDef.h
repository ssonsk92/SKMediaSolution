#pragma once
#include <WinSock2.h>
#include <cstdio>

namespace sksock
{
	inline void err_display(char* msg)
	{
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL);
		printf("[%s] %s", msg, (char*)lpMsgBuf);
		LocalFree(lpMsgBuf);
	}
}
