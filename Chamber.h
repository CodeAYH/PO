//---------------------------------------------------------------------------
#ifndef ChamberH
#define ChamberH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <memory>
#include <vector>
#include "Defines.h"

class DevMainTempController;
class DevLimitController;
class SP540;
class I7018;
class BaseChamber : public TThread
{
protected:
	int FChamberID;
	int FStep;
	bool FEnabled;
	bool StepChanged;

	// 20180621 - Thread Exception Error String - jhlim
	String FErrorString;

	// 알람 관련 변수들
	bool FAlarmStatus[MAX_ALARM_COUNT];
	bool FNewAlarm;
	std::vector<unsigned int> Alarms;

	virtual void __fastcall Execute();
	virtual void DoWorkSequence() = 0;
	virtual void LogStepChanged() = 0;

	virtual void SetStep(int step);
	bool GetAlarmStatus(int idx);
	void SetAlarmStatus(int idx, bool status);

public:
	__property int ChamberID = { read = FChamberID, write = FChamberID };
	__property int Step = { read = FStep, write = SetStep };
	__property bool Enabled = { read = FEnabled, write = FEnabled };

	__property bool AlarmStatus[int idx] = { read = GetAlarmStatus, write = SetAlarmStatus };
	__property bool NewAlarm = { read = FNewAlarm, write = FNewAlarm };
	// 20180621 - Thread Exception Error String - jhlim
	__property String ErrorString = { read = FErrorString, write = FErrorString };

	BaseChamber(int chamberNo);
	__fastcall ~BaseChamber();

	virtual bool LoadChamberParameters() = 0;
	virtual bool SaveChamberParameters() = 0;

	bool SetAlarm(unsigned int alarmCode);
	unsigned int GetAlarm(int idx);
	virtual void ClearAlarm(unsigned int alarmCode);
	virtual void ClearAlarmAll();
	int GetAlarmCount() { return Alarms.size(); }

	void DelayTime(unsigned int milliSeconds, bool workInThread=true);
};

class OvenChamber : public BaseChamber
{
protected:
	// 시리얼 통신 디바이스들
	DevMainTempController *FMainTempController;
	SP540 *FChamberTempController;
	DevLimitController *FChamberTempLimitController;
	DevLimitController *FHeaterTempLimitController;
	DevLimitController *FPressureLimitController;
	I7018 *FLoggerController[MAX_LOGGER_COUNT];

	int FLoggerData[MAX_LOGGERPOINT_COUNT];
	int FMainTempControllerType;
	int FLimitControllerType;

	// 챔버 설정관련 변수들
	int FLoggerOffset[MAX_LOGGERPOINT_COUNT];
	bool FUseChannel[MAX_LOGGERPOINT_COUNT];
	int FChamberTempLimitOffset;
	int FChamberTempLimitBias;
	//2021-08-10 hcw
	int FChamberLoggerDeviation;

	// 도어 열림 조건 관련 변수들
	bool FUseDoorOpenCondition;
	double FDoorOpenConditionTemp;
	int FDoorOpenConditionTime;
	double FWorkCompletionTemp;

	bool FDoorOpenWhenIdleStart;
	TDateTime FDoorOpenWhenIdleStartTime;
	int FDoorOpenDetectedInterval;

protected:
	virtual void DoMainTempControllerKeyLock() = 0;

	void LogStepChanged();
	I7018* GetLoggerController(int idx);
	void SetLoggerController(int idx, I7018 *logger);

	double GetLoggerOffset(int idx);
	void SetLoggerOffset(int idx, double offset);
	bool GetUseChannel(int idx);
	void SetUseChannel(int idx, bool offset);
	double GetChamberTempLimitOffset();
	void SetChamberTempLimitOffset(double offset);
	double GetChamberTempLimitBias();
	void SetChamberTempLimitBias(double bias);
	//2021-08-09 hcw.
	double GetChamberLoggerTempDeviation();
	void SetChamberLoggerTempDeviation(double offset);

	double GetPV();
	double GetSP();
	double GetHeaterOut();
	int GetSegNo();
	int GetTotalSeg();
	int GetOpMode();

	virtual bool IsMainTempCtrlCoolingEventOn();
	virtual bool IsMainTempCtrlRun();
	virtual bool IsMainTempCtrlReset();
	virtual bool IsMainTempCtrlLocal();
	virtual bool IsMainTempCtrlHold();
	virtual bool IsMainTempCtrlKeylocked();
	virtual bool IsBurnout();
	virtual bool IsATOn();
	virtual bool IsMainTempCtrlAutoMode();

	double GetChamberLimitPV();
	double GetChamberLimitSP();
	double GetPressureLimitPV();
	double GetPressureLimitSP();

	double GetLoggerData(int idx);
	void SetLoggerData(int idx, double data);

public:
	int ProgressValue;

	RECIPE Recipe;

