#include "FuncDef.h"

#define SERVER_PORT	8554
#define BUF_SIZE	20000

int main(int argc, char* argv[])
{
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == listen_sock)
	{
		sksock::err_display("socket()");
		return 1;
	}

	SOCKADDR_IN server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY PC 랜카드 중 사용가능한 랜카드의 IP 주소를 사용하라
	server_addr.sin_port = htons(SERVER_PORT);
	retval = bind(listen_sock, (SOCKADDR*)&server_addr, sizeof(server_addr));
	if (SOCKET_ERROR == retval)
	{
		sksock::err_display("bind()");
		return 1;
	}

	retval = listen(listen_sock, SOMAXCONN);
	if (SOCKET_ERROR == retval)
	{
		sksock::err_display("listen()");
		return 1;
	}

	SOCKET client_sock;
	SOCKADDR_IN client_addr;
	int addr_len;
	char buf[BUF_SIZE + 1];

	while (1)
	{
		addr_len = sizeof(client_addr);
		client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &addr_len);
		if (INVALID_SOCKET == client_sock)
		{
			sksock::err_display("accept");
			break;
		}

		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		retval = recv(client_sock, buf, BUF_SIZE, 0);
		if (retval == SOCKET_ERROR) {
			sksock::err_display("recv");
			break;
		}

		buf[retval] = '\0';
		printf("TCP/s:%d %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buf);
	}

	closesocket(listen_sock);

	WSACleanup();
}