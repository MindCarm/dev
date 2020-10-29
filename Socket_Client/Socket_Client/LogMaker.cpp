#include "stdafx.h"
#include "LogMaker.h"
#include <io.h>

constexpr const char* LOGTYPE[] = { " [INFO]", " [WARNING]", " [ERROR]", " [FATAL]" };

const string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);

	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}

const string currentTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);

	strftime(buf, sizeof(buf), "%T", &tstruct);

	return buf;
}

const string currentDate() {
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y%m%d", &tstruct);
	return buf;
}

LogMaker::LogMaker(int deleteDay, const char * pathName)
	:m_nDeleteDay(deleteDay)
{
	m_strCreateDate = currentDate();

	MakeLogFile(pathName);
}

LogMaker::~LogMaker()
{
	DeleteLogFiles();
}

void LogMaker::WriteLog(eLOGTYPE logType, const string& strMessage)
{
	string strCmpDate = currentDate();
	if (m_strCreateDate.compare(strCmpDate) != 0)
		MakeLogFile();


	ofstream logFile;
	logFile.open(m_strFileName, ios::app);

	string strLogMsg = currentTime() + LOGTYPE[logType] + " : " + strMessage + "\n";

	logFile.seekp(ios::end, ios::beg);
	logFile.write(strLogMsg.c_str(), strLogMsg.size());

	logFile.close();
}

void CreateDir(const char* Path)
{

	char DirName[256];  //생성할 디렉초리 이름
	char* p = (char*)Path;     //인자로 받은 디렉토리
	char* q = DirName;



	while (*p)
	{
		if (('\\' == *p) || ('/' == *p))   //루트디렉토리 혹은 Sub디렉토리
		{
			if (':' != *(p - 1))
			{
				CreateDirectory(DirName, NULL);
			}
		}
		*q++ = *p++;

		*q = '\0';
	}

	CreateDirectory(DirName, NULL);
}

void LogMaker::MakeLogFile(const char * pathName)
{
	CreateDir(pathName);
	m_strPathName = pathName;
	ofstream logFile;
	m_strFileName = pathName + currentDate() + ".LOG";
	logFile.open(m_strFileName, ios::app);
	logFile.close();
}

void LogMaker::DeleteLogFiles()
{
	string path = m_strPathName + "*.log";
	_finddata_t fd;
	intptr_t handle = _findfirst(path.c_str(), &fd);

	if (-1 == handle) return;

	int result = 0;
	do
	{
		int year = 0, month = 0, day = 0;
		char* pt1 = nullptr, *pt2 = nullptr, temp[16] = { 0 };
		char yearCopy[4] = { 0 }, monthCopy[2] = { 0 }, dayCopy[2] = { 0 };

		pt1 = strstr(fd.name, "20");
		pt2 = strstr(fd.name, ".log");

		memcpy(temp, pt1, 10);
		memcpy(yearCopy, &temp, 4);
		memcpy(monthCopy, &temp[4], 2);
		memcpy(dayCopy, &temp[6], 2);

		year = atoi(yearCopy);
		month = atoi(monthCopy);
		day = atoi(dayCopy);

		time_t tm_st, tm_nd;
		struct tm fileDay;

		memset(&fileDay, 0, sizeof(tm));

		fileDay.tm_year = year - 1900;
		fileDay.tm_mon = month - 1;
		fileDay.tm_mday = day;

		tm_st = mktime(&fileDay);
		time(&tm_nd);
		double d_diff = difftime(tm_nd, tm_st);
		int tm_day = static_cast<int>(d_diff / (60 * 60 * 24));
		
		if (tm_day >= m_nDeleteDay)
		{
			char deleteFilePath[100];
			memset(deleteFilePath, 0, sizeof(deleteFilePath));
			strcat_s(deleteFilePath, sizeof(deleteFilePath), m_strPathName.c_str());
			strcat_s(deleteFilePath, sizeof(deleteFilePath), fd.name);

			remove(deleteFilePath);
		}
		result = _findnext(handle, &fd);
	} while (result != -1);
	_findclose(handle);
}

