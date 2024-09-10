//---------------------------------------------------------------------------
#include <memory>
#include <System.Math.hpp>

#pragma hdrstop

#include "NormalOvenSystemManager.h"
#include "NormalOvenChamber.h"
#include "HelperFunctions.h"
#include "MainTempController.h"
#include "ST340.h"
#include "I7018.h"
#include "I7000DIOModule.h"
#include "ZR5.h"
#include "DiffPressure.h"
#include "MoonsStepMotor.h"
#include "EziStepMotor.h"
#include "Database.h"
#include "VSOvenBoard.h"
#include "LSInverter.h"
#include "REX_P250.h"
#include "NessLabRFID.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

PORTSETTING MainTempPortSetting			= { br9600, db8, sb1, paEven };
PORTSETTING ST340PortSetting			= { br19200, db8, sb1, paNone };
PORTSETTING LoggerPortSetting			= { br19200, db8, sb1, paNone };
PORTSETTING DPPortSetting				= { br19200, db8, sb1, paNone };
PORTSETTING ZR5O2AnalyzerPortSetting	= { br9600, db8, sb1, paNone };
PORTSETTING DamperMotorPortSetting		= { br115200, db8, sb1, paNone };
PORTSETTING DIOPortSetting				= { br115200, db8, sb1, paNone };
PORTSETTING VSDIOPortSetting		   	= { br19200, db8, sb1, paNone };
PORTSETTING InverterPortSetting			= { br9600, db8, sb1, paNone };
PORTSETTING REXP250PortSetting			= { br9600, db8, sb1, paNone };
PORTSETTING NessLabRFIDPortSetting		= { br9600, db8, sb1, paNone };

String StatusStr[STEP_COUNT] =
{
	"IDLE",
	"PURGE",
	"RUN",
	"COOL",
	"DONE",
	"LOCAL"
};

String EventName[MAX_EVENT_COUNT] =
{
	"PV HIGH",
	"PV LOW",
	"COOLING",
	"PURGE",
	"PV HIGH CLEAR",
	"PV LOW CLEAR",
	"",
	"",
	"",
	""
};

PTNEVENT EventList[MAX_EVENT_COUNT] =
{
	{ EventName[PVHIGH], 		ET_PVHIGH,			"PV Offset",	"" 		},
	{ EventName[PVLOW],			ET_PVLOW,			"PV Offset",	"" 		},
	{ EventName[COOLING], 		ET_COOLING,			"On",			"Off"	},
	{ EventName[PURGE], 		ET_PURGE,			"On",			"Off"	},
	{ EventName[PVHIGH_CLEAR], 	ET_PVHIGH_CLEAR,	"",				""		},
	{ EventName[PVLOW_CLEAR], 	ET_PVLOW_CLEAR,		"",				""	 	},
	{ EventName[6],				ET_NONE,			"",				""		},
	{ EventName[7], 			ET_NONE,			"",				""		},
	{ EventName[8], 			ET_NONE,			"",				""		},
	{ EventName[9], 			ET_NONE,			"",				""		}
};

WatchFtpConnect::WatchFtpConnect()
	: TThread(false)
{
}

__fastcall WatchFtpConnect::~WatchFtpConnect()
{
}

void __fastcall WatchFtpConnect::Execute()
{
	while (Terminated == false)
	{
		WaitForSingleObject((void *)Handle, THREAD_WAIT_TIME);

		if( GetManager()->CheckFtpConnect == true )
		{
			GetManager()->CreateFTPConnection();
			GetManager()->DeleteFTPDataFile();
			GetManager()->CheckFtpConnect = false;
		}
	}
}

/****************************************************************************
Description	: CTowerLamp Class
*****************************************************************************/
CTowerLamp::CTowerLamp()
{
	FbChanging = false;
}
//---------------------------------------------------------------------------
CTowerLamp::~CTowerLamp()
{

}
//---------------------------------------------------------------------------

