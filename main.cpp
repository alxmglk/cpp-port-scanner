#include <stdio.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <vector>
#include <cstring>

using namespace std;

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
}

struct scandata {
	int start;
	int end;
	char host[255];
};

DWORD WINAPI scan(LPVOID param)
{
	scandata* data = (scandata*)param;

	for (int i = data->start; i <= data->end; ++i)
	{
		scan(data->host, i);
	}

	return 0;
}

vector<HANDLE> handles;

void scan(const char* host, const int start, const int end)
{
	DWORD threadId;
	scandata data;
	data.start = start;
	data.end = end;
	strcpy_s(data.host, host);	

	HANDLE handle = CreateThread(NULL, 0, scan, &data, 0, &threadId);

	handles.push_back(handle);
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

	const char* host = "192.168.1.11";

	scan(host, 80); // scan single port
	scan(host, 1433, 1440); // scan ports in range
	scan(host, 15555, 15565);

	// Wait for all threads (let them finish execution)
	WaitForMultipleObjects(handles.size(), &handles[0], true, INFINITE); // &handles[0] - returns pointer to HANDLE* (array of handles)

	// Close all handles to release resources
	for (int i = 0; i < handles.size(); ++i)
	{
		CloseHandle(handles[i]);
	}

	// Release resources used by socket library.
	WSACleanup();

	system("pause"); // приостанавливает выполнение программы до нажатия клавиши
	return 0;
}