	__property DevMainTempController* MainTempController = { read = FMainTempController, write = FMainTempController };
	__property SP540* ChamberTempController = { read = FChamberTempController, write = FChamberTempController };
	__property DevLimitController* ChamberTempLimitController = { read = FChamberTempLimitController, write = FChamberTempLimitController };
	__property DevLimitController* HeaterTempLimitController = { read = FHeaterTempLimitController, write = FHeaterTempLimitController };
	__property DevLimitController* PressureLimitController = { read = FPressureLimitController, write = FPressureLimitController };
	__property I7018* LoggerController[int idx] = { read = GetLoggerController, write = SetLoggerController };
	__property double LoggerData[int idx] = { read = GetLoggerData, write = SetLoggerData };
	__property int MainTempControllerType = { read = FMainTempControllerType, write = FMainTempControllerType };
	__property int LimitControllerType = { read = FLimitControllerType, write = FLimitControllerType };
	__property int DoorOpenDetectedInterval = { read = FDoorOpenDetectedInterval, write = FDoorOpenDetectedInterval };

	// UP550
	__property double PV = { read = GetPV };
	__property double SP = { read = GetSP };
	__property double HeaterOut = { read = GetHeaterOut };
	__property int SegNo = { read = GetSegNo };
	__property int TotalSeg = { read = GetTotalSeg };
	__property int OpMode = { read = GetOpMode };
	__property bool IsCoolingEventOn = { read = IsMainTempCtrlCoolingEventOn };
	__property bool IsRun = { read = IsMainTempCtrlRun };
	__property bool IsReset = { read = IsMainTempCtrlReset };
	__property bool IsLocal = { read = IsMainTempCtrlLocal };
	__property bool IsHold = { read = IsMainTempCtrlHold };
	__property bool IsKeyLocked = { read = IsMainTempCtrlKeylocked };
	__property bool IsTCBurnout = { read = IsBurnout };
	__property bool IsAutoTuningOn = { read = IsATOn };
	__property bool IsAutoMode = { read = IsMainTempCtrlAutoMode };

	// Chamber Temp. Limit
	__property double ChamberLimitPV = { read = GetChamberLimitPV };
	__property double ChamberLimitSP = { read = GetChamberLimitSP };

	// Pressure Limit
	__property double PressureLimitPV = { read = GetPressureLimitPV };
	__property double PressureLimitSP = { read = GetPressureLimitSP };

	__property double LoggerOffset[int idx] = { read = GetLoggerOffset, write = SetLoggerOffset };
	__property bool UseChannel[int idx] = { read = GetUseChannel, write = SetUseChannel };
	__property double ChamberTempLimitOffset = { read = GetChamberTempLimitOffset, write = SetChamberTempLimitOffset };
	__property double ChamberTempLimitBias = { read = GetChamberTempLimitBias, write = SetChamberTempLimitBias };
	//2021-08-09 hcw.
	__property double ChamberLoggerTempDeviation = { read = GetChamberLoggerTempDeviation, write = SetChamberLoggerTempDeviation };

	// 도어 열림 조건 관련 변수들
	__property bool UseDoorOpenCondition = { read = FUseDoorOpenCondition, write = FUseDoorOpenCondition };
	__property double DoorOpenConditionTemp = { read = FDoorOpenConditionTemp, write = FDoorOpenConditionTemp };
	__property int DoorOpenConditionTime = { read = FDoorOpenConditionTime, write = FDoorOpenConditionTime };
	__property double WorkCompletionTemp = { read = FWorkCompletionTemp, write = FWorkCompletionTemp };


	__property bool DoorOpenWhenIdleStart = { read = FDoorOpenWhenIdleStart, write = FDoorOpenWhenIdleStart };
	__property TDateTime DoorOpenWhenIdleStartTime = { read = FDoorOpenWhenIdleStartTime, write = FDoorOpenWhenIdleStartTime };

public:
	OvenChamber(int chamberNo);
	__fastcall ~OvenChamber();

	virtual bool LoadChamberParameters() = 0;
	virtual bool SaveChamberParameters() = 0;

	virtual bool DoCureStart(AnsiString &returnMsg, bool byRemote=false) = 0;
	virtual bool DoCureStop(int stopType) = 0;
	virtual bool SaveCurrentState() = 0;
	virtual bool LoadCurrentState() = 0;
	virtual int CalcRemainTime() = 0;
	virtual void ForcedCureStop() = 0;

	int GetCurrentSegTime() { return Recipe.TempPtn.Seg[SegNo-1].Time; }
	double GetCurrentSegSP() { return Recipe.TempPtn.Seg[SegNo-1].Tsp; }

	virtual void InitCureValues();
	virtual void MakeDataFilename();
	bool WriteMainTempData(TDateTime time);
	bool WritePressData(TDateTime time);
	bool WriteLoggerData(TDateTime time);
	bool WriteLimitData(TDateTime time);
	// 2023-09-06 Cure정보 CSV 파일 저장
	void WriteProductData(TDateTime endtime, String mcID, String userID,
						String lotid, bool errorCode=false, String detail="");
	void WriteJobStartEndData(TDateTime time, bool start);

	bool IsStateFileExist();
	bool DeleteStateFile();
	void SetChamberTempLimit(double maxLimit, double limit);

	bool IsTimeEventOn(int timeEvent);

    bool CheckDoorOpenDetectedWhenIdle();
};

#endif
