//---------------------------------------------------------------------------

#ifndef NormalOvenSystemManagerH
#define NormalOvenSystemManagerH
//---------------------------------------------------------------------------

#include "SystemManager.h"
#include "SCK_RMS.h"

// 20180208 - FTP (OFILM MES) - jhlim
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#define ET_PURGE			ET_TIMEEVENT1
#define ET_COOLING			ET_TIMEEVENT2
#define ET_N2OFF			ET_TIMEEVENT3
#define	ET_DAMPER			ET_TIMEEVENT4

// TOWER LAMP ==================================================
enum { TWR_RED, TWR_YEL, TWR_GRN, TWR_BUZ, TOWERLAMP_KIND };
enum { TWR_AUTORUN, TWR_STOP, TWR_LOTEND, TWR_ERR, TOWERLAMP_MODE };
enum { LAMP_OFF, LAMP_ON, LAMP_BLINK };
enum { BUZZ_OFF, BUZZ_ALARM };

//---------------------------------------------------------------------------
class WatchFtpConnect : public TThread
{
protected:
	void __fastcall Execute();

public:
	__fastcall WatchFtpConnect();
	__fastcall ~WatchFtpConnect();
};
/****************************************************************************
Description	: CTowerLamp Class
*****************************************************************************/
class CTowerLamp
{
	int nTowerVal[TOWERLAMP_KIND][TOWERLAMP_MODE];

public:
	CTowerLamp();
	~CTowerLamp();

	bool FbChanging;

	bool operator==(const CTowerLamp& arg);
	CTowerLamp& operator=(const CTowerLamp& arg);

	void LoadValue(const String sPath);
	void SaveValue(const String sPath);
	int GetTwrVal(const UINT nKind, const UINT nMode) const;
	void SetTwrVal(const UINT nKind, const UINT nMode, const int nVal);
	String GetTwrValStr(const UINT nKind) const;
};

class SckRmsThread;
class NormalOvenChamber;
class NormalOvenSystemManager : public OvenSystemManagerBase
{
private:
	SckRmsThread *SckRmsWatch;		// 2021-12-15
	WatchFtpConnect *WatchFtpConnectThread;

	int FDamperMotorCtrlPort;
	int FDIOCtrlPort;
	int FDPCtrlPort;
	int FO2AnalyzerAPort;
	int FO2AnalyzerBPort;
	int FO2AnalyzerCPort;
	int FO2AnalyzerDPort;
	int FInverterPort;
	int FREXP250APort;
	int FREXP250BPort;
	int FREXP250CPort;
	int FREXP250DPort;
	int FRfidReaderAPort;

	int FDamperMotorTimeout;
	int FDIOTimeout;
	int FDPTimeout;
	int FO2AnalyzerATimeout;
	int FO2AnalyzerBTimeout;

	int FJobEndTimer;

	int FDoorLockTemp;	// 2020-12-16 Add 챔버온도 도어락 설정 온도

	int FDIOType;	// IO Type (ICPDAS / OVEN DIO /...)

	// 2021-06-09 KJH 쿨링워터 배출시간
	// 0초 이면 동작 안함(단위: 초)
	int FCoolingVentExuastTime;

	bool FUseInverter;

	// 차압계 통신 타입 여부 - false : 통신타입 아님, true : 통신타입
	bool FDPType;

	bool FUseWaterFlowDetector;
	bool FUseN2FlowDetector;
	bool FUseN2FlowControl;
	bool FUseMagazineDetector;
	bool FUseAirPressureDetector;
	bool FUseElectricPanelDoorDetector;
    bool FUseN2PressureDetector;
    bool FUseHeaterOverloadDetector;
    bool FUseDiffPressureDetector;
    bool FUseSSROverTempDetector;
    bool FUseChamberDoorTempInterlock;
	bool FUseGroundAlarmDetector;
    bool FUseBlowMotorLowCurrentDetector;
	bool FUseDoorLockCylinder;
	bool FUseTowerLampControl;
	bool FUseDoorLockTempControl;			// 2020-12-16 Add 챔버온도 도어락 사용유무
	bool FUsePasswordWhenIdleCoolingStop;	// 2021-03-05 Add IdleCooling일때 패스워스 사용유무
	bool FUseLoggerDataSave;					// 2021-03-15 ADD Logger 데이터 저장 사용유무
	bool FUseLocalN2Alarm;					// 2021-09-09 Local Mode N2 Alarm Check.
	bool FDrawBgSP;							// 2021-11-09 yjyoo, 메인화면 온도 그래프에 Set Point 그래프를 백그라운드로 그릴지 여부
 //	bool FUseN2PurgeDetector;     //2023-04-17 restore   // 2023-01-30 Change from flow meter to flow switch
 //	bool FUseCDAFlowDetector;      //2023-04-17 restore
 //	bool FUseSafetyReset;            //2023-04-17 restore
	// 산소분석기 사용 여부
	bool FUseO2Analyzer;
	// IDLE 상태 도어 열림 센서 사용 여부 *FUseGroundAlarmDetector 미사용 시 사용 가능*

