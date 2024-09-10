//---------------------------------------------------------------------------

#ifndef MainTempControllerH
#define MainTempControllerH
//---------------------------------------------------------------------------
#include "SerialDevice.h"

#define UP550_PARAM_COUNT		12
#define UP55A_PARAM_COUNT		11

enum MAINTEMPCTRLTYPE { TEMPCTRL_UP550=0, TEMPCTRL_UP55A, TEMPCTRL_SP540 };
extern const char UP550ParamStr[UP550_PARAM_COUNT][300];
extern const char UP55AParamStr[UP55A_PARAM_COUNT][300];

class DevMainTempController : public SerialDevice
{
private:
	AnsiString RxTempData;
//	int FMaxTemp;

//	inline double __fastcall GetMaxTemp() { return (double)FMaxTemp / FScaleFactor; }
//	inline void __fastcall SetMaxTemp(double maxTemp) { FMaxTemp = (int)(maxTemp * FScaleFactor); }

	virtual void DecodeMonitor() = 0;
	virtual bool IsRxDone() = 0;
	virtual void GetMonitorCommands(TStringList *list) = 0;

	virtual void DoWriteParameter(PARAM *param) = 0;
	virtual void DoDeletePattern(PARAM *param) = 0;
	virtual void DoWriteSTC(PARAM *param) = 0;
	virtual void DoWriteSeg(PARAM *param) = 0;
	virtual void DoReset(PARAM *param) = 0;
	virtual void DoLocal(PARAM *param) = 0;
	virtual void DoResume(PARAM *param) = 0;
	virtual void DoRun(PARAM *param) = 0;
	virtual void DoNextSeg(PARAM *param) = 0;
	virtual void DoLockKey(PARAM *param) = 0;
	virtual void DoUnlockKey(PARAM *param) = 0;
	virtual void DoSetMode(PARAM *param) = 0;
	// 20170613 - Manual Test - jhlim
	virtual void DoSetTestTempOn(PARAM *param) = 0;
	virtual	void DoSetTestTempOff(PARAM *param) = 0;
	// 20181218 - DVF func Add - jhlim
	virtual void DoSelPIDZone(PARAM *param) = 0;
	virtual void DoSelPIDNo(PARAM *param) = 0;
	// 20190227 - Pattern Check - jhlim
	virtual void DoReadSTC(PARAM *param) = 0;
	virtual void DoReadSeg(PARAM *param) = 0;

	// 20191119 - YJYOO - 히터 출력 최대 리미트 설정
	virtual void DoWriteOutputLimit(PARAM *param) {}

	virtual int GetDIStatus(int diNum) = 0;
	virtual bool GetPVEventHigh() = 0;
	virtual bool GetPVEventLow() = 0;

	virtual int GetSegNo() = 0;
	virtual int GetTotalSeg() = 0;
	virtual int GetOpMode() = 0;
	virtual int GetRemainingTime() = 0;
	// 20181218 - DVF func Add - jhlim
	virtual int GetSelPIDZone() = 0;
	virtual int GetSelPIDNo() = 0;

	virtual double GetPV() = 0;
	virtual double GetSP() = 0;
	virtual double GetHeaterOut() = 0;

	virtual bool GetIsRun() = 0;
	virtual bool GetIsReset() = 0;
	virtual bool GetIsHold() = 0;
	virtual bool GetIsLocal() = 0;
	virtual bool GetIsBurnout() = 0;
	virtual bool GetIsKeyLocked() = 0;
	virtual bool GetIsAutoTuningOn() = 0;
	virtual bool GetIsAutoMode() = 0;

protected:
	// 2020-02-06 yjyoo, 시리얼 로그 저장 여부
	bool SaveResponseLog;

	int FParamCount;
	// 20190227 - Pattern Check - jhlim
	AnsiString FstrSTC;
	AnsiString FstrSeg;

public:
	DevMainTempController(BaseChamber *chamber, PORTINFO portInfo)
		: SerialDevice(chamber, portInfo) { SaveResponseLog = false; }
	__fastcall ~DevMainTempController() {}

