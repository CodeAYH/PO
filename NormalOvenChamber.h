 //---------------------------------------------------------------------------

#ifndef NormalOvenChamberH
#define NormalOvenChamberH
//---------------------------------------------------------------------------
#include "Chamber.h"
#include "ChamberDefines.h"
#include "ICPDASDIO.h"
#include "VSOvenBoardDIO.h"
#include "BarunCommManager.h"

#define	WM_GEM_ALARM_SET	(WM_USER + 1000)

class NormalOvenChamber;
class NessLabRFID;
class WatchSystemStatus : public TThread
{
protected:
	NormalOvenChamber *Chamber;
	void __fastcall Execute();

public:
	WatchSystemStatus(NormalOvenChamber *chamber);
	__fastcall ~WatchSystemStatus();
};

typedef struct _JSCK_GEM_PV_DATA
{
	double PV;
	double Press;
	double Logger[5];
} JSCK_GEM_PV_DATA;

class DiffPressure;
class ICPDASDIO;
class I7000DIOModule;
class ZR5;
class DamperMotor;
class VSOvenBoard;
class LSInverter;
class REX_P250;
class NormalOvenChamber : public OvenChamber
{
protected:
	bool FInitCompleted;
	WatchSystemStatus *WatchSystemThread;

	DiffPressure *FDPController;
	I7000DIOModule *FModuleDI;
	I7000DIOModule *FModuleDO;
	I7000DIOModule *FModuleDIO;
	ZR5 *FO2Analyzer;
	DamperMotor* FDamperMotor;
	VSOvenBoard* FOvenBoard;
	LSInverter* FInverter;
	REX_P250* FREX_P250;
	NessLabRFID* FRFIDReader;

	String FMachineID;
	String FChamberSelect;
	String FTCardStartCure;

	BarunCommManagerPtr FBarunManager;

	bool FCannotStartWithoutLotInfo;
	bool FHeatingWithoutN2;
	bool FAllowRecipeSelection;	// 오퍼레이터 모드에서 레시피 선택 허용
    bool FDoorOpenDetectedWhenIdle;
    bool FGotoIdleInLocal;			// 로컬모드 운전 중 도어열면 IDLE 스텝으로 이동 허용
	bool FRecipeMaintain;           //Recipe Maintain After JobEnd
	// 산소농도 알람 활성화 시간(초)
	// 큐어링 작업 시작 후 산소농도 알람이 활성화되는 시간. (단위: 초(sec))
	int FO2DensityAlarmEnableTime;

	// 산소농도 알람 상한값
	// 산소농도가 이 값 이상으로 올라가면 알람 발생. (단위: ppm)
	int FO2DensityLimit;

	// 산소농도 알람 발생시 작업진행 여부
	// true이면 작업을 곧바로 종료한다.
	bool FCureStopOnO2Alarm;

	// 자동 작업 시작 여부
	// true이면 챔버에 매거진이 감지되고 문이 닫히면 작업을 자동으로 시작한다.
	bool FIsAbleAutoStart;

	// 알람 종류 설정
	// ICPCON DIO일 경우 사용(아진엑스텍 DIO일 경우는 사용하지 않음)
	// 시리얼통신일 경우 한 쓰레드에서만 IO 제어를 하기 위함
	bool FDoBuzzerAlarm;
	bool FDoBuzzerJobEnd;
	bool FDoBuzzerOffSwitch;
	bool FDoTowerLampRed;

	bool FUseManualMode;	// 바른전자 MES, 해당 챔버를 매뉴얼 모드로 사용

	bool FUseIdleCooling;
	bool FIdleCoolingStarted;
	bool IdleCoolingDisabled;
    bool IdleCoolingDoorOpened;
	double FIdleCoolingTemp;
	double FIdleCoolingOffset;

	// 20180305 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
	TDateTime O2AlarmStartTime;
	bool IsO2AlarmStartTimeSet;
	bool FO2AnalyzerUseRunLimit;
	// 20180313 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
	int FO2DensityRunLimit;
	int	FO2DensityAlarmRunLimit;
	bool FIsO2DensityRunLimitAlarm;
	int FLargeN2ValveOpenO2Density;

	// 2020-12-18 챔버온도 도어락 시간체크.
	TDateTime DoorLockTempTime;		// 10초 동안 온도 낮을 시 도어 오픈
	bool IsDoorLockTempTimeSet;		// 도어락 시간체크 초기화

	bool FIsLoggerDeviationAlarm[MAX_LOGGERPOINT_COUNT]; //2021-08-12 hcw.
	double TempLoggerData[MAX_LOGGERPOINT_COUNT]; //2021-08-10 hcw.
	TDateTime LoggerDeviationTime[MAX_LOGGERPOINT_COUNT];
	bool TempLoggerTimeCheck [MAX_LOGGERPOINT_COUNT];

