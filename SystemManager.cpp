//---------------------------------------------------------------------------
#include <memory>
#include <System.hpp>
#include <vcl.h>
#include "VaClasses.hpp"
#include "VaComm.hpp"

#pragma hdrstop

#include "HelperFunctions.h"
#include "MainTempController.h"
#include "Chamber.h"
#include "SystemManager.h"
#include "LimitController.h"
#include "ST340.h"
#include "VXSeries.h"
#include "I7018.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

unsigned int Colors[MAX_COLOR_COUNT] =
{
	0x00FF0000,
	0x00003300,
	0x0000CC00,
	0x00CCCC00,
	0x00666600,
	0x00FF6600,
	0x00FF0099,
	0x00330099,
	0x009966FF,
	0x00330066,
	0x009900FF,
	0x009900CC,
	0x009999CC,
	0x003399CC,
	0x00FF00FF,
	0x005577DD,
	0x00000000,
	0x000000FF
};

//String LangTag = ENG_TAG;    //2023-03-10  overlap
//String LangStr[LANG_COUNT] =
//{
//	"KOR",
//	"ENG",
//	"CHN",
//};

String ChamberStr[CHAMBER_COUNT] =
{
	"Chamber A",
	"Chamber B",
	"Chamber C",
	"Chamber D"
};

String LevelStr[LV_COUNT] =
{
	"",
	"Operator",
	"Engineer",
	"Supervisor",
	"Developer"
};

String OpmodeStr[UP55X_MODE_COUNT] =
{
	"RESET",
	"PROGRAM",
	"LOCAL",
	"REMOTE"
};

String AutomationKindStr[AUTOMATIONKIND_COUNT] =
{
	"SECS/GEM",
	"Amkor eCIM",
	"SFA ATK",
	"SCK Traceability",
	"Barun MES",
	"ITM Semicon",
	"O-Film MES",
	"MinebeaMitsumi",
	"SCK MES",
	"T-Card Recipe Loading",
	"HANA CIM",
	"Fujitsu T-Card",
	"SECS/GEM AMD",
	"SECS/GEM COWELL",
	"SECS/GEM SCK"
};

// 20190221 - Door Setting - jhlim
String MotorDirectionStr[DIRECTION_COUNT] =
{
  "CCW",
  "CW",
};

// 20180209 - FTP (OFILM MES) - jhlim
FTPConnectInfo ftpConnectInfo;

/* PORTSETTING은 각 장비의 SystemManager.cpp 파일에서 정의한다.

PORTSETTING MainTempPortSetting			= { br9600, db8, sb1, paEven };
PORTSETTING ST340PortSetting			= { br19200, db8, sb1, paNone };
PORTSETTING LoggerPortSetting			= { br19200, db8, sb1, paNone };
PORTSETTING KVC3300PortSetting			= { br9600, db8, sb1, paNone };
PORTSETTING MotorBoardPortSetting		= { br115200, db8, sb1, paNone };
PORTSETTING DIOBoardPortSetting			= { br115200, db8, sb1, paNone };
PORTSETTING DPPortSetting				= { br19200, db8, sb1, paNone };
PORTSETTING O2AnalyzerPortSetting		= { br19200, db8, sb1, paNone };
PORTSETTING ZR5O2AnalyzerPortSetting	= { br9600, db8, sb1, paNone };
PORTSETTING PlasourceRFGenPortSetting	= { br19200, db8, sb1, paNone };
PORTSETTING DamperMotorPortSetting		= { br115200, db8, sb1, paNone };
PORTSETTING DIOPortSetting				= { br115200, db8, sb1, paNone };
// 20170225 - InverterIG5A Add - jhlim
PORTSETTING Inverter008IG5APortSetting 	= { br19200, db8, sb1, paNone };
// 20170321 - FCFC Unit - jhlim
PORTSETTING InverterVFDBPortSetting 	= { br19200, db7, sb2, paNone };
*/

SystemManagerBase::SystemManagerBase()
{
	FLoginLevel = -1;
	FChamberCount = 1;
	FLanguage = ENG;
	FUseAutomation = false;
	FAutomationKind = SECS_GEM;
	FUseRemoteCheck = false;
	// 20181029 - ScreenSaver (infineon) - jhlim
	FUseScreenSaver = false;
	FScreenSaverTime = 60;	// 60 sec
	FIsShowLoginForm = false;
	FOperatorID = "";
	FIsShowValidationForm = false;
    FLotInfoClickChamber = 0;
}