bool CTowerLamp::operator==(const CTowerLamp& arg)
{
	bool bRet = true;

	for(int i = 0; i < TOWERLAMP_KIND; i++)
	{
		for(int j = 0; j < TOWERLAMP_MODE; j++)
		{
			int nSetVal = arg.GetTwrVal(i, j);
			if(nSetVal != nTowerVal[i][j] )
			{
				bRet = false;
				break;
            }
        }
	}

	return bRet;
}
//---------------------------------------------------------------------------
CTowerLamp& CTowerLamp::operator=(const CTowerLamp& arg)
{
	for(int i = 0; i < TOWERLAMP_KIND; i++)
	{
		for(int j = 0; j < TOWERLAMP_MODE; j++)
		{
			int nSetVal = arg.GetTwrVal(i, j);
			SetTwrVal(i, j, nSetVal);
//			nTowerVal[i][j] = nSetVal;
		}
	}
	return *this;
}
//---------------------------------------------------------------------------
void CTowerLamp::LoadValue(const String sPath)
{
	std::shared_ptr<TIniFile> TowerLampFile(new TIniFile(sPath) );
	const String sSection = "TOWER LAMP";

	if(!TowerLampFile->SectionExists(sSection) )
	{
		nTowerVal[TWR_RED][TWR_AUTORUN]	= LAMP_OFF;
		nTowerVal[TWR_RED][TWR_STOP]	= LAMP_OFF;
		nTowerVal[TWR_RED][TWR_LOTEND]	= LAMP_OFF;
		nTowerVal[TWR_RED][TWR_ERR]		= LAMP_ON;

		nTowerVal[TWR_YEL][TWR_AUTORUN]	= LAMP_OFF;
		nTowerVal[TWR_YEL][TWR_STOP]	= LAMP_ON;
		nTowerVal[TWR_YEL][TWR_LOTEND]	= LAMP_BLINK;
		nTowerVal[TWR_YEL][TWR_ERR]		= LAMP_OFF;

		nTowerVal[TWR_GRN][TWR_AUTORUN]	= LAMP_ON;
		nTowerVal[TWR_GRN][TWR_STOP]	= LAMP_OFF;
		nTowerVal[TWR_GRN][TWR_LOTEND]	= LAMP_OFF;
		nTowerVal[TWR_GRN][TWR_ERR]		= LAMP_OFF;

		nTowerVal[TWR_BUZ][TWR_AUTORUN]	= BUZZ_OFF;
		nTowerVal[TWR_BUZ][TWR_STOP]	= BUZZ_OFF;
		nTowerVal[TWR_BUZ][TWR_LOTEND]	= BUZZ_ALARM;
		nTowerVal[TWR_BUZ][TWR_ERR]		= BUZZ_ALARM;
	}
	else
	{
		try
		{
			nTowerVal[TWR_RED][TWR_AUTORUN]	= TowerLampFile->ReadInteger(sSection, "RED[AUTO_RUN]",	nTowerVal[TWR_RED][TWR_AUTORUN]);
			nTowerVal[TWR_RED][TWR_STOP]	= TowerLampFile->ReadInteger(sSection, "RED[STOP]",    	nTowerVal[TWR_RED][TWR_STOP]);
			nTowerVal[TWR_RED][TWR_LOTEND]	= TowerLampFile->ReadInteger(sSection, "RED[LOTEND]", 	nTowerVal[TWR_RED][TWR_LOTEND]);
			nTowerVal[TWR_RED][TWR_ERR]		= TowerLampFile->ReadInteger(sSection, "RED[ERR]", 		nTowerVal[TWR_RED][TWR_ERR]);

			nTowerVal[TWR_YEL][TWR_AUTORUN]	= TowerLampFile->ReadInteger(sSection, "YELLOW[AUTO_RUN]", nTowerVal[TWR_YEL][TWR_AUTORUN]);
			nTowerVal[TWR_YEL][TWR_STOP]	= TowerLampFile->ReadInteger(sSection, "YELLOW[STOP]", 	   nTowerVal[TWR_YEL][TWR_STOP]);
			nTowerVal[TWR_YEL][TWR_LOTEND]	= TowerLampFile->ReadInteger(sSection, "YELLOW[LOTEND]",   nTowerVal[TWR_YEL][TWR_LOTEND]);
			nTowerVal[TWR_YEL][TWR_ERR]		= TowerLampFile->ReadInteger(sSection, "YELLOW[ERR]",      nTowerVal[TWR_YEL][TWR_ERR]);

			nTowerVal[TWR_GRN][TWR_AUTORUN]	= TowerLampFile->ReadInteger(sSection, "GREEN[AUTO_RUN]",  nTowerVal[TWR_GRN][TWR_AUTORUN]);
			nTowerVal[TWR_GRN][TWR_STOP]	= TowerLampFile->ReadInteger(sSection, "GREEN[STOP]", 	   nTowerVal[TWR_GRN][TWR_STOP]);
			nTowerVal[TWR_GRN][TWR_LOTEND]	= TowerLampFile->ReadInteger(sSection, "GREEN[LOTEND]",	   nTowerVal[TWR_GRN][TWR_LOTEND]);
			nTowerVal[TWR_GRN][TWR_ERR]		= TowerLampFile->ReadInteger(sSection, "GREEN[ERR]",       nTowerVal[TWR_GRN][TWR_ERR]);

			nTowerVal[TWR_BUZ][TWR_AUTORUN]	= TowerLampFile->ReadInteger(sSection, "BUZZER[AUTO_RUN]", nTowerVal[TWR_BUZ][TWR_AUTORUN]);
			nTowerVal[TWR_BUZ][TWR_STOP]	= TowerLampFile->ReadInteger(sSection, "BUZZER[STOP]", 	   nTowerVal[TWR_BUZ][TWR_STOP]);
			nTowerVal[TWR_BUZ][TWR_LOTEND]	= TowerLampFile->ReadInteger(sSection, "BUZZER[LOTEND]",   nTowerVal[TWR_BUZ][TWR_LOTEND]);
			nTowerVal[TWR_BUZ][TWR_ERR]		= TowerLampFile->ReadInteger(sSection, "BUZZER[ERR]",      nTowerVal[TWR_BUZ][TWR_ERR]);
		}
		catch (Exception* e)
		{
			SetMessageStr(e->Message);
			ShowMessageDlg();
		}
	}

}
//---------------------------------------------------------------------------
void CTowerLamp::SaveValue(const String sPath)
{
	std::shared_ptr<TIniFile> TowerLampFile(new TIniFile(sPath) );
	const String sSection = "TOWER LAMP";

	try
	{
		TowerLampFile->WriteInteger(sSection, "RED[AUTO_RUN]", 	nTowerVal[TWR_RED][TWR_AUTORUN]);
		TowerLampFile->WriteInteger(sSection, "RED[STOP]", 		nTowerVal[TWR_RED][TWR_STOP]);
		TowerLampFile->WriteInteger(sSection, "RED[LOTEND]", 	nTowerVal[TWR_RED][TWR_LOTEND]);
		TowerLampFile->WriteInteger(sSection, "RED[ERR]", 		nTowerVal[TWR_RED][TWR_ERR]);

		TowerLampFile->WriteInteger(sSection, "YELLOW[AUTO_RUN]", nTowerVal[TWR_YEL][TWR_AUTORUN]);
		TowerLampFile->WriteInteger(sSection, "YELLOW[STOP]", 	  nTowerVal[TWR_YEL][TWR_STOP]);
		TowerLampFile->WriteInteger(sSection, "YELLOW[LOTEND]",   nTowerVal[TWR_YEL][TWR_LOTEND]);
		TowerLampFile->WriteInteger(sSection, "YELLOW[ERR]",      nTowerVal[TWR_YEL][TWR_ERR]);

		TowerLampFile->WriteInteger(sSection, "GREEN[AUTO_RUN]",  nTowerVal[TWR_GRN][TWR_AUTORUN]);
		TowerLampFile->WriteInteger(sSection, "GREEN[STOP]", 	  nTowerVal[TWR_GRN][TWR_STOP]);
		TowerLampFile->WriteInteger(sSection, "GREEN[LOTEND]", 	  nTowerVal[TWR_GRN][TWR_LOTEND]);
		TowerLampFile->WriteInteger(sSection, "GREEN[ERR]",       nTowerVal[TWR_GRN][TWR_ERR]);

		TowerLampFile->WriteInteger(sSection, "BUZZER[AUTO_RUN]", nTowerVal[TWR_BUZ][TWR_AUTORUN]);
		TowerLampFile->WriteInteger(sSection, "BUZZER[STOP]", 	  nTowerVal[TWR_BUZ][TWR_STOP]);
		TowerLampFile->WriteInteger(sSection, "BUZZER[LOTEND]",   nTowerVal[TWR_BUZ][TWR_LOTEND]);
		TowerLampFile->WriteInteger(sSection, "BUZZER[ERR]",      nTowerVal[TWR_BUZ][TWR_ERR]);
	}
	catch (Exception* e)
	{
		SetMessageStr(e->Message);
		ShowMessageDlg();
	}
}
//---------------------------------------------------------------------------
int CTowerLamp::GetTwrVal(const UINT nKind, const UINT nMode) const
{   // error -1
	if(!InRange(nKind, TWR_RED, TOWERLAMP_KIND-1) )
		return -1;

	if(!InRange(nMode, TWR_AUTORUN, TOWERLAMP_MODE-1) )
		return -1;

	int nRet = nTowerVal[nKind][nMode];

	return nRet;
}
//---------------------------------------------------------------------------
void CTowerLamp::SetTwrVal(const UINT nKind, const UINT nMode, const int nVal)
{
	if(!InRange(nKind, TWR_RED, TOWERLAMP_KIND-1) )
		return;

	if(!InRange(nMode, TWR_AUTORUN, TOWERLAMP_MODE-1) )
		return;

	nTowerVal[nKind][nMode] = nVal;
}
//---------------------------------------------------------------------------
String CTowerLamp::GetTwrValStr(const UINT nKind) const
{
	String sVal = "[";

	for(UINT i = 0; i < TOWERLAMP_MODE; ++i)
	{
		sVal += nTowerVal[nKind][i];
	}

	sVal += "]";

	return sVal;
}
//---------------------------------------------------------------------------
NormalOvenSystemManager::NormalOvenSystemManager()
	: OvenSystemManagerBase()
{
	FLanguage = ENG;
	FChamberCount = 4;
	FLoggerCount = 1;
	FLoggerPointCount = 5;
//	FGasType = N2;
//	FUseCIM = false;
	FMaxTemp = 500;
	FEmoOption = STOP_WORKING;
	FJobEndTimer = 10;
	FDoorLockTemp = 0;		// 2020-12-16 Add 챔버온도 도어락

	FCoolingVentExuastTime = 0;	// 2021-06-09 KJH 쿨링워터 배출시간

	FMainTempCtrlPort = COM1;
	FChamberTempLimitCtrlPort = COM2;
	FLoggerPort = COM2;
	FDPCtrlPort = COM3;
	FDIOCtrlPort = COM4;
	FDamperMotorCtrlPort = COM5;
	FO2AnalyzerAPort = COM6;
	FO2AnalyzerBPort = COM7;
	FO2AnalyzerCPort = COM8;
	FO2AnalyzerDPort = COM9;
	FInverterPort = COM4;
	FREXP250APort = COM7; //2021-04-12 HANA OVEN
	FREXP250BPort = COM8;
	FREXP250CPort = COM9;
	FREXP250DPort = COM10;
	FRfidReaderAPort = COM11;

	FDamperMotorTimeout = 500;
	FDIOTimeout = 500;
	FDPTimeout = 500;
	FO2AnalyzerATimeout = 500;
	FO2AnalyzerBTimeout = 500;

	FDIOType = ICPDAS;
	FDPType = false;
	FUseO2Analyzer = false;
	FUseInverter = false;
	FDamperControlType = CYLINDER_DAMPER;

	FUseWaterFlowDetector = true;
	FUseN2FlowDetector = false;
	FUseN2FlowControl = false;
	FUseMagazineDetector = false;
	FUseElectricPanelDoorDetector = false;
	FUseAirPressureDetector = false;
    FUseN2PressureDetector = false;
    FUseHeaterOverloadDetector = false;
	FUseDiffPressureDetector = false;
    FUseSSROverTempDetector = false;
    FUseChamberDoorTempInterlock = false;
	FUseGroundAlarmDetector = false;
    FUseBlowMotorLowCurrentDetector = false;
	FUseDoorLockCylinder = false;
	FUseTowerLampControl = false;
	FUseDoorLockTempControl = false;			// 2020-12-16 Add 챔버온도 도어락
	FUsePasswordWhenIdleCoolingStop = false;	// 2021-03-05 Add IdleCooling일때 패스워스 사용유무
	FUseLoggerDataSave = false;                 // 2021-03-15 ADD Logger 데이터 저장 사용유무
	FUseLocalN2Alarm = false;
	FDrawBgSP = false;
//	FUseN2PurgeDetector = false;   //2023-04-17 restore	 // 2023-01-30 Change from flow meter to flow switch
 //	FUseCDAFlowDetector = false;  //2023-04-17 restore
//	FUseSafetyReset = false;     //2023-04-17 restore
	FCheckFtpConnect = false;
	FI_N2_FLOW_DETECTED = 0;

	TowerLampOption = std::shared_ptr<CTowerLamp> (new CTowerLamp());

	FCureStartAlarmTime = 0;		// 2021-12-01 KJH 큐어 시작 알람 초기화
	FAutoModeChangeTimer = 0;

	FUseCsvDataSave = false;
	CsvMachineId = "";

	FUseCureStartEnableFromLocalState = false;  //로컬 상태에서 GEM Remote메세지로 큐어 시작 가능 사용유무
	FAutoCureStartAfterValidation = true;
}

