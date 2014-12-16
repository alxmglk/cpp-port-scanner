#include <stdio.h>
#include <WS2tcpip.h>
#include <windows.h>

BOOL isPortOpen(const char* host, const u_short port)
{
	SOCKET socketId;

	sockaddr_in address;
	
	address.sin_family = AF_INET; // AF_INET == IPv4
	address.sin_port = htons(port); // convert to "network" representation
	inet_pton(AF_INET, host, &(address.sin_addr)); // convert to "network" representation (in this case from "string" to "ulong") and store it in address.sin_addr

	socketId = socket(AF_INET, SOCK_STREAM, 0); // try to create socket
	if (socketId < 0)
	{
		return FALSE; // can't create socket
	}

	if (connect(socketId, (sockaddr*)&address, sizeof(address)) < 0) // trying connect to socket
	{
		return FALSE; // can't connect
	}

	closesocket(socketId);

	return TRUE; // port is open
}

void scan(const char* host, const int port)
{
	BOOL open = isPortOpen(host, port);
	if (open)
	{
		printf("%d: OPEN\n", port);
	}
	else
	{
		printf("%d: CLOSED\n", port);
	}
}

void scan(const char* host, const int start, const int end)
{
	for (int i = start; i <= end; ++i)
	{
		scan(host, i);
	}
}

int main()
{
	// Initialize socket library.
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		printf("WSAStartup failed");
		return 1;
	}

	const char* host = "127.0.0.1";

	scan(host, 80); // scan single port
	scan(host, 1433, 1440); // scan ports in range

	// Release resources used by socket library.
	WSACleanup();

	system("pause"); // приостанавливает выполнение программы до нажатия клавиши
	return 0;
}