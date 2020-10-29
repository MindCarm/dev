#pragma once
#include <iostream>
#include <ctime>
#include <string>
#include <WinSock2.h>
#include <vector>
#include <map>
#include <cassert>
#include <fstream>
#include <ostream>
#include <Windows.h>
#include <ctime>
#include <cstdarg>
#include <algorithm>
#include <cctype>

#include "Markup.h"
#include "LogMaker.h"

#pragma comment(lib, "ws2_32")


enum ePARAMS
{
	ACTIONTYPE = 1
	, PID 
	, PNAME
	, OTHERSPARAM
	, ADDPARAMCOUNT = 7
	, PARAMSIZE = 10
};

constexpr const char* PARAM_DESC[]
{
	"ID", // PID
	"NM", // PName
	"AN", // AccNo
	"SU", // StudyInsUID
	"SI", // StudyID
	"MD", // Modality
	"SD", // StudyDesc
	"PS", // PatientSex
	"PB"  // PatientBirth
};

constexpr const char* PARAM_NAME[]
{
	"",
	"ACTIONTYPE ",
	"PID",
	"PNAME",
	"ACCNUM",
	"STUDYINSUID",
	"MODALITY",
	"STUDYID",
	"STUDYDESC",
	"PSEX",
	"PBIRTHDATE",
};