NormalOvenSystemManager::~NormalOvenSystemManager()
{
	DestroySerialDevices();
	DestroySCKRMSThread();
	DestroyWatchFtpConnect();
}

NormalOvenChamber* NormalOvenSystemManager::GetAChamber(int chamberId)
{
	if( chamberId >=CHAMBER_A && chamberId < FChamberCount )
		return (NormalOvenChamber*)FChamber[chamberId];

	return nullptr;
}

bool NormalOvenSystemManager::LoadSystemParameters()
{
	AnsiString str;
	String iniPath = GetDataFolder() + OVENSYSTEMINI;
	std::auto_ptr<TIniFile> SysFile(new TIniFile(iniPath));

	try
	{
		if( SysFile->ValueExists(SEC_SYSTEM, KEY_LANGUAGE) )
			str = SysFile->ReadString(SEC_SYSTEM, KEY_LANGUAGE, LangStr[0]);
		else
			SysFile->WriteString(SEC_SYSTEM, KEY_LANGUAGE, LangStr[FLanguage]);
		FLanguage = 0;
		for(int i=0; i<LANG_COUNT; i++)
			if( str == AnsiString(LangStr[i]) )
				Language = i;

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_CHAMBERCOUNT) )
			FChamberCount = SysFile->ReadInteger(SEC_SYSTEM, KEY_CHAMBERCOUNT, FChamberCount);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_CHAMBERCOUNT, FChamberCount);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_LOGGERCOUNT) )
			FLoggerCount = SysFile->ReadInteger(SEC_SYSTEM, KEY_LOGGERCOUNT, FLoggerCount);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_LOGGERCOUNT, FLoggerCount);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_LOGGERPOINTCOUNT) )
			FLoggerPointCount = SysFile->ReadInteger(SEC_SYSTEM, KEY_LOGGERPOINTCOUNT, FLoggerPointCount);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_LOGGERPOINTCOUNT, FLoggerPointCount);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_MAINTEMPPORT) )
			FMainTempCtrlPort = SysFile->ReadInteger(SEC_SYSTEM, KEY_MAINTEMPPORT, FMainTempCtrlPort);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_MAINTEMPPORT, FMainTempCtrlPort);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_LOGGERPORT) )
			FLoggerPort = SysFile->ReadInteger(SEC_SYSTEM, KEY_LOGGERPORT, FLoggerPort);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_LOGGERPORT, FLoggerPort);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_TEMPLIMITPPORT) )
			FChamberTempLimitCtrlPort = SysFile->ReadInteger(SEC_SYSTEM, KEY_TEMPLIMITPPORT, FChamberTempLimitCtrlPort);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_TEMPLIMITPPORT, FChamberTempLimitCtrlPort);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_DAMPERMOTORPORT) )
			FDamperMotorCtrlPort = SysFile->ReadInteger(SEC_SYSTEM, KEY_DAMPERMOTORPORT, FDamperMotorCtrlPort);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_DAMPERMOTORPORT, FDamperMotorCtrlPort);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_DIOPORT) )
			FDIOCtrlPort = SysFile->ReadInteger(SEC_SYSTEM, KEY_DIOPORT, FDIOCtrlPort);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_DIOPORT, FDIOCtrlPort);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_O2ANALYZER_A_TIMEOUT) )
			FO2AnalyzerAPort = SysFile->ReadInteger(SEC_SYSTEM, KEY_O2ANALYZER_A_TIMEOUT, FO2AnalyzerAPort);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_O2ANALYZER_A_TIMEOUT, FO2AnalyzerAPort);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_O2ANALYZER_B_TIMEOUT) )
			FO2AnalyzerBPort = SysFile->ReadInteger(SEC_SYSTEM, KEY_O2ANALYZER_B_TIMEOUT, FO2AnalyzerBPort);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_O2ANALYZER_B_TIMEOUT, FO2AnalyzerBPort);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_O2ANALYZER_C_TIMEOUT) )
			FO2AnalyzerCPort = SysFile->ReadInteger(SEC_SYSTEM, KEY_O2ANALYZER_C_TIMEOUT, FO2AnalyzerCPort);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_O2ANALYZER_C_TIMEOUT, FO2AnalyzerCPort);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_O2ANALYZER_D_TIMEOUT) )
			FO2AnalyzerDPort = SysFile->ReadInteger(SEC_SYSTEM, KEY_O2ANALYZER_D_TIMEOUT, FO2AnalyzerDPort);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_O2ANALYZER_D_TIMEOUT, FO2AnalyzerDPort);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_INVERTER_PORT) )
			FInverterPort = SysFile->ReadInteger(SEC_SYSTEM, KEY_INVERTER_PORT, FInverterPort);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_INVERTER_PORT, FInverterPort);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_EMOOPTION) )
			FEmoOption = SysFile->ReadInteger(SEC_SYSTEM, KEY_EMOOPTION, FEmoOption);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_EMOOPTION, FEmoOption);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USEAUTOMATION) )
			FUseAutomation = SysFile->ReadBool(SEC_SYSTEM, KEY_USEAUTOMATION, FUseAutomation);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USEAUTOMATION, FUseAutomation);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_AUTOMATIONKIND) )
			FAutomationKind = SysFile->ReadInteger(SEC_SYSTEM, KEY_AUTOMATIONKIND, FAutomationKind);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_AUTOMATIONKIND, FAutomationKind);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_MAINTEMPTIMEOUTINT) )
			FMainTempCtrlTimeout = SysFile->ReadInteger(SEC_SYSTEM, KEY_MAINTEMPTIMEOUTINT, FMainTempCtrlTimeout);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_MAINTEMPTIMEOUTINT, FMainTempCtrlTimeout);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_LIMITTIMEOUTINT) )
			FLimitCtrlTimeout = SysFile->ReadInteger(SEC_SYSTEM, KEY_LIMITTIMEOUTINT, FLimitCtrlTimeout);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_LIMITTIMEOUTINT, FLimitCtrlTimeout);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_LOGGERTIMEOUTINT) )
			FLoggerTimeout = SysFile->ReadInteger(SEC_SYSTEM, KEY_LOGGERTIMEOUTINT, FLoggerTimeout);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_LOGGERTIMEOUTINT, FLoggerTimeout);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_DAMPERMOTORTIMEOUT) )
			FDamperMotorTimeout = SysFile->ReadInteger(SEC_SYSTEM, KEY_DAMPERMOTORTIMEOUT, FDamperMotorTimeout);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_DAMPERMOTORTIMEOUT, FDamperMotorTimeout);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_DIOTIMEOUT) )
			FDIOTimeout = SysFile->ReadInteger(SEC_SYSTEM, KEY_DIOTIMEOUT, FDIOTimeout);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_DIOTIMEOUT, FDIOTimeout);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_DPPORT) )
			FDPCtrlPort = SysFile->ReadInteger(SEC_SYSTEM, KEY_DPPORT, FDPCtrlPort);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_DPPORT, FDPCtrlPort);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_DPTIMEOUT) )
			FDPTimeout = SysFile->ReadInteger(SEC_SYSTEM, KEY_DPTIMEOUT, FDPTimeout);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_DPTIMEOUT, FDPTimeout);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_DPTYPE) )
			FDPType = SysFile->ReadBool(SEC_SYSTEM, KEY_DPTYPE, FDPType);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_DPTYPE, FDPType);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_INVERTER) )
			FUseInverter = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_INVERTER, FUseInverter);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_INVERTER, FUseInverter);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_O2_ANALYZER) )
			FUseO2Analyzer = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_O2_ANALYZER, FUseO2Analyzer);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_O2_ANALYZER, FUseO2Analyzer);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_DAMPER_CONTROL_TYPE) )
			FDamperControlType = SysFile->ReadInteger(SEC_SYSTEM, KEY_DAMPER_CONTROL_TYPE, FDamperControlType);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_DAMPER_CONTROL_TYPE, FDamperControlType);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_WATERFLOW_DETECTOR) )
			FUseWaterFlowDetector = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_WATERFLOW_DETECTOR, FUseWaterFlowDetector);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_WATERFLOW_DETECTOR, FUseWaterFlowDetector);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_N2FLOW_DETECTOR) )
			FUseN2FlowDetector = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_N2FLOW_DETECTOR, FUseN2FlowDetector);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_N2FLOW_DETECTOR, FUseN2FlowDetector);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_N2FLOW_CONTROL) )
			FUseN2FlowControl = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_N2FLOW_CONTROL, FUseN2FlowControl);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_N2FLOW_CONTROL, FUseN2FlowControl);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_MAGAZINE_DETECTOR) )
			FUseMagazineDetector = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_MAGAZINE_DETECTOR, FUseMagazineDetector);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_MAGAZINE_DETECTOR, FUseMagazineDetector);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_AIRPRESSURE_DETECTOR) )
			FUseAirPressureDetector = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_AIRPRESSURE_DETECTOR, FUseAirPressureDetector);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_AIRPRESSURE_DETECTOR, FUseAirPressureDetector);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_HEATER_OVERLOAD_DETECTOR) )
			FUseHeaterOverloadDetector = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_HEATER_OVERLOAD_DETECTOR, FUseHeaterOverloadDetector);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_HEATER_OVERLOAD_DETECTOR, FUseHeaterOverloadDetector);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_N2PRESSURE_DETECTOR) )
			FUseN2PressureDetector = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_N2PRESSURE_DETECTOR, FUseN2PressureDetector);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_N2PRESSURE_DETECTOR, FUseN2PressureDetector);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_ELECTRIC_PANEL_DOOR_DETECTOR) )
			FUseElectricPanelDoorDetector = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_ELECTRIC_PANEL_DOOR_DETECTOR, FUseElectricPanelDoorDetector);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_ELECTRIC_PANEL_DOOR_DETECTOR, FUseElectricPanelDoorDetector);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_DIFF_PRESSURE_DETECTOR) )
			FUseDiffPressureDetector = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_DIFF_PRESSURE_DETECTOR, FUseDiffPressureDetector);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_DIFF_PRESSURE_DETECTOR, FUseDiffPressureDetector);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_SSR_OVERTEMP_DETECTOR) )
			FUseSSROverTempDetector = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_SSR_OVERTEMP_DETECTOR, FUseSSROverTempDetector);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_SSR_OVERTEMP_DETECTOR, FUseSSROverTempDetector);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_GROUND_ALARM_DETECTOR) )
			FUseGroundAlarmDetector = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_GROUND_ALARM_DETECTOR, FUseGroundAlarmDetector);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_GROUND_ALARM_DETECTOR, FUseGroundAlarmDetector);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_BLOWMOTOR_LOWCURRENT_DETECTOR) )
			FUseBlowMotorLowCurrentDetector = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_BLOWMOTOR_LOWCURRENT_DETECTOR, FUseBlowMotorLowCurrentDetector);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_BLOWMOTOR_LOWCURRENT_DETECTOR, FUseBlowMotorLowCurrentDetector);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_DOORLOCK_CYLINDER) )
			FUseDoorLockCylinder = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_DOORLOCK_CYLINDER, FUseDoorLockCylinder);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_DOORLOCK_CYLINDER, FUseDoorLockCylinder);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_TOWERLAMP_CONTROL) )
			FUseTowerLampControl = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_TOWERLAMP_CONTROL, FUseTowerLampControl);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_TOWERLAMP_CONTROL, FUseTowerLampControl);

		// 2020-12-16 Add 챔버온도 도어락
		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_DOORLOCKTEMP_CONTROL) )
			FUseDoorLockTempControl = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_DOORLOCKTEMP_CONTROL, FUseDoorLockTempControl);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_DOORLOCKTEMP_CONTROL, FUseDoorLockTempControl);

		// 2021-03-05 Add IdleCooling일때 패스워스 사용유무
		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_PASSWORD_WHEN_IDLECOOLING_STOP) )
			FUsePasswordWhenIdleCoolingStop = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_PASSWORD_WHEN_IDLECOOLING_STOP, FUsePasswordWhenIdleCoolingStop);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_PASSWORD_WHEN_IDLECOOLING_STOP, FUsePasswordWhenIdleCoolingStop);

		// 2021-03-15 ADD Logger 데이터 저장 사용유무
		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_LOGGER_DATA_SAVE) )
			FUseLoggerDataSave = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_LOGGER_DATA_SAVE, FUseLoggerDataSave);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_LOGGER_DATA_SAVE, FUseLoggerDataSave);

		// 2021-09-09 Local Mode N2 Alarm Check
		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_LOCAL_MODE_ALARM) )
			FUseLocalN2Alarm = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_LOCAL_MODE_ALARM, FUseLocalN2Alarm);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_LOCAL_MODE_ALARM, FUseLocalN2Alarm);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_DRAW_BG_SP) )
			FDrawBgSP = SysFile->ReadBool(SEC_SYSTEM, KEY_DRAW_BG_SP, FDrawBgSP);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_DRAW_BG_SP, FDrawBgSP);

		// 20180209 - FTP (OFILM MES) - jhlim
		if( SysFile->ValueExists(SEC_SYSTEM, KEY_FTP_DEVICEID))
			ftpConnectInfo.DeviceID = SysFile->ReadString(SEC_SYSTEM, KEY_FTP_DEVICEID, ftpConnectInfo.DeviceID);
		else
			SysFile->WriteString(SEC_SYSTEM, KEY_FTP_DEVICEID, ftpConnectInfo.DeviceID);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_FTP_IP))
			ftpConnectInfo.Ftp_IP = SysFile->ReadString(SEC_SYSTEM, KEY_FTP_IP, ftpConnectInfo.Ftp_IP);
		else
			SysFile->WriteString(SEC_SYSTEM, KEY_FTP_IP, ftpConnectInfo.Ftp_IP);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_FTP_PORT))
			ftpConnectInfo.Ftp_PORT = SysFile->ReadInteger(SEC_SYSTEM, KEY_FTP_PORT, ftpConnectInfo.Ftp_PORT);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_FTP_PORT, ftpConnectInfo.Ftp_PORT);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_FTP_ID))
			ftpConnectInfo.Ftp_ID = SysFile->ReadString(SEC_SYSTEM, KEY_FTP_ID, ftpConnectInfo.Ftp_ID);
		else
			SysFile->WriteString(SEC_SYSTEM, KEY_FTP_ID, ftpConnectInfo.Ftp_ID);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_FTP_PW))
			ftpConnectInfo.Ftp_PW = SysFile->ReadString(SEC_SYSTEM, KEY_FTP_PW, ftpConnectInfo.Ftp_PW);
		else
			SysFile->WriteString(SEC_SYSTEM, KEY_FTP_PW, ftpConnectInfo.Ftp_PW);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_FTP_FOLDER))
			ftpConnectInfo.Ftp_FOLDER = SysFile->ReadString(SEC_SYSTEM, KEY_FTP_FOLDER, ftpConnectInfo.Ftp_FOLDER);
		else
			SysFile->WriteString(SEC_SYSTEM, KEY_FTP_FOLDER, ftpConnectInfo.Ftp_FOLDER);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_CHAMBER_DOOR_TEMP_INTERLOCK) )
			FUseChamberDoorTempInterlock = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_CHAMBER_DOOR_TEMP_INTERLOCK, FUseChamberDoorTempInterlock);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_CHAMBER_DOOR_TEMP_INTERLOCK, FUseChamberDoorTempInterlock);

		// 20181029 - ScreenSaver (infineon) - jhlim
		if( SysFile->ValueExists(SEC_SYSTEM, KEY_SCREENSAVER_USE) )
			FUseScreenSaver = SysFile->ReadBool(SEC_SYSTEM, KEY_SCREENSAVER_USE, FUseScreenSaver);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_SCREENSAVER_USE, FUseScreenSaver);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_SCREENSAVER_TIME) )
			FScreenSaverTime = SysFile->ReadInteger(SEC_SYSTEM, KEY_SCREENSAVER_TIME, FScreenSaverTime);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_SCREENSAVER_TIME, FScreenSaverTime);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_DIO_TYPE) )
			FDIOType = SysFile->ReadInteger(SEC_SYSTEM, KEY_DIO_TYPE, FDIOType);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_DIO_TYPE, FDIOType);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_JOBEND_TIMER) )
			FJobEndTimer = SysFile->ReadInteger(SEC_SYSTEM, KEY_JOBEND_TIMER, FJobEndTimer);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_JOBEND_TIMER, FJobEndTimer);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_DOORLOCK_TEMP) )		// 2020-12-16 Add 챔버온도 도어락
			FDoorLockTemp = SysFile->ReadInteger(SEC_SYSTEM, KEY_DOORLOCK_TEMP, FDoorLockTemp);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_DOORLOCK_TEMP, FDoorLockTemp);

		// 2021-06-09 KJH 쿨링워터 배출시간
		if( SysFile->ValueExists(SEC_SYSTEM, KEY_COOLING_VENT_EXUAST_TIME) )
			FCoolingVentExuastTime = SysFile->ReadInteger(SEC_SYSTEM, KEY_COOLING_VENT_EXUAST_TIME, FCoolingVentExuastTime);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_COOLING_VENT_EXUAST_TIME, FCoolingVentExuastTime);

		////HANA OVEN 2021-04-14
		if( SysFile->ValueExists(SEC_SYSTEM, KEY_HANA_OVEN_A_PORT) )
			FREXP250APort = SysFile->ReadInteger(SEC_SYSTEM, KEY_HANA_OVEN_A_PORT, FREXP250APort);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_HANA_OVEN_A_PORT, FREXP250APort);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_HANA_OVEN_B_PORT) )
			FREXP250BPort = SysFile->ReadInteger(SEC_SYSTEM, KEY_HANA_OVEN_B_PORT, FREXP250BPort);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_HANA_OVEN_B_PORT, FREXP250BPort);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_HANA_OVEN_C_PORT) )
			FREXP250CPort = SysFile->ReadInteger(SEC_SYSTEM, KEY_HANA_OVEN_C_PORT, FREXP250CPort);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_HANA_OVEN_C_PORT, FREXP250CPort);

		if( SysFile->ValueExists(SEC_SYSTEM, KEY_HANA_OVEN_D_PORT) )
			FREXP250DPort = SysFile->ReadInteger(SEC_SYSTEM, KEY_HANA_OVEN_D_PORT, FREXP250DPort);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_HANA_OVEN_D_PORT, FREXP250DPort);

		// JSCK RFID Reader Port
		if( SysFile->ValueExists(SEC_SYSTEM, KEY_RFID_READER_A_PORT) )
			FRfidReaderAPort = SysFile->ReadInteger(SEC_SYSTEM, KEY_RFID_READER_A_PORT, FRfidReaderAPort);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_RFID_READER_A_PORT, FRfidReaderAPort);

		// 2021-12-01
		if( SysFile->ValueExists(SEC_SYSTEM, KEY_CURE_START_ALARM_TIME) )
			FCureStartAlarmTime = SysFile->ReadInteger(SEC_SYSTEM, KEY_CURE_START_ALARM_TIME, FCureStartAlarmTime);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_CURE_START_ALARM_TIME, FCureStartAlarmTime);
		// 2021-12-22 hcw.
		if( SysFile->ValueExists(SEC_SYSTEM, KEY_AUTO_MODE_CHANGE_TIMER) )
			FAutoModeChangeTimer = SysFile->ReadInteger(SEC_SYSTEM, KEY_AUTO_MODE_CHANGE_TIMER, FAutoModeChangeTimer);
		else
			SysFile->WriteInteger(SEC_SYSTEM, KEY_AUTO_MODE_CHANGE_TIMER, FAutoModeChangeTimer);
		//2023-04-17 restore  // 2023-01-30 Change from flow meter to flow switch
