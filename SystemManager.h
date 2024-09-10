//---------------------------------------------------------------------------
#ifndef SystemManagerH
#define SystemManagerH
//---------------------------------------------------------------------------
#include <vector>
#include "Defines.h"

class BaseChamber;
class SystemManagerBase : public Singleton<SystemManagerBase>
{
protected:
	String FProgramName;
	int FLoginLevel;
	String FUserID;
	String FOperatorID;
	BaseChamber* FChamber[MAX_CHAMBER_COUNT];

	// 시스템 설정 관련 변수들
	int FLanguage;
	int FChamberCount;
	bool FUseAutomation;
	int FAutomationKind;
	bool FUseRemoteCheck;
	// 20181029 - ScreenSaver (infineon) - jhlim
	bool FUseScreenSaver;
	int FScreenSaverTime;

	//20181218 - DVF Mode Logger Offset - mtcho
	int FDVFModeLoggerOffset;

	bool FIsShowLoginForm;

	bool FIsShowValidationForm;

	int FLotInfoClickChamber;    // 랏 정보 그리드 멈춤 방지 클릭된 랏정보창 확인용.

	int GetLanguage() { return FLanguage; }
	void SetLanguage(int language);
	BaseChamber* GetAChamber(int chamberId);

public:
	SystemManagerBase();
	~SystemManagerBase();

	__property String ProgramName = { read = FProgramName };
	__property int LoginLevel = { read = FLoginLevel, write = FLoginLevel };
	__property String UserID = { read = FUserID, write = FUserID };
	__property String OperatorID = { read = FOperatorID, write = FOperatorID };
	__property int Language = { read = GetLanguage, write = SetLanguage };
	__property int ChamberCount = { read = FChamberCount, write = FChamberCount };
	__property bool UseAutomation = { read = FUseAutomation, write = FUseAutomation };
	__property int AutomationKind = { read = FAutomationKind, write = FAutomationKind };
	__property bool UseRemoteCheck = { read = FUseRemoteCheck, write = FUseRemoteCheck };
	// 20181029 - ScreenSaver (infineon) - jhlim
	__property bool UseScreenSaver = { read = FUseScreenSaver, write = FUseScreenSaver };
	__property int ScreenSaverTime = { read = FScreenSaverTime, write = FScreenSaverTime };
	//20181218 - DVF Mode Logger Offset - mtcho
	__property int DVFModeLoogerOffset = { read = FDVFModeLoggerOffset, write = FDVFModeLoggerOffset };
	__property bool IsShowLoginForm = { read = FIsShowLoginForm, write = FIsShowLoginForm };

	__property bool IsShowValidationForm = { read = FIsShowValidationForm, write = FIsShowValidationForm };

	__property int LotInfoClickChamber = { read = FLotInfoClickChamber, write = FLotInfoClickChamber };

	static SystemManagerBase* GetSystemManager() { return Singleton<SystemManagerBase>::GetInstance(); }

	virtual bool LoadSystemParameters() { return false; }
	virtual bool SaveSystemParameters() { return false; }
	virtual bool CreateSerialDevices() { return false; }
	virtual void DestroySerialDevices() { }
	virtual void InitDirectory(String PrgName) { }
};

class OvenChamber;
class DevMainTempController;
class DevLimitController;
class OvenSystemManagerBase : public SystemManagerBase
{
protected:
	// 시스템 설정 관련 변수들
	int FLoggerCount;
	int FLoggerPointCount;
	int FMainTempCtrlPort;
	int FChamberTempCtrlPort;
	int FChamberTempLimitCtrlPort;
	int FLoggerPort;
	int FEmoOption;
	int FGasType;
	int FInverter008IG5APort;	// 20170225 - InverterIG5A Add - jhlim
	int FHeaterPort; // 20170302 - Heater Add - hwang

	// 시리얼 통신 송수신 타임아웃 시간(단위:ms)
	int FMainTempCtrlTimeout;
	int FChamberTempCtrlTimeout;
	int FLimitCtrlTimeout;
	int FLoggerTimeout;
	int FInverter008IG5ATimeout;	// 20170225 - InverterIG5A Add - jhlim
	int FHeaterTimeout; // 20170302 - Heater Add - hwang

	double FMaxTemp;

	DevMainTempController* CreateMainTempController(OvenChamber *chamber, PORTINFO portInfo);
	DevLimitController* CreateLimitController(OvenChamber *chamber, PORTINFO portInfo, int limitType);

public:
	OvenSystemManagerBase();
	~OvenSystemManagerBase();

	__property double MaxTemp = { read = FMaxTemp };
	__property OvenChamber* AChamber[int idx] = { read = GetAChamber };

	__property int MainTempCtrlTimeout = { read = FMainTempCtrlTimeout, write = FMainTempCtrlTimeout };
	__property int ChamberTempCtrlTimeout = { read = FChamberTempCtrlTimeout, write = FChamberTempCtrlTimeout };
	__property int LimitCtrlTimeout = { read = FLimitCtrlTimeout, write = FLimitCtrlTimeout };
	__property int LoggerTimeout = { read = FLoggerTimeout, write = FLoggerTimeout };

	__property int LoggerCount = { read = FLoggerCount, write = FLoggerCount };
	__property int LoggerPointCount = { read = FLoggerPointCount, write = FLoggerPointCount };
	__property int MainTempCtrlPort = { read = FMainTempCtrlPort, write = FMainTempCtrlPort };
	__property int ChamberTempCtrlPort = { read = FChamberTempCtrlPort, write = FChamberTempCtrlPort };
	__property int ChamberTempLimitCtrlPort = { read = FChamberTempLimitCtrlPort, write = FChamberTempLimitCtrlPort };
	__property int LoggerPort = { read = FLoggerPort, write = FLoggerPort };
	__property int EmoOption = { read = FEmoOption, write = FEmoOption };
	__property int GasType = { read = FGasType, write = FGasType };

	virtual void CreateChamber() = 0;
	virtual void DestroyChamber() = 0;
	virtual bool CreateSerialDevices();
	virtual void DestroySerialDevices();
	virtual void InitDirectory(String PrgName);
};

extern PORTSETTING MainTempPortSetting;
extern PORTSETTING SP540PortSetting;
extern PORTSETTING ST340PortSetting;
extern PORTSETTING LoggerPortSetting;
extern PORTSETTING KVC3300PortSetting;
extern PORTSETTING MotorBoardPortSetting;
extern PORTSETTING DIOBoardPortSetting;
extern PORTSETTING DPPortSetting;
extern PORTSETTING O2AnalyzerPortSetting;
extern PORTSETTING ZR5O2AnalyzerPortSetting;
extern PORTSETTING PlasourceRFGenPortSetting;
extern PORTSETTING DamperMotorPortSetting;
extern PORTSETTING DIOPortSetting;
extern PORTSETTING RF400PortSetting;
extern PORTSETTING GT600PortSetting;
extern PORTSETTING Inverter008IG5APortSetting;	// 20170225 - InverterIG5A Add - jhlim
extern PORTSETTING InverterVFDBPortSetting;		// 20170321 - FCFC Unit - jhlim
extern PORTSETTING VSDIOPortSetting;
extern PORTSETTING REXP250PortSetting; //2021-04-12 HANA OVEN

#endif
