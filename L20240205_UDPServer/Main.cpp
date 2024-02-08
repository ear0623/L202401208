#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <winsock2.h>

using namespace std;

#pragma comment(lib,"ws2_32")

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET ServerSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	SOCKADDR_IN ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_family = AF_INET;
	ServerSockAddr.sin_addr.s_addr = INADDR_ANY;
	ServerSockAddr.sin_port = htons(5001);
	int ServerSockAddrLength = sizeof(ServerSockAddr);

	bind(ServerSocket, (SOCKADDR*)&ServerSockAddr, sizeof(ServerSockAddr));

	char Message[1024] = { 0, };

	SOCKADDR_IN ClientSockAddr;
	memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
	int ClientSocketAddrLength = sizeof(ClientSockAddr);

	int RecvLength = recvfrom(ServerSocket, Message, sizeof(Message), 0, (SOCKADDR*)&ServerSockAddr, &ServerSockAddrLength);

	cout << "Recv : " << Message << endl;

	FILE* InputFile = fopen("1.png", "rb");

	while (1)
	{
		char Buffer[1024] = { 0, };
		int ReadSize = (int)fread(Buffer, sizeof(char), sizeof(Buffer), InputFile);
		if (ReadSize == 0)
		{
			int SendLength = sendto(ServerSocket, Buffer, 0, 0, (SOCKADDR*)&ClientSockAddr, sizeof(ClientSocketAddrLength));

			break;
		}

		int SendLength = sendto(ServerSocket, Buffer, RecvLength, 0, (SOCKADDR*)&ClientSockAddr, sizeof(ClientSocketAddrLength));

	}
	closesocket(ServerSocket);
	fclose(InputFile);

	WSACleanup();

	return 0;
}