//		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_N2_PURGE_DETECTOR) )
//			FUseN2PurgeDetector = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_N2_PURGE_DETECTOR, FUseN2PurgeDetector);
//		else
//			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_N2_PURGE_DETECTOR, FUseN2PurgeDetector);

//		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_CDA_FLOW_DETECTOR) )  //2023-04-17 restore
//			FUseCDAFlowDetector = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_CDA_FLOW_DETECTOR, FUseCDAFlowDetector);
//		else
//			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_CDA_FLOW_DETECTOR, FUseCDAFlowDetector);

//		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_SAFETY_RESET) )   //2023-04-17 restore
//			FUseSafetyReset = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_SAFETY_RESET, FUseSafetyReset);
//		else
//			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_SAFETY_RESET, FUseSafetyReset);

		//로컬 상태에서 GEM Remote메세지로 큐어 시작 가능 사용유무
		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_CURE_START_ENABLE_FROM_LOCAL_STATE) )
			FUseCureStartEnableFromLocalState = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_CURE_START_ENABLE_FROM_LOCAL_STATE, FUseCureStartEnableFromLocalState);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_CURE_START_ENABLE_FROM_LOCAL_STATE, FUseCureStartEnableFromLocalState);
		//JSCK
		if( SysFile->ValueExists(SEC_SYSTEM, KEY_AUTO_CURE_START_AFTER_VALID) )
			FAutoCureStartAfterValidation = SysFile->ReadBool(SEC_SYSTEM, KEY_AUTO_CURE_START_AFTER_VALID, FAutoCureStartAfterValidation);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_AUTO_CURE_START_AFTER_VALID, FAutoCureStartAfterValidation);
		//COWELL
		if( SysFile->ValueExists(SEC_SYSTEM, KEY_USE_REMOTE_CHECK) )
			FUseRemoteCheck = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_REMOTE_CHECK, FUseRemoteCheck);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_REMOTE_CHECK, FUseRemoteCheck);

		if (SysFile->ValueExists(SEC_SYSTEM, KEY_USE_CSV_DATA_SAVE) )
			FUseCsvDataSave = SysFile->ReadBool(SEC_SYSTEM, KEY_USE_CSV_DATA_SAVE, FUseCsvDataSave);
		else
			SysFile->WriteBool(SEC_SYSTEM, KEY_USE_CSV_DATA_SAVE, FUseCsvDataSave);

		if (SysFile->ValueExists(SEC_SYSTEM, KEY_MACHINE_ID) )
			CsvMachineId = SysFile->ReadString(SEC_SYSTEM, KEY_MACHINE_ID, CsvMachineId);
        else
			SysFile->WriteString(SEC_SYSTEM, KEY_MACHINE_ID, CsvMachineId);
	}
	catch (Exception* e)
	{
		SetMessageStr(e->Message);
		ShowMessageDlg();
		return false;
	}

	return true;
}

