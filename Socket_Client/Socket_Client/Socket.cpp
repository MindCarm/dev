#include "stdafx.h"
#include "Socket.h"



Socket::Socket(const std::string strIP, u_short nPort, LogMaker* pLog)
	:m_pLog(pLog)
{
	int initialResult = WSAStartup(MAKEWORD(2, 2), &m_WsaData);
	assert(initialResult == 0);

	
	m_hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	assert(m_hSocket != INVALID_SOCKET);
	
	DWORD recvTimeout = 5000;
	setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&recvTimeout, sizeof(recvTimeout));

	m_tAddr = {};
	m_tAddr.sin_family = AF_INET;
	m_tAddr.sin_port = htons(nPort);
	m_tAddr.sin_addr.s_addr = inet_addr(strIP.c_str());
}

Socket::~Socket()
{
	closesocket(m_hSocket);
	
	WSACleanup();
}

bool Socket::Connect()
{
	

	if (connect(m_hSocket, (SOCKADDR*)&m_tAddr, sizeof(m_tAddr)) == SOCKET_ERROR)
	{
		int result = WSAGetLastError();
 		m_pLog->WriteLog(LogMaker::eERROR, "TCP 연결실패");
		std::string errcode = "Connect 에러코드 : " + std::to_string(result);
		m_pLog->WriteLog(LogMaker::eERROR, errcode);
		return false;
	}
	

	return true;
}

bool Socket::Communication(const char * sendMsg, char(&recvMsg)[PACKET_SIZE], int nFlag)
{
	if (send(m_hSocket, sendMsg, strlen(sendMsg) + 1, nFlag) == SOCKET_ERROR)
	{
		int result = WSAGetLastError();
		m_pLog->WriteLog(LogMaker::eERROR, "TCP 송신실패");
		std::string errcode = "Send 에러코드 : " + std::to_string(result);
		m_pLog->WriteLog(LogMaker::eERROR, errcode);
		return false;
	}

	if (recv(m_hSocket, recvMsg, PACKET_SIZE, nFlag) == SOCKET_ERROR)
	{
		int result = WSAGetLastError();
		m_pLog->WriteLog(LogMaker::eERROR, "TCP 수신실패");
		std::string errcode = "Recv 에러코드 : " + std::to_string(result);
		m_pLog->WriteLog(LogMaker::eERROR, errcode);
		return false;
	}

	return true;
}

//bool connect(char *host, int port, int timeout)
//{
//	TIMEVAL Timeout;
//
//	Timeout.tv_sec = timeout;
//
//	Timeout.tv_usec = 0;
//
//	struct sockaddr_in address;  /* the libc network address data structure */
//
//	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//
//
//	address.sin_addr.s_addr = inet_addr(host); /* assign the address */
//
//	address.sin_port = htons(port);            /* translate int2port num */
//
//	address.sin_family = AF_INET;
//
//
//
//	//set the socket in non-blocking
//
//	unsigned long iMode = 1;
//
//	int iResult = ioctlsocket(sock, FIONBIO, &iMode);
//
//	if (iResult != NO_ERROR)
//	{
//
//		printf("ioctlsocket failed with error: %ld\n", iResult);
//
//	}
//
//
//	if (connect(sock, (struct sockaddr *)&address, sizeof(address)) == false)
//	{
//
//		return false;
//
//	}
//
//
//
//	// restart the socket mode
//
//	iMode = 0;
//
//	iResult = ioctlsocket(sock, FIONBIO, &iMode);
//
//	if (iResult != NO_ERROR)
//
//	{
//
//		printf("ioctlsocket failed with error: %ld\n", iResult);
//
//	}
//
//
//
//	fd_set Write, Err;
//
//	FD_ZERO(&Write);
//
//	FD_ZERO(&Err);
//
//	FD_SET(sock, &Write);
//
//	FD_SET(sock, &Err);
//
//
//
//	// check if the socket is ready
//	select(0, NULL, &Write, &Err, &Timeout);
//
//	if (FD_ISSET(sock, &Write))
//	{
//
//		return true;
//
//	}
//
//
//
//	return false;
//}