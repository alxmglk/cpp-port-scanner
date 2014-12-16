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

int main()
{
	// Initialize socket library.
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		printf("WSAStartup failed");
		return 1;
	}

	for (int i = 0; i < 100; ++i)
	{
		BOOL open = isPortOpen("127.0.0.1", i);
		if (open)
		{
			printf("%s: OPEN", i);
		}
		else
		{
			printf("%s: CLOSED", i);
		}
	}

	// Release resources used by socket library.
	WSACleanup();

	system("pause"); // приостанавливает выполнение программы до нажатия клавиши
	return 0;
}