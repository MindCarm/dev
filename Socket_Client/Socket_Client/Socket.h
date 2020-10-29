#pragma once

class LogMaker;

constexpr int PACKET_SIZE = 2048;

class Socket
{
public:
	Socket(const std::string strIP, u_short nPort, LogMaker* pLog);
	~Socket();

	bool Connect();

	bool Communication(const char* sendMsg, char(&recvMsg)[PACKET_SIZE], int nFlag = 0);


private:
	SOCKET m_hSocket;
	SOCKADDR_IN m_tAddr;
	WSADATA m_WsaData;

	LogMaker* m_pLog;
};