	bool FUseCureStartEnableFromLocalState; //로컬 상태에서 GEM Remote메세지로 큐어 시작 가능 사용유무

	// 댐퍼 모터 제어 타입
	int FDamperControlType;

	int FCureStartAlarmTime;
	int FAutoModeChangeTimer;

	// FTP 연결 확인
	bool FCheckFtpConnect;

	int FI_N2_FLOW_DETECTED;

	HINTERNET mh_ftp_session;
	HINTERNET mh_ftp_connection;

	//JSCK
	bool FAutoCureStartAfterValidation;

	// 2023-09-11 Cowell Csv File Save
	bool FUseCsvDataSave;
	String FCsvMachineId;

	NormalOvenChamber* __fastcall GetAChamber(int chamberId);
	void N2FlowSetting();

public:
	NormalOvenSystemManager();
	~NormalOvenSystemManager();

	__property NormalOvenChamber* AChamber[int idx] = { read = GetAChamber };
	__property bool DPType = { read = FDPType, write = FDPType };
	__property int DIOType = { read = FDIOType, write = FDIOType };
	__property bool UseO2Analyzer = { read = FUseO2Analyzer, write = FUseO2Analyzer };
	__property bool UseWaterFlowDetector = { read = FUseWaterFlowDetector, write = FUseWaterFlowDetector };
	__property bool UseN2FlowDetector = { read = FUseN2FlowDetector, write = FUseN2FlowDetector };
	__property bool UseN2FlowControl = { read = FUseN2FlowControl, write = FUseN2FlowControl };
	__property bool UseMagazineDetector = { read = FUseMagazineDetector, write = FUseMagazineDetector };
	__property bool UseAirPressureDetector = { read = FUseAirPressureDetector, write = FUseAirPressureDetector };
	__property bool UseElectricPanelDoorDetector = { read = FUseElectricPanelDoorDetector, write = FUseElectricPanelDoorDetector };
	__property bool UseInverter = { read = FUseInverter, write = FUseInverter };
	__property bool UseN2PressureDetector = { read = FUseN2PressureDetector, write = FUseN2PressureDetector };
	__property bool UseHeaterOverloadDetector = { read = FUseHeaterOverloadDetector, write = FUseHeaterOverloadDetector };
	__property bool UseDiffPressureDetector = { read = FUseDiffPressureDetector, write = FUseDiffPressureDetector };
	__property bool UseSSROverTempDetector = { read = FUseSSROverTempDetector, write = FUseSSROverTempDetector };
	__property bool UseChamberDoorTempInterlock = { read = FUseChamberDoorTempInterlock, write = FUseChamberDoorTempInterlock };
	__property bool UseGroundAlarmDetector = { read = FUseGroundAlarmDetector, write = FUseGroundAlarmDetector };
	__property bool UseBlowMotorLowCurrentDetector = { read = FUseBlowMotorLowCurrentDetector, write = FUseBlowMotorLowCurrentDetector };
	__property bool UseDoorLockCylinder = { read = FUseDoorLockCylinder, write = FUseDoorLockCylinder };
	__property bool UseTowerLampControl = { read = FUseTowerLampControl, write = FUseTowerLampControl };
	// 2020-12-16 Add 챔버온도 도어락 사용유무
	__property bool UseDoorLockTempControl = { read = FUseDoorLockTempControl, write = FUseDoorLockTempControl };
	// 2021-03-05 Add IdleCooling일때 패스워스 사용유무
	__property bool UsePasswordWhenIdleCoolingStop = { read = FUsePasswordWhenIdleCoolingStop, write = FUsePasswordWhenIdleCoolingStop };
	// 2021-03-15 ADD Logger 데이터 저장 사용유무
	__property bool UseLoggerDataSave = { read = FUseLoggerDataSave, write = FUseLoggerDataSave };

	__property bool UseLocalN2Alarm = { read = FUseLocalN2Alarm, write = FUseLocalN2Alarm };
	__property bool DrawBgSP = { read = FDrawBgSP, write = FDrawBgSP };
	// 2023-01-30 Change from flow meter to flow switch
//	__property bool UseN2PurgeDetector = { read = FUseN2PurgeDetector, write = FUseN2PurgeDetector }; //2023-04-17 restore
 //	__property bool UseCDAFlowDetector = { read = FUseCDAFlowDetector, write = FUseCDAFlowDetector }; //2023-04-17 restore
//	__property bool UseSafetyReset = { read = FUseSafetyReset, write = FUseSafetyReset };  //2023-04-17 restore

