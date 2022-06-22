#include "Socket.h"

#include <cstdio>

namespace skson
{
	namespace net
	{
		int GetWinSockAddressFaily(AddressFamily eAdressFamily)
		{
			int nAddressFamily = -1;

			switch (eAdressFamily)
			{
			case AddressFamily::IPv4:
				nAddressFamily = AF_INET;
				break;
			default:
				printf("Unknown Address Family! Forced IPv4 Setting!\n");
				nAddressFamily = AF_INET;
				break;
			}

			return nAddressFamily;
		}

		int GetWinSockSocketType(SocketType eSockType)
		{
			int nSockType = -1;

			switch (eSockType)
			{
			case SocketType::TCP:
				nSockType = SOCK_STREAM;
				break;
			case SocketType::UDP:
				nSockType = SOCK_DGRAM;
				break;
			default:
				printf("Unknown Socket Type! Forced TCP Socket Setting!\n");
				nSockType = SOCK_STREAM;
				break;
			}

			return 0;
		}

		Socket::Socket(AddressFamily eAdressFamily, SocketType eSockType)
			: m_sock(INVALID_SOCKET)
		{
			m_sock = socket(GetWinSockAddressFaily(eAdressFamily), GetWinSockSocketType(eSockType), 0);
			if (INVALID_SOCKET == m_sock)
			{
				err_display("socket()");
			}
		}

		Socket::~Socket()
		{
			closesocket(m_sock);
		}

		bool Socket::Bind(AddressFamily eAdressFamily, uint16_t nPort, const std::string sAddress)
		{
			SOCKADDR_IN serverAddr;
			memset(&serverAddr, 0, sizeof(serverAddr));
			serverAddr.sin_family = GetWinSockAddressFaily(eAdressFamily);
			serverAddr.sin_port = htons(nPort);

			if (sAddress.empty())
			{
				serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			}
			else
			{
				serverAddr.sin_addr.s_addr = inet_addr(sAddress.c_str());
			}

			int res = bind(m_sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
			if (SOCKET_ERROR == res)
			{
				err_display("bind()");
				return false;
			}

			return true;
		}

		bool Socket::Listen(uint32_t nMaxSocket)
		{
			if (SOMAXCONN < nMaxSocket)
			{
				printf("Maximum connections exceeded\n");
				return false;
			}
			//listen에 SOMAXCONN 넘으면 에러 뿜는지 확인해보기
			int res = listen(m_sock, nMaxSocket);

			return true;
		}

		void Socket::err_display(char* msg)
		{
			LPVOID lpMsgBuf;
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf, 0, NULL);
			printf("[%s] %s", msg, (char*)lpMsgBuf);
			LocalFree(lpMsgBuf);
		}
	}
}