bool NormalOvenSystemManager::SaveSystemParameters()
{
	AnsiString str;
	String iniPath = GetDataFolder() + OVENSYSTEMINI;
	std::auto_ptr<TIniFile> SysFile(new TIniFile(iniPath));

	try
	{
		SysFile->WriteString(SEC_SYSTEM, KEY_LANGUAGE, LangStr[FLanguage]);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_CHAMBERCOUNT, FChamberCount);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_LOGGERCOUNT, FLoggerCount);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_LOGGERPOINTCOUNT, FLoggerPointCount);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_MAINTEMPPORT, FMainTempCtrlPort);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_LOGGERPORT, FLoggerPort);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_TEMPLIMITPPORT, FChamberTempLimitCtrlPort);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_DAMPERMOTORPORT, FDamperMotorCtrlPort);
//		SysFile->WriteInteger(SEC_SYSTEM, KEY_GAS, FGasType);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_EMOOPTION, FEmoOption);
		SysFile->WriteBool(SEC_SYSTEM, KEY_USEAUTOMATION, FUseAutomation);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_AUTOMATIONKIND, FAutomationKind);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_MAINTEMPTIMEOUTINT, FMainTempCtrlTimeout);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_LIMITTIMEOUTINT, FLimitCtrlTimeout);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_LOGGERTIMEOUTINT, FLoggerTimeout);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_DAMPERMOTORTIMEOUT, FDamperMotorTimeout);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_DIOPORT, FDIOCtrlPort);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_DIOTIMEOUT, FDIOTimeout);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_DPPORT, FDPCtrlPort);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_DPTIMEOUT, FDPTimeout);
		SysFile->WriteBool(SEC_SYSTEM, KEY_DPTYPE, FDPType);
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_INVERTER, FUseInverter);
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_O2_ANALYZER, FUseO2Analyzer);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_O2ANALYZER_A_TIMEOUT, FO2AnalyzerAPort);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_O2ANALYZER_B_TIMEOUT, FO2AnalyzerBPort);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_O2ANALYZER_C_TIMEOUT, FO2AnalyzerCPort);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_O2ANALYZER_D_TIMEOUT, FO2AnalyzerDPort);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_INVERTER_PORT, FInverterPort);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_DAMPER_CONTROL_TYPE, FDamperControlType);
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_WATERFLOW_DETECTOR, FUseWaterFlowDetector);
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_N2FLOW_DETECTOR, FUseN2FlowDetector);
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_N2FLOW_CONTROL, FUseN2FlowControl);
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_MAGAZINE_DETECTOR, FUseMagazineDetector);
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_AIRPRESSURE_DETECTOR, FUseAirPressureDetector);
        SysFile->WriteBool(SEC_SYSTEM, KEY_USE_N2PRESSURE_DETECTOR, FUseN2PressureDetector);
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_ELECTRIC_PANEL_DOOR_DETECTOR, FUseElectricPanelDoorDetector);
        SysFile->WriteBool(SEC_SYSTEM, KEY_USE_HEATER_OVERLOAD_DETECTOR, FUseHeaterOverloadDetector);
        SysFile->WriteBool(SEC_SYSTEM, KEY_USE_DIFF_PRESSURE_DETECTOR, FUseDiffPressureDetector);
        SysFile->WriteBool(SEC_SYSTEM, KEY_USE_SSR_OVERTEMP_DETECTOR, FUseSSROverTempDetector);
        SysFile->WriteBool(SEC_SYSTEM, KEY_USE_CHAMBER_DOOR_TEMP_INTERLOCK, FUseChamberDoorTempInterlock);
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_GROUND_ALARM_DETECTOR, FUseGroundAlarmDetector);
        SysFile->WriteBool(SEC_SYSTEM, KEY_USE_BLOWMOTOR_LOWCURRENT_DETECTOR, FUseBlowMotorLowCurrentDetector);
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_DOORLOCK_CYLINDER, FUseDoorLockCylinder);
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_TOWERLAMP_CONTROL, FUseTowerLampControl);
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_DOORLOCKTEMP_CONTROL, FUseDoorLockTempControl);	// 2020-12-16 Add 챔버온도 도어락
		// 2021-03-05 Add IdleCooling일때 패스워스 사용유무
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_PASSWORD_WHEN_IDLECOOLING_STOP, FUsePasswordWhenIdleCoolingStop);
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_LOGGER_DATA_SAVE, FUseLoggerDataSave);	// 2021-03-15 ADD Logger 데이터 저장 사용유무
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_LOCAL_MODE_ALARM, FUseLocalN2Alarm);
		SysFile->WriteBool(SEC_SYSTEM, KEY_DRAW_BG_SP, FDrawBgSP);

		// 20180209 - FTP (OFILM MES) - jhlim
		SysFile->WriteString(SEC_SYSTEM, 	KEY_FTP_DEVICEID, 	ftpConnectInfo.DeviceID);
		SysFile->WriteString(SEC_SYSTEM, 	KEY_FTP_IP, 		ftpConnectInfo.Ftp_IP);
		SysFile->WriteInteger(SEC_SYSTEM, 	KEY_FTP_PORT, 		ftpConnectInfo.Ftp_PORT);
		SysFile->WriteString(SEC_SYSTEM, 	KEY_FTP_ID, 		ftpConnectInfo.Ftp_ID);
		SysFile->WriteString(SEC_SYSTEM, 	KEY_FTP_PW, 		ftpConnectInfo.Ftp_PW);
		SysFile->WriteString(SEC_SYSTEM, 	KEY_FTP_FOLDER, 	ftpConnectInfo.Ftp_FOLDER);
		// 20181029 - ScreenSaver (infineon) - jhlim
		SysFile->WriteBool(SEC_SYSTEM, KEY_SCREENSAVER_USE, FUseScreenSaver);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_SCREENSAVER_TIME, FScreenSaverTime);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_DIO_TYPE, FDIOType);

		SysFile->WriteInteger(SEC_SYSTEM, KEY_JOBEND_TIMER, FJobEndTimer);

		SysFile->WriteInteger(SEC_SYSTEM, KEY_DOORLOCK_TEMP, FDoorLockTemp);		// 2020-12-16 Add 챔버온도 도어락

		// 2021-06-09 KJH 쿨링워터 배출시간
		SysFile->WriteInteger(SEC_SYSTEM, KEY_COOLING_VENT_EXUAST_TIME, FCoolingVentExuastTime);

		//HANA OVEN 2021-04-12
		SysFile->WriteInteger(SEC_SYSTEM, KEY_HANA_OVEN_A_PORT, FREXP250APort);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_HANA_OVEN_B_PORT, FREXP250BPort);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_HANA_OVEN_C_PORT, FREXP250CPort);
		SysFile->WriteInteger(SEC_SYSTEM, KEY_HANA_OVEN_D_PORT, FREXP250DPort);

		//JSCK RFID Redaer Port
		SysFile->WriteInteger(SEC_SYSTEM, KEY_RFID_READER_A_PORT, FRfidReaderAPort);

		//2021-12-01
		SysFile->WriteInteger(SEC_SYSTEM, KEY_CURE_START_ALARM_TIME, FCureStartAlarmTime);

		SysFile->WriteInteger(SEC_SYSTEM, KEY_AUTO_MODE_CHANGE_TIMER, FAutoModeChangeTimer);
		//2023-01-30
