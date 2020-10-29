#include "stdafx.h"
#include "MakeXml.h"
#include "Socket.h"

using namespace std;

void LoadConfig(string& strIP, int& nPort, int& nDeleteDay)
{
	GetPrivateProfileString("Socket", "ServerIP", "InvalidIP", (LPSTR)strIP.data(), 64, "./Config.ini");
	nPort = GetPrivateProfileInt("Socket", "ServerPort", 0, "./Config.ini");
	nDeleteDay = GetPrivateProfileInt("Socket", "LogDeleteDay", 0, "./Config.ini");
}

void SaveConfig(const string strIP, const int nPort, const int nDeleteDay)
{
	WritePrivateProfileString("Socket", "ServerIP", strIP.c_str(), "./Config.ini");
	char szValue[255];
	sprintf(szValue, "%d", nPort);
	WritePrivateProfileString("Socket", "ServerPort", szValue, "./Config.ini");
	sprintf(szValue, "%d", nDeleteDay);
	WritePrivateProfileString("Socket", "LogDeleteDay", szValue, "./Config.ini");
}


int main(int argc, char* argv[])
{
	string strServerIP;
	int nServerPort, nLogDeleteDay;
	LoadConfig(strServerIP, nServerPort, nLogDeleteDay);

	LogMaker log(nLogDeleteDay, "SocketLog/");
	log.WriteLog(LogMaker::eINFO, "------------------------------");
	log.WriteLog(LogMaker::eINFO, "Program Start");

	
#ifndef _DEBUG
	if (argc < PNAME)
	{
		log.WriteLog(LogMaker::eERROR, "Program End Because Not Enough argc");
		log.WriteLog(LogMaker::eINFO, "------------------------------");
		return 0;
	}
#endif
	
	
#ifdef _DEBUG
	MakeXml xml(&log);
	//xml.MakeProtocol(argc, argv);
		xml.MakeProtocol(
			"END",
			"PID",
			"유민근",
			"Sex",
			"BirthDate",
			"AccNum",
			"Modality",
			"StudyDesc",
			"StudyInsUID",
			"StudyID");
	
	cout << xml.GetSendXml() << endl;
#else
	MakeXml xml(&log);
	if(xml.MakeProtocol(argc, argv) == false)
		log.WriteLog(LogMaker::eERROR, "Xml File Make Failed!");

	
	log.WriteLog(LogMaker::eINFO, xml.GetSendXml());
	
#endif
	
	
	std::string sendMsg = xml.GetSendXml() + "\0";

	char recvMsg[PACKET_SIZE] = {};
	Socket socket(strServerIP, nServerPort, &log);
	if (socket.Connect() == false)
	{
		log.WriteLog(LogMaker::eFATAL, "Socket Connect Fail!");
		log.WriteLog(LogMaker::eINFO, "Program End");
		log.WriteLog(LogMaker::eINFO, "------------------------------");
		return 0;
	}

	

	if (socket.Communication(sendMsg.c_str(), recvMsg) == false)
	{
		log.WriteLog(LogMaker::eFATAL, "Socket Communication Fail!");
		log.WriteLog(LogMaker::eINFO, "Program End");
		log.WriteLog(LogMaker::eINFO, "------------------------------");
		return 0;
	}
	

	if (xml.RecvXml(recvMsg) == false)
	{
		log.WriteLog(LogMaker::eERROR, "Recv Msg Error");
		log.WriteLog(LogMaker::eINFO, "Program End");
		log.WriteLog(LogMaker::eINFO, "------------------------------");
		return 0;
	}

#ifdef _DEBUG
	cout << xml.GetRecvXml().c_str() << endl;
#endif // _DEBUG

	
	SaveConfig(strServerIP, nServerPort, nLogDeleteDay);
	log.WriteLog(LogMaker::eINFO, "Program End");
	log.WriteLog(LogMaker::eINFO, "------------------------------");
	return 0;
}
