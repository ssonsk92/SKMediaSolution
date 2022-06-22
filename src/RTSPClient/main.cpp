#include "FuncDef.h"
#include "Socket.h"

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (0 < left)
	{
		received = recv(s, ptr, left, flags);
		if (SOCKET_ERROR == received)
			return SOCKET_ERROR;
		else if (0 == received)
			break;

		left -= received;
		ptr += received;
	}
}

int main(int argc, char* argv[])
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sock)
	{
		sksock::err_display("socket()");
		WSACleanup();
		return 1;
	}

	HOSTENT* h = gethostbyname("wowzaec2demo.streamlock.net");
	IN_ADDR inAddr;
	memcpy(&inAddr, h->h_addr, h->h_length);

	SOCKADDR_IN serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = h->h_addrtype;
	serverAddr.sin_addr.s_addr = inAddr.s_addr;
	serverAddr.sin_port = htons(554);

	int retval = connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (SOCKET_ERROR == retval)
	{
		sksock::err_display("connect()");
		WSACleanup();
		return 1;
	}

	//char buf[1024] = { "OPTIONS rtsp://wowzaec2demo.streamlock.net:554/vod/mp4:BigBuckBunny_115k.mp4 RTSP/1.0" };
	char* const buf =
		"OPTIONS rtsp://wowzaec2demo.streamlock.net:554/vod/mp4:BigBuckBunny_115k.mp4 RTSP/1.0 CSeq: 1 User-Agent: DXMediaPlayedr";
	
	while (1)
	{
		retval = send(sock, buf, strlen(buf), 0);
		if (SOCKET_ERROR == retval)
		{
			sksock::err_display("connect()");
			break;
		}

		retval = recvn(sock, buf, retval, 0);
		if (SOCKET_ERROR == retval)
		{
			sksock::err_display("recv()");
			break;
		}
		else if (0 == retval)
			break;

		buf[retval] = '\0';
		printf("Data recv [%s]\n", buf);
	}

	WSACleanup();

	return 0;
}