	////HANA OVEN 2021-10-13
	bool FHanaCureComplete;
	bool FHanaCureFinishStatus;
	TDateTime HanaEndStatusTime;
	bool FIsBlink;

	int CheckResetCount;	// 2022-02-14

	void DoWorkSequence();

	void OpenN2ValveAll();
	void CloseN2ValveAll();
	void OpenN2LargeValveOnly();
	void OpenN2MediumValveOnly();
	void OpenN2SmallValveOnly();

	void StartIdleCooling();
	void StopIdleCooling();
	void DoIdleCooling();
	void DoDoorLockControl();
	void DoHeaterProblemLampControl();
	void DoWaterExaust();				// 2021-06-09 KJH 쿨링워터 배출

	void DoHanaProtocalSeq(); //2021-04-13 HANA OVEN
	void HanaRecipeInfoUpdate(int ChamberNo, String Recip, PTN ptn);//2021-08-09 hcw
	bool SelectReceiveHanaRecipe(); //2021-08-09 hcw
	void HanaCureFinishStatus();
	void DoLoggerTempDeviation(); //2021-08-11 hcw
	bool DwellTimeLoggerCalc();

	bool GetLoggerDeviation(int idx); //2021-08-12 hcw.
	void SetLoggerDeviation(int idx, bool status);

 //	void N2FlowSetting();

public:
	NormalOvenChamber(int chamberNo);
	__fastcall ~NormalOvenChamber();

	SerialDIO* DIO[DIO_COUNT]; //JSCK 2023-03-09 MAX_DIO_COUNT -> DIO_COUNT

	AnsiString OperatorID;
	bool DeviceRemoved;
	bool DoNotChangeStep;
//	bool IsLoggerDeviationAlarm[MAX_LOGGERPOINT_COUNT]; //2021-08-12 hcw.  삭제
	TStringList *LotIDList;
	//2023-02-28 JSCK Add
	int MesMode;
	String ProcessCode;
	TStringList *ProcessCodeList;
	TStringList *DeviceIDList;
	TStringList *MgzDeviceIDList;
	bool ValidationRequested;
	bool ValidationSuccess;
	std::vector<JSCK_GEM_PV_DATA> JsckPvDataList;

	// 2023-09-11 COWELL CSV Load Data Error Detail.
    String ErrorDetail;

	// 20180227 - FTP (OFILM MES) - jhlim
	LOTINFO FTPLotInfo;
	LOTINFO CowellLotInfo;

	__property bool CannotStartWithoutLotInfo = { read = FCannotStartWithoutLotInfo, write = FCannotStartWithoutLotInfo };
	__property bool HeatingWithoutN2 = { read = FHeatingWithoutN2, write = FHeatingWithoutN2 };
	__property bool AllowRecipeSelection = { read = FAllowRecipeSelection, write = FAllowRecipeSelection };
    __property bool DoorOpenDetectedWhenIdle = { read = FDoorOpenDetectedWhenIdle, write = FDoorOpenDetectedWhenIdle };
	__property bool GotoIdleInLocal = { read = FGotoIdleInLocal, write = FGotoIdleInLocal };
	__property bool RecipeMaintain = { read = FRecipeMaintain, write = FRecipeMaintain };
	__property bool InitCompleted = { read = FInitCompleted, write = FInitCompleted };
	__property bool CureStopOnO2Alarm = { read = FCureStopOnO2Alarm, write = FCureStopOnO2Alarm };
	// 20180305 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
	__property bool O2AnalyzerUseRunLimit = { read = FO2AnalyzerUseRunLimit, write = FO2AnalyzerUseRunLimit };
	// 20180313 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
	__property int O2DensityRunLimit = { read = FO2DensityRunLimit, write = FO2DensityRunLimit };
	__property int O2DensityAlarmRunLimit = { read = FO2DensityAlarmRunLimit, write = FO2DensityAlarmRunLimit };
	__property bool IsO2DensityRunLimitAlarm = { read = FIsO2DensityRunLimitAlarm, write = FIsO2DensityRunLimitAlarm };
	__property int LargeN2ValveOpenO2Density = { read = FLargeN2ValveOpenO2Density, write = FLargeN2ValveOpenO2Density };
	__property bool IsAbleAutoStart = { read = FIsAbleAutoStart, write = FIsAbleAutoStart };
	__property bool DoBuzzerAlarm = { read = FDoBuzzerAlarm, write = FDoBuzzerAlarm };
	__property bool DoBuzzerJobEnd = { read = FDoBuzzerJobEnd, write = FDoBuzzerJobEnd };
	__property bool DoBuzzerOffSwitch = { read = FDoBuzzerOffSwitch, write = FDoBuzzerOffSwitch };
	__property bool DoTowerLampRed = { read = FDoTowerLampRed, write = FDoTowerLampRed };
	__property bool UseManualMode = { read = FUseManualMode, write = FUseManualMode };