SystemManagerBase::~SystemManagerBase()
{
}

void SystemManagerBase::SetLanguage(int language)
{
	FLanguage = language;
	switch( FLanguage )
	{
	case KOR:
		LangTag = KOR_TAG;
		break;
	case ENG:
		LangTag = ENG_TAG;
		break;
/*	case JPN:
		LangTag = JPN_TAG;
		break; */
	case CHN:
		LangTag = CHN_TAG;
		break;
	default:
		LangTag = KOR_TAG;
		break;
	}
}

BaseChamber* SystemManagerBase::GetAChamber(int chamberId)
{
	if( chamberId >=CHAMBER_A && chamberId < FChamberCount )
		return (OvenChamber*)FChamber[chamberId];

	return nullptr;
}

OvenSystemManagerBase::OvenSystemManagerBase()
{
	for(int i=0; i<MAX_CHAMBER_COUNT; i++)
		FChamber[i] = nullptr;

	SetLanguage(KOR);

	FLoginLevel = LV_NONE;
	FMainTempCtrlTimeout = 2000;
	FLimitCtrlTimeout = 2000;
	FLoggerTimeout = 2000;

	FMaxTemp = 3000;	// 300도
}

OvenSystemManagerBase::~OvenSystemManagerBase()
{
	DestroySerialDevices();
}

DevMainTempController* OvenSystemManagerBase::CreateMainTempController(OvenChamber *chamber, PORTINFO portInfo)
{
	DevMainTempController *mainCtrl = nullptr;

	if( chamber->MainTempControllerType == TEMPCTRL_UP550 )
		mainCtrl = new UP550(chamber, portInfo);
	else
	if( chamber->MainTempControllerType == TEMPCTRL_UP55A )
		mainCtrl = new UP55A(chamber, portInfo);
	return mainCtrl;
}

DevLimitController* OvenSystemManagerBase::CreateLimitController(OvenChamber *chamber, PORTINFO portInfo, int limitType)
{
	DevLimitController *limitCtrl = nullptr;

	if( chamber->LimitControllerType == LIMIT_ST )
	{
		limitCtrl = new ST340(chamber, portInfo, limitType);
	}
	else if( chamber->LimitControllerType == LIMIT_VX )
	{
		limitCtrl = new VXSeries(chamber, portInfo, limitType);
	}
	return limitCtrl;
}

bool OvenSystemManagerBase::CreateSerialDevices()
{
	return true;
}

void OvenSystemManagerBase::DestroySerialDevices()
{
	for(int i=0; i<FChamberCount; i++)
	{
		OvenChamber *chamber = (OvenChamber*)FChamber[i];
		if( chamber )
		{
			if( chamber->MainTempController )
			{
				delete chamber->MainTempController;
				chamber->MainTempController = nullptr;
			}
			if( chamber->ChamberTempLimitController )
			{
				delete chamber->ChamberTempLimitController;
				chamber->ChamberTempLimitController = nullptr;
			}
			for(int j=0; j<FLoggerCount; j++)
			{
				if( chamber->LoggerController[j] )
				{
					delete chamber->LoggerController[j];
					chamber->LoggerController[j] = nullptr;
				}
			}
		}
	}
}