//		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_N2_PURGE_DETECTOR, FUseN2PurgeDetector);  //2023-04-17 restore
//		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_CDA_FLOW_DETECTOR, FUseCDAFlowDetector); //2023-04-17 restore
//		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_SAFETY_RESET, FUseSafetyReset);  //2023-04-17 restore
        //로컬 상태에서 GEM Remote메세지로 큐어 시작 가능 사용유무
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_CURE_START_ENABLE_FROM_LOCAL_STATE, FUseCureStartEnableFromLocalState);
		//JSCK
		SysFile->WriteBool(SEC_SYSTEM, KEY_AUTO_CURE_START_AFTER_VALID, FAutoCureStartAfterValidation);
		//COWELL
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_REMOTE_CHECK, FUseRemoteCheck);
		SysFile->WriteBool(SEC_SYSTEM, KEY_USE_CSV_DATA_SAVE, UseCsvDataSave);
		SysFile->WriteString(SEC_SYSTEM, KEY_MACHINE_ID, CsvMachineId);
	}
	catch (Exception* e)
	{
		SetMessageStr(e->Message);
		ShowMessageDlg();
		return false;
	}

	return true;
}

void NormalOvenSystemManager::CreateChamber()
{
 	N2FlowSetting();

	for(int i=0; i<FChamberCount; i++)
	{
		FChamber[i] = new NormalOvenChamber(i);
		FChamber[i]->LoadChamberParameters();
	}

	if( GetManager()->DamperControlType == EZI_STEP_MOTOR_DAMPER )
	{
		EziStepMotor::Disconnect(FChamberCount);
    }
}

void NormalOvenSystemManager::DestroyChamber()
{
	for(int i=0; i<FChamberCount; i++)
	{
		if( FChamber[i] )
		{
			delete FChamber[i];
			FChamber[i] = nullptr;
		}
	}
}

