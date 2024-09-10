//---------------------------------------------------------------------------
#pragma hdrstop
#include <System.DateUtils.hpp>
#include "Chamber.h"
#include "HelperFunctions.h"
#include "MainTempController.h"
#include "LimitController.h"
#include "Database.h"
#include "SystemManager.h"
#include "Defines.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

BaseChamber::BaseChamber(int chamberNo)
	: TThread(false)
{
	FChamberID = chamberNo;
	FEnabled = true;
	FErrorString = "";
 //	FStep = STEP_IDLE;
	FStep = -1;

	for(int i=0; i<MAX_ALARM_COUNT; i++)
		FAlarmStatus[i] = false;
	FNewAlarm = false;
}

BaseChamber::~BaseChamber()
{
	StepChanged = false;
}

void __fastcall BaseChamber::Execute()
{
	while( !Terminated )
	{
		WaitForSingleObject((void *)Handle, THREAD_WAIT_TIME);

		//if( FEnabled == true )
			DoWorkSequence();
	}
}

void BaseChamber::DelayTime(unsigned int milliSeconds, bool workInThread/*=true*/)
{
	TDateTime now = Now();
	while( 1 )
	{
		if( workInThread )
			WaitForSingleObject((void*)Handle, THREAD_WAIT_TIME);
		else
		{
			Application->ProcessMessages();
			Sleep(THREAD_WAIT_TIME);
		}

		if( MilliSecondsBetween(now, Now()) >= milliSeconds )
			break;
	}
}

void BaseChamber::SetStep(int step)
{
	FStep = step;
	StepChanged = true;
}

bool BaseChamber::SetAlarm(unsigned int alarmCode)
{
	std::vector<unsigned int>::iterator i;
	for(i=Alarms.begin(); i!=Alarms.end(); i++)
	{
		if( *i == alarmCode )
		{
		//	FNewAlarm = false;
			return false;
		}
	}

	FNewAlarm = true;
	Alarms.push_back(alarmCode);
	return true;
}

unsigned int BaseChamber::GetAlarm(int idx)
{
	return Alarms[idx];
}

void BaseChamber::ClearAlarm(unsigned int alarmCode)
{
	std::vector<unsigned int>::iterator i;
	for(i=Alarms.begin(); i!=Alarms.end(); i++)
	{
		if( *i == alarmCode )
		{
			Alarms.erase(i);
			FAlarmStatus[alarmCode] = false;
			break;
		}
	}
}

void BaseChamber::ClearAlarmAll()
{
	Alarms.clear();
	for(int i=0; i<MAX_ALARM_COUNT; i++)
	{
		FAlarmStatus[i] = false;
	}
}

bool BaseChamber::GetAlarmStatus(int idx)
{
	if( idx >= 0 &&
		idx < MAX_ALARM_COUNT )
		return FAlarmStatus[idx];
	return false;
}

void BaseChamber::SetAlarmStatus(int idx, bool status)
{
	if( idx >= 0 &&
		idx < MAX_ALARM_COUNT )
		FAlarmStatus[idx] = status;
}

OvenChamber::OvenChamber(int chamberNo)
	: BaseChamber(chamberNo)
{
	FMainTempControllerType = TEMPCTRL_UP550;
	FLimitControllerType = LIMIT_ST;
	FMainTempController = nullptr;
	FChamberTempLimitController = nullptr;
	FHeaterTempLimitController = nullptr;
	FPressureLimitController = nullptr;
	for(int i=0; i<MAX_LOGGER_COUNT; i++)
		FLoggerController[i] = nullptr;

	ProgressValue = 0;

	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
	{
		FLoggerOffset[i] = 0;
		FUseChannel[i] = FALSE;
	}

	FDoorOpenConditionTemp = 0;
	FWorkCompletionTemp = 0;

	FDoorOpenWhenIdleStart = false;
	FDoorOpenWhenIdleStartTime = Now();
    FDoorOpenDetectedInterval = 2;
}

OvenChamber::~OvenChamber()
{
	StepChanged = false;
}

void OvenChamber::LogStepChanged()
{
	if( StepChanged )
	{
		StepChanged = false;

		String str;
		str.sprintf(L"Step = %s", StatusStr[FStep].c_str());
		GetOvenDB()->SaveLog(str, MESSAGES, FChamberID);
	}
}

