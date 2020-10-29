#pragma once
using namespace std;

class LogMaker
{
public:
	enum eLOGTYPE
	{
		eINFO = 0
		, eWARNING
		, eERROR
		, eFATAL
	};

public:
	LogMaker(int deleteDay, const char* pathName = "");
	~LogMaker();

	void WriteLog(eLOGTYPE logType, const string& strMessage);

private:
	void MakeLogFile(const char* pathName = "");
	void DeleteLogFiles();

private:
	int m_nDeleteDay;
	string m_strFileName;
	string m_strPathName;
	string m_strCreateDate;
};