	virtual bool WriteParameter(int idx) = 0;
	virtual bool DeletePattern(int ptnNo) = 0;
	virtual bool WriteSTC(PTN ptn) = 0;
	virtual bool WriteSeg(int idx, PTN ptn) = 0;
	virtual bool Reset() = 0;
	virtual bool Local() = 0;
	virtual bool Resume() = 0;
	virtual bool Run(int ptnNo) = 0;
	virtual bool Hold() = 0;
	virtual bool GotoNextSeg() = 0;
	virtual bool LockKey() = 0;		// RUN/RESET key lock
	virtual bool UnlockKey() = 0;    // RUN/RESET key unlock
	virtual bool SetAutoMode(bool autoMode=true) = 0;
	// 20170613 - Manual Test - jhlim
	virtual bool SetTestTempOn(int nTemp) = 0;
	virtual bool SetTestTempOff(int nTemp) = 0;
	// 20181218 - DVF func Add - jhlim
	virtual bool SelPIDZone(int ZoneNo) = 0;
	virtual bool SelPIDNo(int PIDNo) = 0;
	// 20190227 - Pattern Check - jhlim
	virtual bool ReadSTC(int PtnNo) = 0;
	virtual bool ReadSeg(int PtnNo, int idx) = 0;
	virtual bool CompareSTC(PTN ptn, AnsiString &returnMsg) = 0;
	virtual bool CompareSeg(int idx, PTN ptn, AnsiString &returnMsg) = 0;

	virtual bool GetTimeEvent(int eventNo) = 0;

	// 20191119 - YJYOO - 히터 출력 최대 리미트 설정
	virtual bool WriteOutputLimit(int limit) { return true; }
	virtual int GetOutputLimit() { return 100; }

//	readwrite_property(int, GetMaxTemp, SetMaxTemp) MaxTemp;
	__property int SegNo = { read = GetSegNo };
	__property int TotalSeg = { read = GetTotalSeg };
	__property int DIStatus[int idx] = { read = GetDIStatus };
	__property int OpMode = { read = GetOpMode };
	__property int RemainingTime = { read = GetRemainingTime };

	__property bool PVEventHigh = { read = GetPVEventHigh };
	__property bool PVEventLow = { read = GetPVEventLow };
	__property double PV = { read = GetPV };
	__property double SP = { read = GetSP };
	__property double HeaterOutput = { read = GetHeaterOut };

	__property bool IsRun = { read = GetIsRun };
	__property bool IsReset = { read = GetIsReset };
	__property bool IsHold = { read = GetIsHold };
	__property bool IsLocal = { read = GetIsLocal };
	__property bool IsBurnout = { read = GetIsBurnout };
	__property bool IsKeyLocked = { read = GetIsKeyLocked };
	__property bool IsAutoTuningOn = { read = GetIsAutoTuningOn };
	__property bool IsAutoMode = { read = GetIsAutoMode };
	// 20181218 - DVF func Add - jhlim
	__property int IsSelPIDZone = { read = GetSelPIDZone };
	__property int IsSelPIDNo = { read = GetSelPIDNo };

	__property int ParamCount = { read = FParamCount };
	// 20190227 - Pattern Check - jhlim
	__property AnsiString strSTC = { read = FstrSTC, write = FstrSTC };
	__property AnsiString strSeg = { read = FstrSeg, write = FstrSeg };
};

class UP550 : public DevMainTempController
{
private:
	MAINTEMPDATA MainTempData;

	void DecodeMonitor();
	bool IsRxDone();
	void GetMonitorCommands(TStringList *list);

	void DoWriteParameter(PARAM *param);
	void DoDeletePattern(PARAM *param);
	void DoWriteSTC(PARAM *param);
	void DoWriteSeg(PARAM *param);
	void DoReset(PARAM *param);
	void DoLocal(PARAM *param);
	void DoResume(PARAM *param);
	void DoRun(PARAM *param);
	void DoHold(PARAM *param);
	void DoNextSeg(PARAM *param);
	void DoLockKey(PARAM *param);
	void DoUnlockKey(PARAM *param);
	void DoSetMode(PARAM *param);
	// 20170613 - Manual Test - jhlim
	void DoSetTestTempOn(PARAM *param);
	void DoSetTestTempOff(PARAM *param);
	// 20181218 - DVF func Add - jhlim
	void DoSelPIDZone(PARAM *param);
	void DoSelPIDNo(PARAM *param);
	// 20190227 - Pattern Check - jhlim
	void DoReadSTC(PARAM *param);
	void DoReadSeg(PARAM *param);

	int GetDIStatus(int diNum);
	int GetSegNo();
	int GetTotalSeg();
	int GetOpMode();
	int GetRemainingTime();
	// 20181218 - DVF func Add - jhlim
	int GetSelPIDZone(){ return MainTempData.SelPIDZone; }
	int GetSelPIDNo(){ return MainTempData.SelPIDNo; }

	double GetPV();
	double GetSP();
	bool GetPVEventHigh();
	bool GetPVEventLow();
	double GetHeaterOut();

	bool GetIsRun();
	bool GetIsReset();
	bool GetIsHold();
	bool GetIsLocal();
	bool GetIsBurnout();
	bool GetIsKeyLocked();
	bool GetIsAutoTuningOn();
	bool GetIsAutoMode() { return MainTempData.IsAuto; }

	bool GetTimeEvent(int eventNo);

public:
	UP550(BaseChamber *chamber, PORTINFO portInfo);
	__fastcall ~UP550();