I7018* OvenChamber::GetLoggerController(int idx)
{
	if( idx >=0 && idx < MAX_LOGGER_COUNT )
	{
		return FLoggerController[idx];
	}
	return nullptr;
}

void OvenChamber::SetLoggerController(int idx, I7018 *logger)
{
	if( idx >=0 && idx < MAX_LOGGER_COUNT )
	{
		FLoggerController[idx] = logger;
	}
}

double OvenChamber::GetLoggerOffset(int idx)
{
	if( idx >=0 && idx < MAX_LOGGERPOINT_COUNT )
		return (double)((double)FLoggerOffset[idx] / (double)DEFAULT_SCALEFACTOR);
	return 0;
}

void OvenChamber::SetLoggerOffset(int idx, double offset)
{
	if( idx >=0 && idx < MAX_LOGGERPOINT_COUNT )
	{
		FLoggerOffset[idx] = (int)(offset * DEFAULT_SCALEFACTOR);
	}
}

double OvenChamber::GetLoggerData(int idx)
{
	if( idx >=0 && idx < MAX_LOGGERPOINT_COUNT )
	{
		int val = FLoggerData[idx] + FLoggerOffset[idx];
		return (double)(val / (double)DEFAULT_SCALEFACTOR);
	}
	return 0;
}

void OvenChamber::SetLoggerData(int idx, double data)
{
	if( idx >=0 && idx < MAX_LOGGERPOINT_COUNT )
		FLoggerData[idx] = (int)(data * DEFAULT_SCALEFACTOR);
}

void OvenChamber::SetUseChannel(int idx, bool use)
{
	if( idx >=0 && idx < MAX_LOGGERPOINT_COUNT )
	{
		FUseChannel[idx] = use;
	}
}

bool OvenChamber::GetUseChannel(int idx)
{
	if( idx >=0 && idx < MAX_LOGGERPOINT_COUNT )
		return FUseChannel[idx];
	return false;
}

double OvenChamber::GetChamberTempLimitOffset()
{
	return (double)((double)FChamberTempLimitOffset / (double)DEFAULT_SCALEFACTOR);
}

void OvenChamber::SetChamberTempLimitOffset(double offset)
{
	FChamberTempLimitOffset = (int)(offset * (double)DEFAULT_SCALEFACTOR);
	if( FChamberTempLimitController )
		FChamberTempLimitController->LimitOffset = offset;
}

double OvenChamber::GetChamberTempLimitBias()
{
	return (double)((double)FChamberTempLimitBias / (double)DEFAULT_SCALEFACTOR);
}

void OvenChamber::SetChamberTempLimitBias(double bias)
{
	FChamberTempLimitBias = (int)(bias * (double)DEFAULT_SCALEFACTOR);
	if( FChamberTempLimitController )
		FChamberTempLimitController->Bias = bias;
}
//----------------------------------------------------
double OvenChamber::GetChamberLoggerTempDeviation()
{
	return (double)FChamberLoggerDeviation;
}

void OvenChamber::SetChamberLoggerTempDeviation(double offset)
{
	FChamberLoggerDeviation = offset;
}
//----------------------------------------------------
void OvenChamber::InitCureValues()
{
	Recipe.StartTime = Now().FormatString("yyyy-mm-dd hh:nn:ss");;
	MakeDataFilename();
	Recipe.StopType = NORMAL_STOP;
	Recipe.JobEnd = false;
}

void OvenChamber::MakeDataFilename()
{
//	FILE * pFile;
	AnsiString file, name, folder;

	name.sprintf("%c_", 'A' + FChamberID);
	name += Recipe.StartTime.FormatString("yyyy_mm_dd_hh_nn_ss");

	folder = GetReportFolder();
	file = folder + "MainTemp_" + name + ".cht";
	Recipe.MainTempFilename = file;

	file = folder + "LIMIT_" + name + ".cht";
	Recipe.LimitFilename = file;

	file = folder + "VACUUM_" + name + ".cht";
	Recipe.VacuumFilename = file;

	file = folder + "HEATER_" + name + ".cht";
	Recipe.HeaterFilename = file;

	file = folder + "PRESS_" + name + ".cht";
	Recipe.PressFilename = file;

	file = folder + "O2_" + name + ".cht";
	Recipe.O2Filename = file;

	file = folder + "LOGGER_" + name + ".cht";
	Recipe.LoggerFilename = file;
}

