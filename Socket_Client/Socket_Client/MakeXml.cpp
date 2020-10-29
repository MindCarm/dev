#include "stdafx.h"
#include "MakeXml.h"
#include "LogMaker.h"

const char* convert_TagTable[]
{
	"0010_0020",
	"0010_0010",
	"0008_0050",
	"0020_000d",
	"0020_0010",
	"0008_0060",
	"0008_1030",
	"0010_0040",
	"0010_0030"
};

const char* convert_NameTable[]
{
	"PatientID",
	"PationtName",
	"AccessionNumber",
	"StudyInstanceUID",
	"StudyID",
	"Modality",
	"StudyDesc",
	"PatientSex",
	"PatientBrithDate"
};

const string gen_random(const int len)
{
	srand((unsigned int)time(0));

	static const char alpha[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	string s;

	for (int i = 0; i < len; ++i) {
		s += alpha[rand() % (sizeof(alpha) - 1)];
	}

	return s;
}

static inline void ltrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

static inline void rtrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

static inline void trim(std::string& s)
{
	ltrim(s);
	rtrim(s);
}

void MakeXml::GetAttributes(const string & val, string & key, string & name)
{
	for (int i = 0; i < 9; i++) {
		if (stricmp(val.c_str(), PARAM_DESC[i]) == 0) {
			key = convert_TagTable[i];
			name = convert_NameTable[i];
			break;
		}
	}
}

bool MakeXml::MakeProtocol(const string strActionType, const string strPID, const string strPName, const string strSex, const string strBirthDate, const string strAccNum, const string strModality, const string strStudyDesc, const string strStudyInsUID, const string strStudyID)
{
	if(strPID.empty() || strPName.empty())
		return false;

	CMarkup xml;

	xml.AddElem("ENDO");
	xml.AddChildElem("TYPE", "REQUEST");
	xml.IntoElem();
	xml.AddAttrib("SEQUENCE_ID", gen_random(10));
	xml.OutOfElem();

	xml.AddChildElem("COMMAND");
	xml.IntoElem();
	xml.AddChildElem("ACTION", strActionType);
	xml.OutOfElem();

	xml.AddChildElem("PARAMETERS");
	xml.IntoElem();
	
	xml.AddChildElem("PARAM");
	xml.IntoElem();
	xml.AddAttrib("TAG_KEY", "0010_0020");
	xml.AddAttrib("NAME", "PatientID");
	xml.AddAttrib("VALUE", strPID);
	xml.OutOfElem();
	xml.AddChildElem("PARAM");
	xml.IntoElem();
	xml.AddAttrib("TAG_KEY", "0010_0010");
	xml.AddAttrib("NAME", "PatientName");
	xml.AddAttrib("VALUE", strPName);
	xml.OutOfElem();
	xml.AddChildElem("PARAM");
	xml.IntoElem();
	xml.AddAttrib("TAG_KEY", "0010_0040");
	xml.AddAttrib("NAME", "PatientSex");
	xml.AddAttrib("VALUE", strSex);
	xml.OutOfElem();
	xml.AddChildElem("PARAM");
	xml.IntoElem();
	xml.AddAttrib("TAG_KEY", "0010_0030");
	xml.AddAttrib("NAME", "PatientBirthDate");
	xml.AddAttrib("VALUE", strBirthDate);
	xml.OutOfElem();
	xml.AddChildElem("PARAM");
	xml.IntoElem();
	xml.AddAttrib("TAG_KEY", "0008_0050");
	xml.AddAttrib("NAME", "AccessionNumber");
	xml.AddAttrib("VALUE", strAccNum);
	xml.OutOfElem();
	xml.AddChildElem("PARAM");
	xml.IntoElem();
	xml.AddAttrib("TAG_KEY", "0008_0060");
	xml.AddAttrib("NAME", "Modality");
	xml.AddAttrib("VALUE", strModality);
	xml.OutOfElem();
	xml.AddChildElem("PARAM");
	xml.IntoElem();
	xml.AddAttrib("TAG_KEY", "0008_1030");
	xml.AddAttrib("NAME", "StudyDescription");
	xml.AddAttrib("VALUE", strStudyDesc);
	xml.OutOfElem();
	xml.AddChildElem("PARAM");
	xml.IntoElem();
	xml.AddAttrib("TAG_KEY", "0020_000d");
	xml.AddAttrib("NAME", "StudyInstanceUID");
	xml.AddAttrib("VALUE", strStudyInsUID);
	xml.OutOfElem();
	xml.AddChildElem("PARAM");
	xml.IntoElem();
	xml.AddAttrib("TAG_KEY", "0020_0010");
	xml.AddAttrib("NAME", "StudyID");
	xml.AddAttrib("VALUE", strStudyID);
	xml.OutOfElem();

	xml.OutOfElem();

	xml.OutOfElem();

	m_strSendXml = xml.GetDoc();

	return true;
}

bool MakeXml::MakeProtocol(int argc, char * params[])
{
	for (int i = 0; i < 9; i++) 
		m_argcs[PARAM_DESC[i]] = "";
	

	m_argcs["ID"] = params[PID];
	m_argcs["NM"] = params[PNAME];

	for (int i = OTHERSPARAM; i < argc; i++)
	{
		char* otherParam = strtok(params[i], "/:");
		for (auto& iter : m_argcs)
		{
			if (stricmp(iter.first.c_str(), otherParam) == 0)
			{
				otherParam = strtok(nullptr, "/:");
				iter.second = otherParam;
				trim(iter.second);
				break;
			}
		}
	}

	m_Xml.AddElem("ENDO");
	m_Xml.AddChildElem("TYPE", "REQUEST");
	m_Xml.IntoElem();
	m_Xml.AddAttrib("SEQUENCE_ID", gen_random(10));
	m_Xml.OutOfElem();

	m_Xml.AddChildElem("COMMAND");
	m_Xml.IntoElem();
	m_Xml.AddChildElem("ACTION", params[ACTIONTYPE]);
	m_Xml.OutOfElem();

	m_Xml.AddChildElem("PARAMETERS");
	m_Xml.IntoElem();
	for (auto param : m_argcs)
	{
		string tagKey, name;
		GetAttributes(param.first, tagKey, name);
		m_Xml.AddChildElem("PARAM");
		m_Xml.IntoElem();
		m_Xml.AddAttrib("TAG_KEY", tagKey);
		m_Xml.AddAttrib("NAME", name);
		m_Xml.AddAttrib("VALUE", param.second);
		m_Xml.OutOfElem();
	}

	m_Xml.OutOfElem();

	m_Xml.OutOfElem();

	m_Xml.OutOfElem();
	
	
	m_Xml.Save("m_Xml.xml");
	m_strSendXml = m_Xml.GetDoc();
	p_Log->WriteLog(LogMaker::eINFO, "Make SendString");

	return true;
}



bool MakeXml::RecvXml(const char* strRecv)
{
	CMarkup recvXml;
	if (recvXml.SetDoc(strRecv) == false)
	{
		p_Log->WriteLog(LogMaker::eERROR, "Not XML format!");
		return false;
	}
	string strResult, strCode, strDesc;
	while (recvXml.FindElem("ENDO"))
	{
		recvXml.FindChildElem("RESULT");
		strResult = recvXml.GetChildData();
		recvXml.FindChildElem("CODE");
		strCode = recvXml.GetChildData();
		recvXml.FindChildElem("DESCRIPTION");
		strDesc = recvXml.GetChildData();
	}

	if (strResult == "ERROR")
	{
		string faildMsg = "Failed Result : ";
		faildMsg += strResult;
		p_Log->WriteLog(LogMaker::eERROR, faildMsg);

		faildMsg = "Failed Code : ";
		faildMsg += strCode;
		p_Log->WriteLog(LogMaker::eERROR, faildMsg);

		faildMsg = "Failed Desc : ";
		faildMsg += strDesc;
		p_Log->WriteLog(LogMaker::eERROR, faildMsg);

		return false;
	}

#ifdef _DEBUG
	recvXml.Save("recv.xml");
#endif // _DEBUG

	p_Log->WriteLog(LogMaker::eINFO, "Recv Success!");
	string logMsg = "recvCodeMsg : " + strCode;
	p_Log->WriteLog(LogMaker::eINFO, logMsg);
	logMsg = "recvDescMsg : " + strDesc;
	p_Log->WriteLog(LogMaker::eINFO, logMsg);

	m_strRecvXml = move(recvXml.GetDoc());

	return true;
}