void OvenSystemManagerBase::InitDirectory(String PrgName)
{
	// 폴더 생성
	FProgramName = PrgName;

	AnsiString sDrive;
	String root, data, log, report, remote, FTP, product;

	root = GetRootFolder();
	data = GetDataFolder();
	log = GetLogFolder();
	report = GetReportFolder();
	remote = GetRemotePatternFolder();
	FTP = GetFTPDataFolder();
	product = GetProductFolder();

	if( !DirectoryExists(root) )
	{
		if( !CreateDir(root) )
			throw (Exception(PrgName+" folder creation fail."));
	}
	if( !DirectoryExists(data) )
	{
		if( !CreateDir(data) )
			throw (Exception("DATA folder creation fail."));
	}
	if( !DirectoryExists(log) )
	{
		if( !CreateDir(log) )
			throw (Exception("LOG folder creation fail."));
	}
	if( !DirectoryExists(report) )
	{
		if( !CreateDir(report) )
			throw (Exception("REPORT folder creation fail."));
	}
	if( !DirectoryExists(remote) )
	{
		if( !CreateDir(remote) )
			throw (Exception("REMOTE PATTERN folder creation fail."));
	}
	if( !DirectoryExists(FTP) )
	{
		if( !CreateDir(FTP) )
			throw (Exception("FTP Data File folder creation fail."));
	}
	if( !DirectoryExists(product) )
	{
		if( !CreateDir(product) )
			throw (Exception("PRODUCT folder creation fail."));
	}

	AnsiString sTemp;
	for(int i=0; i<FChamberCount; i++)
	{
		sTemp.sprintf(PATTERN_FILE, 'A' + i);
		sTemp = root + sTemp;
		if( !DirectoryExists(sTemp) )
		{
			if( !CreateDir(sTemp) )
				throw (Exception(sTemp + " folder creation fail."));
		}
	}

	// 파일 생성
	String srcFolder = ExtractFilePath(Application->ExeName);
	String dstFile = data + OVENDB;
	String srcFile = srcFolder + OVENDB;
	if( FileExists(dstFile) == false )
		CopyFile(srcFile.c_str(), dstFile.c_str(), true);

	dstFile = data + ERRDEFDB;
	srcFile = srcFolder + ERRDEFDB;
//	if( FileExists(dstFile) == false )
		CopyFile(srcFile.c_str(), dstFile.c_str(), false);

	dstFile = data + LANGUAGEINI;
	srcFile = srcFolder + LANGUAGEINI;
//	if( FileExists(dstFile) == false )
		CopyFile(srcFile.c_str(), dstFile.c_str(), false);

	dstFile = data + OVENSYSTEMINI;
	srcFile = srcFolder + OVENSYSTEMINI;
	if( FileExists(dstFile) == false )
		CopyFile(srcFile.c_str(), dstFile.c_str(), true);

	// GEM File Copy
	dstFile = data + "ALID.TXT";
	srcFile = srcFolder + "ALID.TXT";
//	if( FileExists(dstFile) == false )
		CopyFile(srcFile.c_str(), dstFile.c_str(), false);

	dstFile = data + "ALID_COWELL.TXT";
	srcFile = srcFolder + "ALID_COWELL.TXT";
//	if( FileExists(dstFile) == false )
		CopyFile(srcFile.c_str(), dstFile.c_str(), false);

	dstFile = data + "CEID.TXT";
	srcFile = srcFolder + "CEID.TXT";
//	if( FileExists(dstFile) == false )
		CopyFile(srcFile.c_str(), dstFile.c_str(), false);

	dstFile = data + "RPTID.TXT";
	srcFile = srcFolder + "RPTID.TXT";
//	if( FileExists(dstFile) == false )
		CopyFile(srcFile.c_str(), dstFile.c_str(), false);

	dstFile = data + "SVID.TXT";
	srcFile = srcFolder + "SVID.TXT";
//	if( FileExists(dstFile) == false )
		CopyFile(srcFile.c_str(), dstFile.c_str(), false);

	dstFile = data + "FORMAT.SML";
	srcFile = srcFolder + "FORMAT.SML";
//	if( FileExists(dstFile) == false )
		CopyFile(srcFile.c_str(), dstFile.c_str(), false);

	dstFile = data + "CEID_JSCK.TXT";
	srcFile = srcFolder + "CEID_JSCK.TXT";
//	if( FileExists(dstFile) == false )
		CopyFile(srcFile.c_str(), dstFile.c_str(), false);

	dstFile = data + "RPTID_JSCK.TXT";
	srcFile = srcFolder + "RPTID_JSCK.TXT";
//	if( FileExists(dstFile) == false )
		CopyFile(srcFile.c_str(), dstFile.c_str(), false);

	dstFile = data + "SVID_JSCK.TXT";
	srcFile = srcFolder + "SVID_JSCK.TXT";
//	if( FileExists(dstFile) == false )
		CopyFile(srcFile.c_str(), dstFile.c_str(), false);
}