bool OvenChamber::WriteMainTempData(TDateTime time)
{
	if( MainTempController == nullptr )
		return false;

	AnsiString data, tmp;

	data = (AnsiString)time.FormatString("hh:nn:ss");
	tmp.sprintf(",%.1f,%.1f,%.1f\n",
				 PV,
				 SP,
				 HeaterOut);
    data += tmp;

//	data.sprintf("%s,%.1f,%.1f,%.1f\n",
//				 time.FormatString("hh:nn:ss").c_str(),
//				 PV,
//				 SP,
//				 HeaterOut);

	FILE * pFile;
	pFile = fopen(Recipe.MainTempFilename.c_str(), "a+t");
	if( pFile == nullptr )
		return false;
	std::fwrite(data.c_str(), data.Length(), 1, pFile);
	fclose(pFile);
	return true;
}

bool OvenChamber::WriteLoggerData(TDateTime time)
{
	AnsiString data, tmp;

	data = (AnsiString)time.FormatString("hh:nn:ss");

	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
	{
		tmp.sprintf(",%.1f", LoggerData[i]);
		data += tmp;
	}

/*	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
	{
		if( FUseChannel[i] )
			tmp.sprintf(",%.1f", LoggerData[i]);
		else
			tmp = ",---";
		data += tmp;
	}*/
	data += "\n";

	FILE * pFile;
	pFile = fopen(Recipe.LoggerFilename.c_str(), "a+t");
	if( pFile == nullptr )
		return false;
	std::fwrite(data.c_str(), data.Length(), 1, pFile);
	fclose(pFile);
	return true;
}

bool OvenChamber::WriteLimitData(TDateTime time)
{
	if( FChamberTempLimitController == nullptr )
		return false;

	AnsiString data, tmp;
	data = (AnsiString)time.FormatString("hh:nn:ss");
	tmp.sprintf(",%.1f,%.1f\n",
				 ChamberLimitPV,
				 ChamberLimitSP);
	data += tmp;

	FILE * pFile;
	pFile = fopen(Recipe.LimitFilename.c_str(), "a+t");
	if( pFile == nullptr )
		return false;
	std::fwrite(data.c_str(), data.Length(), 1, pFile);
	fclose(pFile);
	return true;
}

bool OvenChamber::WritePressData(TDateTime time)
{
	if( FPressureLimitController == nullptr )
		return false;

	AnsiString data, tmp;
	data = (AnsiString)time.FormatString("hh:nn:ss");
	tmp.sprintf(",%.2f,%.2f\n",
				 PressureLimitPV,
				 PressureLimitSP);
 	data += tmp;

	FILE * pFile;
	pFile = fopen(Recipe.PressFilename.c_str(), "a+t");
	if( pFile == nullptr )
		return false;
	std::fwrite(data.c_str(), data.Length(), 1, pFile);
	fclose(pFile);
	return true;
}