bool NormalOvenSystemManager::CreateSerialDevices()
{
	if( GetManager()->DamperControlType == EZI_STEP_MOTOR_DAMPER )
	{
		if (EziStepMotor::Connect(FDamperMotorCtrlPort))
		{
			EziStepMotor::SetParameter(FChamberCount);
		}
		else
		{
			String str;
			SetMessageStr(L"Damper Serial Connection Fail.");
			str.sprintf(L"[COM%d]", FDamperMotorCtrlPort);
			SetMessageStr(str);
			ShowMessageDlg();
        }
	}

//	if (GetManager()->UseAutomation == true && GetManager()->AutomationKind == SECS_GEM_SCK)
//	{
//		portInfo.portSetting = NessLabRFIDPortSetting;
//		portInfo.portNo = FRfidReaderAPort
//		portInfo.addr = i;      //챔버 구분을 위한 RFID 어드레스
//		portInfo.timeout = 3000;
//		chamber->RFIDReader = new NessLabRFID(chamber, portInfo);
//	}

	for(int i=0; i<FChamberCount; i++)
	{
		NormalOvenChamber *chamber = GetChamber(i);
		if( chamber )
		{
			try
			{
				int timeout = 500;
				PORTINFO portInfo;

				portInfo.portSetting = MainTempPortSetting;
				portInfo.portNo = FMainTempCtrlPort;
				portInfo.addr = i+1;
				portInfo.timeout = timeout;
				chamber->MainTempController = CreateMainTempController(chamber, portInfo);

				if (chamber->LimitControllerType != LIMIT_NONE)
				{
					int *regData = nullptr, *valData = nullptr, regCount = 0;
					if (chamber->LimitControllerType == LIMIT_ST)
					{
						// 주의
						// 센서타입 업데이트하면, IN-U, IN-RH, IN-RL, SP, HYS-H, HYS-L, ONOFF 값들이 초기화 된다.
						// 입력상한 하한 업데이트 하면, SP값이 0으로 초기화 된다.
						// 따라서 센서타입을 가장 앞부분에 넣어야 한다.

						// Format : "%c|%02d|Cmd|Data|%c|%c", ST_STX, Address, ST_CR, ST_LF
						// typedef	enum { AUTOMAN, IN_TYPE, ONOFF, IN_UNIT, IN_RH, IN_RL, IN_DP, IN_SH, IN_SL, HYS_H, HYS_L }STCTRL_PARA;
						// 				      0105     0601   0207     0602   0603   0604   0605   0607   0608   0615   0616

						const int REG_COUNT	= 19;
						int reg[REG_COUNT] = { 105, 601, 207, 602,  603, 604, 605,   607, 608, 609, 610, 615, 616, 401, 402,  406, 412, 417, 422 };
						int val[REG_COUNT] = {   0,   1,   1,   0,  500,   0,   0,  5000,   0,   1,   1,   0,   0,   1,   1, 5000,   0,   0,   0 };

						regCount = REG_COUNT;
						regData = new int[REG_COUNT];
						valData = new int[REG_COUNT];
						for (int i=0; i < regCount; i++)
						{
							regData[i] = reg[i];
							valData[i] = val[i];
						}
					}
					else if (chamber->LimitControllerType == LIMIT_VX)
					{
						const int REG_COUNT	= 18;
						//                     A/M R/S  INP UNIT DP-P SL-H SL-L SV.NO SV-H SV-L A1.TY A1.DB A2.TY A2.DB SUB1 SUB2 CNT1 CNT2
						//					  AUTO RUN   K0   ℃                                                                   ON/OFF
						int reg[REG_COUNT] = {  31, 33, 900, 901, 904, 905, 906, 100,  101, 102,  300,  302,  304,  306, 500, 501, 800, 801 };
						int val[REG_COUNT] = {   0,  1,   1,   0,   0, 500,   0,   1,  500,   0,    1,    0,    1,    0,   1,   2,   1,   1 };

						regCount = REG_COUNT;
						regData = new int[REG_COUNT];
						valData = new int[REG_COUNT];
						for (int i=0; i < regCount; i++)
						{
							regData[i] = reg[i];
							valData[i] = val[i];
						}
					}
					portInfo.portSetting = ST340PortSetting;
					portInfo.portNo = FChamberTempLimitCtrlPort;
					portInfo.addr = i+1;
					portInfo.timeout = timeout;
					chamber->ChamberTempLimitController = CreateLimitController(chamber, portInfo, CHAMBER_TEMP_LIMIT);
					chamber->ChamberTempLimitController->SetConfigData(regData, valData, regCount);
					chamber->ChamberTempLimitController->DP = 0;
					chamber->ChamberTempLimitController->Bias = chamber->ChamberTempLimitBias;
					chamber->ChamberTempLimitController->LimitOffset = chamber->ChamberTempLimitOffset;
					chamber->SetChamberTempLimit(300, 150);
				}

				portInfo.portSetting = LoggerPortSetting;
				portInfo.portNo = FLoggerPort;
				portInfo.timeout = timeout;
				for (int j=0; j < LoggerCount; j++)
				{
					portInfo.addr = j+1 + LoggerCount * i;
					chamber->LoggerController[j] = new I7018(chamber, portInfo);
					chamber->LoggerController[j]->LoggerIndex = j;
				}

				if( FDPType )
				{
					portInfo.portSetting = DPPortSetting;
					portInfo.portNo = FDPCtrlPort;
					portInfo.addr = i+1;
					portInfo.timeout = timeout;
					chamber->DPController = new DiffPressure(chamber, portInfo);
				}

				portInfo.portSetting = DamperMotorPortSetting;
				portInfo.portNo = FDamperMotorCtrlPort;
				portInfo.addr = i+1;
				portInfo.timeout = timeout;
				if( GetManager()->DamperControlType == MOONS_STEP_MOTOR_DAMPER )
				{
					chamber->DamperMotor = new MoonsStepMotor(chamber, portInfo);
				}
				else if( GetManager()->DamperControlType == EZI_STEP_MOTOR_DAMPER )
				{
					chamber->DamperMotor = new EziStepMotor(chamber, portInfo);
				}
				//
				if( FDIOType == ICPDAS )
				{
					portInfo.portSetting = DIOPortSetting;
					portInfo.portNo = FDIOCtrlPort;
					portInfo.addr = i*3+1;
					portInfo.timeout = 200;
					chamber->ModuleDI = new I7000DIOModule(chamber, portInfo, I_7051);

					portInfo.addr = i*3+2;
					chamber->ModuleDO = new I7000DIOModule(chamber, portInfo, I_7045);

					portInfo.addr = i*3+3;
					chamber->ModuleDIO = new I7000DIOModule(chamber, portInfo, I_7055);
				}
				else if( FDIOType == OVEN_DIO )
				{
					portInfo.portSetting = VSDIOPortSetting;
					portInfo.portNo = FDIOCtrlPort;
					portInfo.addr = i+1;
					portInfo.timeout = 300;
					chamber->OvenBoard = new VSOvenBoard(chamber, portInfo);
				}

				if( UseO2Analyzer )
				{
					if( i == 0 )
					{
						portInfo.portSetting = ZR5O2AnalyzerPortSetting;
						portInfo.portNo = FO2AnalyzerAPort;
						portInfo.timeout = timeout;
						chamber->O2Analyzer = new ZR5(chamber, portInfo);
					}
					else
					if( i == 1 )
					{
						portInfo.portSetting = ZR5O2AnalyzerPortSetting;
						portInfo.portNo = FO2AnalyzerBPort;
						portInfo.timeout = timeout;
						chamber->O2Analyzer = new ZR5(chamber, portInfo);
					}
					else
					if( i == 2 )
					{
						portInfo.portSetting = ZR5O2AnalyzerPortSetting;
						portInfo.portNo = FO2AnalyzerCPort;
						portInfo.timeout = timeout;
						chamber->O2Analyzer = new ZR5(chamber, portInfo);
					}
					else
					if( i == 3 )
					{
						portInfo.portSetting = ZR5O2AnalyzerPortSetting;
						portInfo.portNo = FO2AnalyzerDPort;
						portInfo.timeout = timeout;
						chamber->O2Analyzer = new ZR5(chamber, portInfo);
					}
				}

				if( GetManager()->UseInverter )
				{
					portInfo.portSetting = InverterPortSetting;
					portInfo.portNo = FInverterPort;
					portInfo.addr = i+1;
					portInfo.timeout = timeout;
					chamber->Inverter = new LSInverter(chamber, portInfo);
				}

				//HANA_OVEN 2021-08-09
				if(GetManager()->UseAutomation == true
					&& GetManager()->AutomationKind == HANA_CIM)
				{
					portInfo.portSetting = REXP250PortSetting;
					switch (i)
					{
					  case 0:  portInfo.portNo = FREXP250APort;
								break;
					  case 1:  portInfo.portNo = FREXP250BPort;
								break;
					  case 2:  portInfo.portNo = FREXP250CPort;
								break;
					  case 3:  portInfo.portNo = FREXP250DPort;
								break;
					  default:  break;
					}

					portInfo.timeout = timeout;
					chamber->HanOvenCim = new REX_P250(chamber, portInfo);
				}

				/*if (FChamberCount == CHAMBER_A
					&& GetManager()->UseAutomation == true
					&& GetManager()->AutomationKind == SECS_GEM_SCK)
				{
					portInfo.portSetting = NessLabRFIDPortSetting;
					portInfo.portNo = FRfidReaderAPort;

					portInfo.addr = 1;
					portInfo.timeout = 3000;
					chamber->RFIDReader = new NessLabRFID(chamber, portInfo);
				}*/
				//
			}
			catch(...)
			{
				return false;
			}
		}
	}

	return true;
}

