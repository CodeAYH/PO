//---------------------------------------------------------------------------
#ifndef REX_P250H
#define REX_P250H
//---------------------------------------------------------------------------
#include "SerialDevice.h"

#define AVERAGE_COUNT	10
#define MAX_REX_P250_SEG_COUNT 15

/* ==========================================================================
Description	: 하나마이크론 MES 대응용 클래스, REX_P250 온도 컨트롤러 프로토콜 구현
========================================================================== */

// Polling command
const AnsiString M1				= "M1";
const AnsiString TR				= "TR";
const AnsiString HO				= "HO";
const AnsiString EN				= "EN";
const AnsiString WT				= "WT";
const AnsiString XM				= "XM";
const AnsiString PS				= "PS";
const AnsiString SN				= "SN";
const AnsiString RA				= "RA";
const AnsiString S1				= "S1";
const AnsiString B1				= "B1";

// Selecting command
const AnsiString PN				= "PN";
//const AnsiString SN				= "SN";
const AnsiString LE				= "LE";
const AnsiString TM				= "TM";
//const AnsiString XM				= "XM";
const AnsiString PE				= "PE";

// HO
const int NOT_IN_HOLD_STATE		= 0;
const int IN_HOLD_STATE			= 1;

// EN
const int NOT_IN_END_STATE		= 0;
const int IN_END_STATE			= 1;

// WT
const int NOT_IN_WAIT_STATE		= 0;
const int IN_WAIT_STATE			= 1;

// XM
const int OPMODE_RESET			= 0;
const int OPMODE_PROG			= 1;
const int OPMODE_FIX			= 2;
const int OPMODE_MAN			= 3;

// B1
const int BS_NORMAL				= 0;
const int BS_BURNOUT			= 1;

// 오븐의 현재 상태정보를 실시간으로 업데이트한다.
// 호스트가 요청할 때 이 값을 전송한다.
typedef struct _REPORT_DATA     //2021-04-12 HANA OVEN
{
	double PV;					// M1
	double SV;					// S1
	int CurrentPatternNo;		// PS
	int CurrentSegNo;			// SN
 //	int SegmentRemainingTime;	// TR
	AnsiString SegmentRemainingTime;	// TR
	int HoldState;				// HO
	int EndState;				// EN
	int WaitState;				// WT
	int OperationMode;			// XM
	int BurnoutState;			// B1
} REPORT_DATA;

// 호스트가 전송해 주는 패턴값을 저장한다.
// Run 커맨드가 오면 이 정보를 이용하여 패턴정보를 구성하여
// 온도 컨트롤러에 전송한다.

typedef struct _SEG_DATA
{
	bool TimeReceived;
	bool SVReceived;
	int Time;
	int SV;
} SEG_DATA;

typedef struct _SETTING_DATA
{
	int PatternNo;
	int SegmentCount;
	int ReceivedSegCount;
	SEG_DATA Seg[100];

	void Init()
	{
		PatternNo = 0;
		SegmentCount = 0;
		ReceivedSegCount = 0;
		for (int i = 0; i < 100; i++)
		{
			Seg[i].TimeReceived = false;
			Seg[i].SVReceived = false;
			Seg[i].Time = 0;
			Seg[i].SV = 0;
		}
    }

	int GetMaxTemp()
	{
		int maxTemp = 0;
		for (int i = 0; i < SegmentCount; i++)
		{
			if (maxTemp < Seg[i].SV)
			{
				maxTemp = Seg[i].SV;
			}
		}
		return maxTemp;
	}

	int GetTotalTime()
	{
		int total = 0;
		for (int i = 0; i < SegmentCount; i++)
		{
			total += Seg[i].Time;
		}
		return total;
	}

	void GetHourMin(int &hour, int &min)
	{
		int total = GetTotalTime();
		hour = total / 60;
		min = total % 60;
	}
} SETTING_DATA;

class REX_P250 : public SerialDevice
{
private:
	char RxStx;
	int DecodeMode;
	int ChamberNo;
	AnsiString FstrSeg;
	AnsiString RxTempData; //2021-07-16 hcw
	bool CureStrtCmd;
	int AckCount;
	TDateTime CommErrBaseTime;

	void DoMonitor() {};
	void DecodeMonitor();
	bool IsRxDone();
	void GetMonitorCommands(TStringList *list);
	void DoCommunicate();

	REPORT_DATA FHanaData;
	void SetHanaData(const REPORT_DATA cpVal)	{ FHanaData = cpVal; }
	void UpdatePtnTimeTemp();
	void HanaSaveLog(String str);

protected:
	bool FSelectingCmd;

protected:
	bool IsMyCommand();
	bool IsPollingCommand();
	bool IsSelectingCommand();
	void DoPollingSequence();
	void DoSelectingSequence();
	bool IsSelectingParsing();
	void SendAck();
	void SendNak();

	AnsiString MakeTempDataString(const double tempData);
	AnsiString MakeTimeDataString(const int timeData);
	AnsiString MakeIntDataString(const int intData);
	unsigned short CRC16(const AnsiString data);
	unsigned char CalcBCC(const AnsiString data);
	bool CheckBCC(const AnsiString data, const unsigned char bcc);

public:
	REX_P250(BaseChamber *chamber, PORTINFO portInfo);
	__fastcall ~REX_P250();

	REPORT_DATA ReportData;
	SETTING_DATA SettingData;
	PTN TempPtn;

	int SegCount;
	__property REPORT_DATA HanaData = { read = FHanaData, write = SetHanaData };

	void DoHanaPatternNo(int PatternNo);
	void DoHanaSegNo(int SenNo);
	void DoOperationMode(int OPMode);
	void DoEndState(int EndState);
	void DoHanaPV(double PV);
	void DoHanaSV(double SV);
	void DoHanaRemainTime(String RemainTime);

	bool GetOperationEndState(int* EndState, int* OperationMode);
	bool GetHanaReceivePtn(PTN &ReceivePtn);
};
#endif