void OvenChamber::WriteProductData(TDateTime endtime, String mcID, String userID,
									String lotid, bool errorCode, String detail)
{
	AnsiString file, cm, name, folder, date="", data="";
	String tmp, strValue, ecode="", eDetail="";

	cm.sprintf("%c", 'A' + FChamberID);

	if (errorCode == true)
	{
		ecode = "NG";
		name = cm + endtime.FormatString("_yyyymmdd");
	}
	else
	{
		ecode = "OK";
		detail = "";
		name = cm + endtime.FormatString("_yyyymmdd");
	}

	folder = GetProductFolder() + cm + "\\";

	if (DirectoryExists(folder) == false)
	{
		CreateDir(folder);
	}

	unsigned short year, mon, day;
	endtime.DecodeDate(&year, &mon, &day);

	TDateTime prevEndTime = endtime;
	String temptime = prevEndTime.FormatString("hhnn");
	int comparetime = StrToInt(temptime);

	if (comparetime < 730)  // 7시30분 이전 데이터는 전날 파일에 저장 요청
	{
		TDateTime tempdate;
		tempdate = IncDay(endtime, -1);
		name = "Lot ID" + tempdate.FormatString("_yyyymmdd");
	}
	file = folder + name + ".csv";

	date = (AnsiString)Now().FormatString("yyyy/mm/dd,");	//Date

	TStringList *LotIDList = new TStringList;
	LotIDList->CommaText = lotid;

	if (LotIDList->Count == 0)
	{   //Lot ID가 없을때
		data += date + Recipe.StartTime.FormatString("yyyy-mm-dd hh:nn:ss,");	// Start Time
		tmp.sprintf(L"%s,%s,NONE,",
					 mcID.c_str(),									// Machine ID
					 userID.c_str());								// User ID
		data += (AnsiString)tmp + endtime.FormatString("yyyy-mm-dd hh:nn:ss,");	// End Time
		data += (AnsiString)ecode + "," + (AnsiString)detail + "\n";	// Error Code, Detail
	}
	else
	{   //Lot ID가 존재할때
		for (int i = 0; i < LotIDList->Count; i++)
		{
			strValue = LotIDList->Strings[i];
			data += date + Recipe.StartTime.FormatString("yyyy-mm-dd hh:nn:ss,");	// Start Time
			tmp.sprintf(L"%s,%s,%s,",
						 mcID.c_str(),									// Machine ID
						 userID.c_str(),								// User ID
						 strValue.c_str());								// Lot ID
			data += (AnsiString)tmp + endtime.FormatString("yyyy-mm-dd hh:nn:ss,");	// End Time
			data += (AnsiString)ecode + "," + (AnsiString)detail + "\n";	// Error Code, Detail
		}
	}

    delete LotIDList;
	FILE * pFile;

	if (FileExists(file) == false)
	{
		try
		{
			int hFile = FileCreate(file);
			FileClose(hFile);
		}
		catch(...)
		{
			return;
		}

		AnsiString firstData;
		pFile = fopen(file.c_str(), "a+t");
		if (pFile == nullptr)
			return;

		firstData = "DATE,START TIME,M/C ID,OP ID,LOT ID,END TIME,ERROR CODE,DETAIL\n";
		std::fwrite(firstData.c_str(), firstData.Length(), 1, pFile);
		fclose(pFile);
	}

	pFile = fopen(file.c_str(), "a+t");
	if (pFile == nullptr)
		return;
	std::fwrite(data.c_str(), data.Length(), 1, pFile);
	fclose(pFile);
	return;
}

void OvenChamber::WriteJobStartEndData(TDateTime time, bool start)
{
	AnsiString file, cm, name, folder, data="";

	cm.sprintf("%c", 'A' + ChamberID);

	folder = GetProductFolder();

	if (DirectoryExists(folder) == false)
	{
		CreateDir(folder);
	}
	name = AnsiString(Now().FormatString("yyyymmdd"));

	file = folder + name + ".txt";

	FILE * pFile;

	if( FileExists(file) == false)
	{
		try
		{
			int hFile = FileCreate(file);
			FileClose(hFile);
		}
		catch(...)
		{
			return;
		}

		pFile = fopen(file.c_str(), "a+t");
		if( pFile == nullptr )
		{
			return;
		}
		fclose(pFile);
	}

	AnsiString firstData;
	pFile = fopen(file.c_str(), "a+t");
	if( pFile == nullptr )
	{
		return;
	}

	AnsiString jobTime = (AnsiString)time.FormatString("yyyy-mm-dd hh:nn:ss");
	AnsiString job = (start) ?  L"CURE_START" : L"CURE_END";

	data += jobTime + ", ";
	data += cm + ", ";
	data += job + "\n";
	pFile = fopen(file.c_str(), "a+t");

	std::fwrite(data.c_str(), data.Length(), 1, pFile);
	fclose(pFile);
}

bool OvenChamber::DeleteStateFile()
{
	String strFile;
	strFile.sprintf(L"%s%s%c\\Current_T.ini",
					GetRootFolder().c_str(),
					L"PATTERN_CM_",
					'A'+FChamberID);

	if( FileExists(strFile) )
		return DeleteFile(strFile);

	return true;
}

bool OvenChamber::IsStateFileExist()
{
	String strFile;
	strFile.sprintf(L"%s%s%c\\Current_T.ini",
					GetRootFolder().c_str(),
					L"PATTERN_CM_",
					'A'+FChamberID);

	return FileExists(strFile);
}

void OvenChamber::SetChamberTempLimit(double maxLimit, double limit)
{
//	if( FMainTempController )
//		FMainTempController->MaxTemp = maxLimit;

	if( FChamberTempLimitController )
	{
		FChamberTempLimitController->MaxLimit = maxLimit;
		FChamberTempLimitController->Limit1 = limit;
		FChamberTempLimitController->Limit2 = limit;
		FChamberTempLimitController->LimitOffset = ChamberTempLimitOffset;
		FChamberTempLimitController->SP = limit + ChamberTempLimitOffset;
		FChamberTempLimitController->Bias = ChamberTempLimitBias;
	}
}

