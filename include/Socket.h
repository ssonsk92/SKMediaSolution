#pragma once
#include <WinSock2.h>
#include <string>

namespace skson
{
	namespace net
	{
		enum class AddressFamily
		{
			IPv4,
		};

		enum class SocketType
		{
			TCP,
			UDP
		};

		int GetWinSockAddressFaily(AddressFamily eAdressFamily);
		int GetWinSockSocketType(SocketType eSockType);

		class Socket
		{
		public:
			Socket(AddressFamily eAdressFamily, SocketType eSockType);
			~Socket();

			bool Bind(AddressFamily eAdressFamily, uint16_t nPort, const std::string sAddress = std::string());
			bool Listen(uint32_t nMaxSocket);

		protected:
			void err_display(char* msg);

		private:
			SOCKET m_sock;
		};
	}
}