	__property int DamperMotorCtrlPort = { read = FDamperMotorCtrlPort, write = FDamperMotorCtrlPort };
	__property int DIOCtrlPort = { read = FDIOCtrlPort, write = FDIOCtrlPort };
	__property int DPCtrlPort = { read = FDPCtrlPort, write = FDPCtrlPort };
	__property int O2AnalyzerAPort = { read = FO2AnalyzerAPort, write = FO2AnalyzerAPort };
	__property int O2AnalyzerBPort = { read = FO2AnalyzerBPort, write = FO2AnalyzerBPort };
	__property int O2AnalyzerCPort = { read = FO2AnalyzerCPort, write = FO2AnalyzerCPort };
	__property int O2AnalyzerDPort = { read = FO2AnalyzerDPort, write = FO2AnalyzerDPort };
	__property int InverterPort = { read = FInverterPort, write = FInverterPort };
	//HANA OVEN 2021-04-12
	__property int REXP250APort = { read = FREXP250APort, write = FREXP250APort };
	__property int REXP250BPort = { read = FREXP250BPort, write = FREXP250BPort };
	__property int REXP250CPort = { read = FREXP250CPort, write = FREXP250CPort };
	__property int REXP250DPort = { read = FREXP250DPort, write = FREXP250DPort };
	// JSCK RFID Reader Port
	__property int RfidReaderAPort = { read = FRfidReaderAPort, write = FRfidReaderAPort };

	__property int DamperMotorTimeout = { read = FDamperMotorTimeout, write = FDamperMotorTimeout };
	__property int DIOTimeout = { read = FDIOTimeout, write = FDIOTimeout };
	__property int DPTimeout = { read = FDPTimeout, write = FDPTimeout };
	__property int O2AnalyzerATimeout = { read = FO2AnalyzerATimeout, write = FO2AnalyzerATimeout };
	__property int O2AnalyzerBTimeout = { read = FO2AnalyzerBTimeout, write = FO2AnalyzerBTimeout };

	__property int DamperControlType = { read = FDamperControlType, write = FDamperControlType };
	__property int JobEndTimer = { read = FJobEndTimer, write = FJobEndTimer };

	// 2020-12-16 Add 챔버온도 도어락
	__property int DoorLockTemp = { read = FDoorLockTemp, write = FDoorLockTemp };

	// 2021-06-09 KJH 쿨링워터 배출시간
	__property int CoolingVentExuastTime = { read = FCoolingVentExuastTime, write = FCoolingVentExuastTime };

	// 2021-12-01 KJH 도어 닫기 후 큐어 시작 알람
	__property int CureStartAlarmTime = { read = FCureStartAlarmTime, write = FCureStartAlarmTime };

	__property int AutoModeChangeTimer = { read = FAutoModeChangeTimer, write = FAutoModeChangeTimer };
	//로컬 상태에서 GEM Remote메세지로 큐어 시작 가능 사용유무
	__property bool UseCureStartEnableFromLocalState = { read = FUseCureStartEnableFromLocalState, write = FUseCureStartEnableFromLocalState};

	// FTP 연결 확인
	__property bool CheckFtpConnect = { read = FCheckFtpConnect, write = FCheckFtpConnect };

	__property int I_N2_FLOW_DETECTED = { read = FI_N2_FLOW_DETECTED, write = FI_N2_FLOW_DETECTED };

	//JSCK
	__property bool AutoCureStartAfterValidation = { read = FAutoCureStartAfterValidation, write = FAutoCureStartAfterValidation };

	// 2023-09-11 Cowell Csv File Save
	__property bool UseCsvDataSave = { read = FUseCsvDataSave, write = FUseCsvDataSave };
	__property String CsvMachineId = { read = FCsvMachineId, write = FCsvMachineId };

	std::shared_ptr<CTowerLamp> TowerLampOption;

	static NormalOvenSystemManager* GetSystemManager()
	{
		return Singleton<NormalOvenSystemManager>::GetInstance();
	}

	bool LoadSystemParameters();
	bool SaveSystemParameters();
	void CreateChamber();
	void DestroyChamber();

	void LoadTwrValue();
	void SaveTwrValue();

	bool CreateSerialDevices();
	void DestroySerialDevices();

	// 20180208 - FTP (OFILM MES) - jhlim
	bool CreateFTPConnection();
	void DestroyFTPConnection();
	bool TrasferFileUpload();
	// 20180212 - FTP (OFILM MES) - jhlim
	bool DeleteFTPDataFile();

	// 2021-12-15 SCK Thread
	void CreateSCKRMSThread();
	void DestroySCKRMSThread();

	bool IsUseGem();

	void RunWatchFtpConnect();
	void DestroyWatchFtpConnect();
};

#define GetManager() NormalOvenSystemManager::GetSystemManager()
#define GetChamber(idx) NormalOvenSystemManager::GetSystemManager()->AChamber[idx]

typedef enum _EVENTINDEX
{
	PVHIGH=0, PVLOW, COOLING, PURGE, PVHIGH_CLEAR, PVLOW_CLEAR, DAMPER
} EVENTINDEX;

#endif
