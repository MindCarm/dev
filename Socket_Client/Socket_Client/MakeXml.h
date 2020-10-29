#pragma once

class LogMaker;

class MakeXml
{
public:
	MakeXml(LogMaker* pLog) : p_Log(pLog) {};
	~MakeXml() = default;

	bool MakeProtocol(const std::string strActionType, const std::string strPID, const std::string strPName,
		const std::string strSex, const std::string strBirthDate, const std::string strAccNum,
		const std::string strModality, const std::string strStudyDesc, const std::string strStudyInsUID,
		const std::string strStudyID);
	bool MakeProtocol(int argc, char* params[]);

	void GetAttributes(const string& val, string& key, string& name);

	bool RecvXml(const char* strRecv);

	const std::string GetSendXml() const { return m_strSendXml + "\0"; }
	const std::string GetRecvXml() const { return m_strRecvXml; }

private:
	bool InValidString(const char* c);

private:
	map<string, string> m_argcs;
	CMarkup m_Xml;
	LogMaker* p_Log;

	std::string m_strSendXml;
	std::string m_strRecvXml;
};