void NormalOvenSystemManager::DestroySerialDevices()
{
	for(int i=0; i<FChamberCount; i++)
	{
		if( AChamber[i] )
		{
			if( AChamber[i]->MainTempController )
			{
				delete AChamber[i]->MainTempController;
				AChamber[i]->MainTempController = nullptr;
			}
			if( AChamber[i]->ChamberTempLimitController )
			{
				delete AChamber[i]->ChamberTempLimitController;
				AChamber[i]->ChamberTempLimitController = nullptr;
			}
			for(int j=0; j<FLoggerCount; j++)
			{
				if( AChamber[i]->LoggerController[j] )
				{
					delete AChamber[i]->LoggerController[j];
					AChamber[i]->LoggerController[j] = nullptr;
				}
			}

			if( AChamber[i]->ModuleDI )
			{
				delete AChamber[i]->ModuleDI;
				AChamber[i]->ModuleDI = nullptr;
			}

			if( AChamber[i]->ModuleDO )
			{
				delete AChamber[i]->ModuleDO;
				AChamber[i]->ModuleDO = nullptr;
			}

			if( AChamber[i]->ModuleDIO )
			{
				delete AChamber[i]->ModuleDIO;
				AChamber[i]->ModuleDIO = nullptr;
			}

			if( AChamber[i]->DPController )
			{
				delete AChamber[i]->DPController;
				AChamber[i]->DPController = nullptr;
			}

			if( AChamber[i]->DamperMotor )
			{
				delete AChamber[i]->DamperMotor;
				AChamber[i]->DamperMotor = nullptr;
			}

			if( AChamber[i]->OvenBoard )
			{
				delete AChamber[i]->OvenBoard;
				AChamber[i]->OvenBoard = nullptr;
			}

			if( AChamber[i]->HanOvenCim )  //2021-04-20 HANA OVEN
			{
				delete AChamber[i]->HanOvenCim;
				AChamber[i]->HanOvenCim = nullptr;
			}
		}
	}
}

// 20180208 - FTP (OFILM MES) - jhlim
bool NormalOvenSystemManager::CreateFTPConnection()
{
	String str;
	bool bReturn = false;

	mh_ftp_session = nullptr;
	mh_ftp_connection = nullptr;
	mh_ftp_session = ::InternetOpen(nullptr, INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);

   if(mh_ftp_session != nullptr)
   {
		// Save Log
		str = "[FTP] Open Success";
		GetOvenDB()->SaveLog(str, MESSAGES);

		mh_ftp_connection =
			::InternetConnect(
				mh_ftp_session
				, ftpConnectInfo.Ftp_IP.c_str()
				, ftpConnectInfo.Ftp_PORT
				, ftpConnectInfo.Ftp_ID.c_str()
				, ftpConnectInfo.Ftp_PW.c_str()
				, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);

		if(mh_ftp_connection != nullptr)
		{
			// Save Log
			str = "[FTP] Connect Success";
			GetOvenDB()->SaveLog(str, MESSAGES);

			// FTP 서버에 접속됨
			bReturn = true;

			// 파일 전송
			TrasferFileUpload();
		}
		else
		{
			// Save Log
			str = "[FTP] Connect Failed";
			GetOvenDB()->SaveLog(str, MESSAGES);
		}
		// 연결 해제
		DestroyFTPConnection();
   }
   else
   {
		// Save Log
		str = "[FTP] Open Failed";
		GetOvenDB()->SaveLog(str, MESSAGES);

		// 연결 해제
		DestroyFTPConnection();
   }

	return bReturn;
}

// 20180208 - FTP (OFILM MES) - jhlim
void NormalOvenSystemManager::DestroyFTPConnection()
{
	String str;

	if(mh_ftp_connection != nullptr)
	{
		if(::InternetCloseHandle(mh_ftp_connection) == true)
		{
			mh_ftp_connection = nullptr;

			// Save Log
			str = "[FTP] Connect Close";
			GetOvenDB()->SaveLog(str, MESSAGES);
		}
	}

	if(mh_ftp_session != nullptr)
	{
		if(::InternetCloseHandle(mh_ftp_session) == true)
		{
			mh_ftp_session = nullptr;

			// Save Log
			str = "[FTP] Open Close";
			GetOvenDB()->SaveLog(str, MESSAGES);
		}
	}
}

// 20180208 - FTP (OFILM MES) - jhlim
bool NormalOvenSystemManager::TrasferFileUpload()
{
	int nErrorNo = 0;
	bool bReturn = false;
	String Localfile, Remotefile, time, str;

	if(mh_ftp_connection != nullptr)
	{
		if(ftpConnectInfo.Ftp_FOLDER != "")
			::FtpSetCurrentDirectory(mh_ftp_connection, ftpConnectInfo.Ftp_FOLDER.c_str());

		time = FormatDateTime(L"yyyymmddhhnnss", Now());
		for(int i=0; i<ChamberCount; i++)
		{
			// Oven Data File
			Localfile.sprintf(L"%s%02d.TXT", GetDataFolder().c_str(), i+1);
			// FTP Send File
			Remotefile.sprintf(L"%s_%02d_%s.TXT"
				, ftpConnectInfo.DeviceID.c_str()
				, i+1
				, time.c_str());

			// FTP Server Send File
			bReturn = ::FtpPutFile(
				mh_ftp_connection
				, Localfile.w_str()
				, Remotefile.w_str()
				,FTP_TRANSFER_TYPE_BINARY, NULL);

			if(bReturn == true)
			{
			   str = "[FTP] Data File Upload Success";
			   GetOvenDB()->SaveLog(str, MESSAGES, i);
			}
			else
			{
			   str = "[FTP] Data File Upload Fail";
			   GetOvenDB()->SaveLog(str, MESSAGES, i);
            }
		}
	}
	return bReturn;
}

// 20180212 - FTP (OFILM MES) - jhlim
bool NormalOvenSystemManager::DeleteFTPDataFile()
{
	String sFileName;
	bool bReturn = false;

	for(int i=0; i<FChamberCount; i++)
	{
		if( AChamber[i] != nullptr )
		{
			sFileName.sprintf(L"%s%02d.TXT", GetDataFolder().c_str(), i+1);
			bReturn |= DeleteFile(sFileName);
		}
	}

	return bReturn;
}

void NormalOvenSystemManager::LoadTwrValue()
{
	String iniPath = GetDataFolder() + OVENSYSTEMINI;
	TowerLampOption->LoadValue(iniPath);
}

void NormalOvenSystemManager::SaveTwrValue()
{
	String iniPath = GetDataFolder() + OVENSYSTEMINI;
	TowerLampOption->SaveValue(iniPath);
}

void NormalOvenSystemManager::CreateSCKRMSThread()
{
	SckRmsWatch = new SckRmsThread(0);
}

void NormalOvenSystemManager::DestroySCKRMSThread()
{
	if( SckRmsWatch )
	{
		SckRmsWatch->Terminate();
		Sleep(500);

		delete	SckRmsWatch;
		SckRmsWatch = nullptr;
	}
}

bool NormalOvenSystemManager::IsUseGem()
{
	return (AutomationKind == SECS_GEM ||
		AutomationKind == SECS_GEM_AMD ||
		AutomationKind == SECS_GEM_COWELL);
}

void NormalOvenSystemManager::RunWatchFtpConnect()
{
	WatchFtpConnectThread = new WatchFtpConnect();
}

void NormalOvenSystemManager::DestroyWatchFtpConnect()
{
	if( WatchFtpConnectThread )
	{
		WatchFtpConnectThread->Terminate();
		Sleep(500);

		delete	WatchFtpConnectThread;
		WatchFtpConnectThread = nullptr;
	}
}

void NormalOvenSystemManager::N2FlowSetting()
{
	if (DIOType == ICPDAS)
	{
		FI_N2_FLOW_DETECTED = X017;
	}
	else if (DIOType == OVEN_DIO)
	{
		FI_N2_FLOW_DETECTED = X019;
	}
}