	bool WriteParameter(int idx);
	bool DeletePattern(int ptnNo);
	bool WriteSTC(PTN ptn);
	bool WriteSeg(int idx, PTN ptn);
	bool Reset();
	bool Local();
	bool Resume();
	bool Run(int ptnNo);
	bool Hold();
	bool GotoNextSeg();
	bool LockKey();		// RUN/RESET key lock
	bool UnlockKey();    // RUN/RESET key unlock
	bool SetAutoMode(bool autoMode=true);
	bool SetLocalMode(bool autoMode=true);

	// 20170613 - Manual Test - jhlim
	bool SetTestTempOn(int nTemp);
	bool SetTestTempOff(int nTemp);

	// 20181218 - DVF func Add - jhlim
	bool SelPIDZone(int ZoneNo);
	bool SelPIDNo(int PIDNo);

	// 20190227 - Pattern Check - jhlim
	bool ReadSTC(int PtnNo);
	bool ReadSeg(int PtnNo, int idx);
	bool CompareSTC(PTN ptn, AnsiString &returnMsg);
	bool CompareSeg(int idx, PTN ptn, AnsiString &returnMsg);
};

class UP55A : public DevMainTempController
{
private:
	MAINTEMPDATA MainTempData;

	void DecodeMonitor();
	bool IsRxDone();
	void GetMonitorCommands(TStringList *list);

	void DoWriteParameter(PARAM *param);
	void DoDeletePattern(PARAM *param);
	void DoWriteSTC(PARAM *param);
	void DoWriteSeg(PARAM *param);
	void DoReset(PARAM *param);
	void DoLocal(PARAM *param);
	void DoResume(PARAM *param);
	void DoRun(PARAM *param);
	void DoHold(PARAM *param);
	void DoNextSeg(PARAM *param);
	void DoLockKey(PARAM *param);
	void DoUnlockKey(PARAM *param);
	void DoSetMode(PARAM *param);
	// 20170613 - Manual Test - jhlim
	void DoSetTestTempOn(PARAM *param);
	void DoSetTestTempOff(PARAM *param);
	// 20181218 - DVF func Add - jhlim
	void DoSelPIDZone(PARAM *param);
	void DoSelPIDNo(PARAM *param);
	// 20190227 - Pattern Check - jhlim
	void DoReadSTC(PARAM *param);
	void DoReadSeg(PARAM *param);

	// 20191119 - YJYOO - 히터 출력 최대 리미트 설정
	void DoWriteOutputLimit(PARAM *param);

	int GetDIStatus(int diNum);
	int GetSegNo();
	int GetTotalSeg();
	int GetOpMode();
	int GetRemainingTime();
	// 20181218 - DVF func Add - jhlim
	int GetSelPIDZone(){ return MainTempData.SelPIDZone; }
	int GetSelPIDNo(){ return MainTempData.SelPIDNo; }

	double GetPV();
	double GetSP();
	bool GetPVEventHigh();
	bool GetPVEventLow();
	double GetHeaterOut();

	bool GetIsRun();
	bool GetIsReset();
	bool GetIsHold();
	bool GetIsLocal();
	bool GetIsBurnout();
	bool GetIsKeyLocked();
	bool GetIsAutoTuningOn();
	bool GetIsAutoMode() { return MainTempData.IsAuto; }

	bool GetTimeEvent(int eventNo);

public:
	UP55A(BaseChamber *chamber, PORTINFO portInfo);
	__fastcall ~UP55A();

	bool WriteParameter(int idx);
	bool DeletePattern(int ptnNo);
	bool WriteSTC(PTN ptn);
	bool WriteSeg(int idx, PTN ptn);
	bool Reset();
	bool Local();
	bool Resume();
	bool Run(int ptnNo);
	bool Hold();
	bool GotoNextSeg();
	bool LockKey();		// RUN/RESET key lock
	bool UnlockKey();    // RUN/RESET key unlock
	bool SetAutoMode(bool autoMode=true);
	// 20170613 - Manual Test - jhlim
	bool SetTestTempOn(int nTemp);
	bool SetTestTempOff(int nTemp);
	// 20181218 - DVF func Add - jhlim
	bool SelPIDZone(int ZoneNo);
	bool SelPIDNo(int PIDNo);
	// 20190227 - Pattern Check - jhlim
	bool ReadSTC(int PtnNo);
	bool ReadSeg(int PtnNo, int idx);
	bool CompareSTC(PTN ptn, AnsiString &returnMsg);
	bool CompareSeg(int idx, PTN ptn, AnsiString &returnMsg);

	// 20191119 - YJYOO - 히터 출력 최대 리미트 설정
	bool WriteOutputLimit(int limit);
	int GetOutputLimit() { return MainTempData.OutputLimit; }
};
#endif