	__property int O2DensityAlarmEnableTime = { read = FO2DensityAlarmEnableTime, write = FO2DensityAlarmEnableTime };
	__property int O2DensityLimit = { read = FO2DensityLimit, write = FO2DensityLimit };
	__property DiffPressure* DPController = { read = FDPController, write = FDPController };
	__property I7000DIOModule* ModuleDI = { read = FModuleDI, write = FModuleDI };
	__property I7000DIOModule* ModuleDO = { read = FModuleDO, write = FModuleDO };
	__property I7000DIOModule* ModuleDIO = { read = FModuleDIO, write = FModuleDIO };
	__property ZR5* O2Analyzer = { read = FO2Analyzer, write = FO2Analyzer };
	__property DamperMotor* DamperMotor = { read = FDamperMotor, write = FDamperMotor };
	__property VSOvenBoard* OvenBoard = { read = FOvenBoard, write = FOvenBoard };
	__property LSInverter* Inverter = { read = FInverter, write = FInverter };
	__property REX_P250* HanOvenCim = { read = FREX_P250, write = FREX_P250 };
	__property NessLabRFID* RFIDReader = { read = FRFIDReader, write = FRFIDReader };
	__property bool IsLoggerDeviationAlarm[int idx] = { read = GetLoggerDeviation, write = SetLoggerDeviation };

	__property BarunCommManagerPtr BarunManager = { read = FBarunManager, write = FBarunManager };
	__property String MachineID = { read = FMachineID, write = FMachineID };

	__property double IdleCoolingTemp = { read = FIdleCoolingTemp, write = FIdleCoolingTemp };
	__property double IdleCoolingOffset = { read = FIdleCoolingOffset, write = FIdleCoolingOffset };
	__property bool IdleCoolingStarted = { read = FIdleCoolingStarted };
	__property bool UseIdleCooling = { read = FUseIdleCooling, write = FUseIdleCooling };
	__property bool HanaCureComplete = { read = FHanaCureComplete, write = FHanaCureComplete };
	__property bool IsBlink = { read = FIsBlink, write = FIsBlink };

	__property String ChamberSelect = { read = FChamberSelect, write = FChamberSelect };
	__property String TCardStartCure = { read = FTCardStartCure, write = FTCardStartCure };

	void JsckGemParamClear();
	void RunWatchAlarm();
	void ForcedCureStop();

	bool LoadChamberParameters();
	bool SaveChamberParameters();
	bool SaveCurrentState();
	bool LoadCurrentState();
	bool WriteOxygenData(TDateTime time);
	void CreateDIO();
	void DestroyDIO();

	bool DoCureStart(AnsiString &returnMsg, bool byRemote=false);
	bool DoCureStop(int stopType);
	void InitDIO();
	void BuzzerOn(int buzzerType);
	void BuzzerOff();
	void BuzzerOffSwitchOn(bool on=true);

	void TowerLampGreen(bool on);
	void TowerLampRed(bool on);
	void TowerLampYellow(bool on);

	void DoTowerLamp(int towerLamp, int mode, bool isBlink);
	void DoTowerLampControl(bool isBlink);

	bool IsDoorClosed();
	bool IsDoorLocked();
	bool IsDoorUnlocked();
	bool IsMagazineDetected();

	void DoDamperControl();
	void DoInverterControl();
	void DoN2FlowControl();

	void ApplyDataToController();
	int CalcRemainTime();
	void MakeDataFilename();
	void DoMainTempControllerKeyLock();
	void DoMainTempControllerSetAuto();

	bool IsBuzzerOffSwitchPushed() { return DIO[I_BUZZER_OFF]->IsOn(); }
	bool IsBuzzerOn() { return DIO[O_BUZZER_ON]->IsOn(); }

	void DoIdleCoolingDisable();

	// 20180305 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
	bool CheckO2DensityforCureStart();

	// 20191216 - SCK MES - jhlim
	SCKMESInfo stSckMesInfo;
	void InitData_MESLotInfo();
	void MakeFolder_MESLotInfo();
	bool CheckExistFile_MESLotInfo();
	bool ReadFile_MESLotInfo();

	bool DoHanaCureStop(); //2021-07-23 hcw.
	void HanaStopStatus(); //2021-10-12
	void ResetSafetyRelay();
	void ResetSafetyOn();
	void CowellLotInfoClear();
};

#endif