double OvenChamber::GetPV()
{
	if( MainTempController == nullptr )
		return 0;

	return MainTempController->PV;
}

double OvenChamber::GetSP()
{
	if( MainTempController == nullptr )
		return 0;

	return MainTempController->SP;
}

double OvenChamber::GetHeaterOut()
{
	if( MainTempController == nullptr )
		return 0;

	return MainTempController->HeaterOutput;
}

int OvenChamber::GetSegNo()
{
	if( MainTempController == nullptr )
		return 0;

	return MainTempController->SegNo;
}

int OvenChamber::GetTotalSeg()
{
	if( MainTempController == nullptr )
		return 0;

	return MainTempController->TotalSeg;
}

int OvenChamber::GetOpMode()
{
	if( MainTempController == nullptr )
		return 0;

	return MainTempController->OpMode;
}

bool OvenChamber::IsMainTempCtrlCoolingEventOn()
{
	if( FMainTempController == nullptr )
		return false;

	return FMainTempController->GetTimeEvent(ET_TIMEEVENT2);
}

bool OvenChamber::IsTimeEventOn(int timeEvent)
{
	if( FMainTempController == nullptr )
		return false;

	return FMainTempController->GetTimeEvent(timeEvent);
}

bool OvenChamber::IsMainTempCtrlRun()
{
	if( MainTempController == nullptr )
		return false;

	return MainTempController->IsRun;
}

bool OvenChamber::IsMainTempCtrlReset()
{
	if( MainTempController == nullptr )
		return false;

	return MainTempController->IsReset;
}

bool OvenChamber::IsMainTempCtrlLocal()
{
	if( MainTempController == nullptr )
		return false;

	return MainTempController->IsLocal;
}

bool OvenChamber::IsMainTempCtrlHold()
{
	if( MainTempController == nullptr )
		return false;

	return MainTempController->IsHold;
}

bool OvenChamber::IsMainTempCtrlKeylocked()
{
	if( MainTempController == nullptr )
		return false;

	return MainTempController->IsKeyLocked;
}

bool OvenChamber::IsMainTempCtrlAutoMode()
{
	if( MainTempController == nullptr )
		return false;

	return MainTempController->IsAutoMode;
}

bool OvenChamber::IsBurnout()
{
	if( MainTempController == nullptr )
		return false;

	return MainTempController->IsBurnout;
}

bool OvenChamber::IsATOn()
{
	if( MainTempController == nullptr )
		return false;

	return MainTempController->IsAutoTuningOn;
}

double OvenChamber::GetChamberLimitPV()
{
	if( FChamberTempLimitController == nullptr )
		return 0;

	return FChamberTempLimitController->PV;
//	return FChamberTempLimitData.PV / FChamberTempLimitController->ScaleFactor;
}

double OvenChamber::GetChamberLimitSP()
{
	if( FChamberTempLimitController == nullptr )
		return 0;

	return FChamberTempLimitController->SP;
//	return FChamberTempLimitData.SP / FChamberTempLimitController->ScaleFactor;
}

double OvenChamber::GetPressureLimitPV()
{
	if( FPressureLimitController == nullptr )
		return 0;

	return FPressureLimitController->PV;
//	return FPressureLimitData.PV / FPressureLimitController->ScaleFactor;


	// 소수점 둘째 자리에서 반올림하여 소수점 1자리 실수로 만든다.
//	double value = FPressureLimitData.PV / FPressureLimitController->ScaleFactor;
//	int tmp = (int)(value * 10.0 + 0.5);
//	value = (double)tmp / 10.0;
}

double OvenChamber::GetPressureLimitSP()
{
	if( FPressureLimitController == nullptr )
		return 0;

	return FPressureLimitController->SP;
//	return FPressureLimitData.SP / FPressureLimitController->ScaleFactor;
}

bool OvenChamber::CheckDoorOpenDetectedWhenIdle()
{
	if (DoorOpenWhenIdleStart == false)
	{
		DoorOpenWhenIdleStartTime = Now();
		DoorOpenWhenIdleStart = true;
	}

	if (SecondsBetween(double(DoorOpenWhenIdleStartTime), double(Now())) >= double(DoorOpenDetectedInterval))
	{
		DoorOpenWhenIdleStart = false;
		return true;
	}

    return false;
}
