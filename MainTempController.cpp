//---------------------------------------------------------------------------
#include <System.AnsiStrings.hpp>
#
#pragma hdrstop

#include "MainTempController.h"
#include "HelperFunctions.h"
#include "SerialPortManager.h"
#include "Chamber.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

UP550::UP550(BaseChamber *chamber, PORTINFO portInfo)
	: DevMainTempController(chamber, portInfo)
{
	MainTempData.DIStatus = 0xFFFF;
	MainTempData.SP = 300;
	MainTempData.IsAuto = true;

	FScaleFactor = 10.0;
	FDevName = "UP550";
	FParamCount = UP550_PARAM_COUNT;
}

UP550::~UP550()
{

}

int UP550::GetDIStatus(int diNum)
{
	int tmp = MainTempData.DIStatus;

	tmp = tmp >> diNum;
	return tmp & 0x1;
}

int UP550::GetSegNo()
{
	return MainTempData.SegNo;
}

int UP550::GetTotalSeg()
{
	return MainTempData.TotalSeg;
}

int UP550::GetRemainingTime()
{
	return MainTempData.RemainingTime;
}

double UP550::GetHeaterOut()
{
	return MainTempData.HeaterOutput / FScaleFactor;
}

double UP550::GetPV()
{
	return MainTempData.PV / FScaleFactor;
}

double UP550::GetSP()
{
	return MainTempData.SP / FScaleFactor;
}

bool UP550::GetPVEventHigh()
{
	return MainTempData.PVEventHigh;
}

bool UP550::GetPVEventLow()
{
	return MainTempData.PVEventLow;
}

bool UP550::GetIsRun()
{
	return MainTempData.OpMode == 1 ? true : false;
}

bool UP550::GetIsReset()
{
	return MainTempData.OpMode == 0 ? true : false;
}

bool UP550::GetIsHold()
{
	return MainTempData.IsHold;
}

bool UP550::GetIsLocal()
{
	return MainTempData.OpMode == 2 ? true : false;
}

int UP550::GetOpMode()
{
	return MainTempData.OpMode;
}

bool UP550::GetIsBurnout()
{
	return MainTempData.PV1Error & 0x0002;
}

bool UP550::GetIsKeyLocked()
{
	if( MainTempData.RunResetKeylock == true &&
		MainTempData.ModeKeylock == true )
		return true;

	return false;
}

bool UP550::GetIsAutoTuningOn()
{
	return MainTempData.Mod1 & 0x4000;
}

bool UP550::GetTimeEvent(int eventNo)
{
	int mask[7] = { 0x0001, 0x0002, 0x0004, 0x0010, 0x0020, 0x0040, 0x0100 };
	bool ret = false;

	if( eventNo > 0 &&
		eventNo <= 7 )
		ret = MainTempData.TimeEventStatus & mask[eventNo-1];

	return ret;
}

bool UP550::IsRxDone()
{
	if( RxBuffer.IsEmpty() )
		return false;

	try
	{
		SaveSerialLog(DevName, RxBuffer, FALSE);

		AnsiString strStx, strEtx, strOk, strCr;

		strStx.sprintf("%c", STX);
		strEtx.sprintf("%c", ETX);
		strOk = "OK";
		strCr.sprintf("%c", CR);
		if( AnsiContainsStr(RxBuffer, strStx) &&
			AnsiContainsStr(RxBuffer, strEtx) &&
			AnsiContainsStr(RxBuffer, strOk) &&
			AnsiContainsStr(RxBuffer, strCr) )
		{
			// 2020-02-06 yjyoo
			if (SaveResponseLog)
			{
				SaveResponseLog = false;
				SaveSerialLog(DevName, RxBuffer, FALSE);
			}

			int idx = RxBuffer.AnsiPos(strEtx);
			RxData = RxBuffer.SubString(2, idx-2);

			if( IsValidHex(RxData.SubString(1, 2)) )
			{
				if( HexToInt(RxData.SubString(1, 2)) != Addr )
					return false;
			}
			else
				return false;

			ValidRxData = true;
			return true;
		}
	}
	catch(...)
	{
		SaveResponseLog = false;
		return false;
	}

	return false;
}

void UP550::GetMonitorCommands(TStringList *list)
{
	// D0208, Operation Mode, (0000: Rest, 0001: Program, 0002: Local)
	// 00:D0002 PV1 Error
	// 01:D0003	PV.1 (Measured input value [PV] for Loop-1)
	// 02:D0004	CSP.1 (Current target setpoint [SP] for Loop-1)
	// 03:D0005	OUT.1 (Control output value [OUT] for Loop-1)
	// 04:D0008	MOD.1(Operation mode of Loop-1)
	// 05:D0011	ALM (Alarm Status)
	// 06:D0012	PVEV (PV Event Status)
	// 07:D0013	TMEV1 (Time Event Status)
	// 08:D0015	PTN (Program pattern number in operation)
	// 09:D0016	SEGNO (Current segment number)
	// 10:D0017	TIME (Remaining time of the current segment,
	//					or elapsed time during wait status)
	// 11:D0033	DISTS (Statuses of External Contact Inputs)
	// 12:D0035	PARAERR (Error in calibration values and parameters)
	// 13:D0044	SEGUSE (Number of segments contained in the selected pattern)
	// 14:D0045	REM.RCY (Number of repetitions of the pattern in operation)
	// 15:D0046	ALL.RCY (Remaining number of repetitions
	//						of the pattern in operation)
	// 16:D0047	RST (Repeat Start number of the current segment)
	// 17:D0048	REN (Repeat End number of the current segment)

	// 18:D1027 RUN/RESET Key-lock
	// 19:D1028 MODE Key-lock
	// 20:D0033 DISTS (Status of DIs)
	// 21:D0211 A/M1 (AUTO/MAN modes for Loop-1)

	AnsiString regs;
	AnsiString txData;

	regs = "D0208,D0209,D0002,D0003,D0004,D0005,D0008,D0012,D0013,D0015,D0016,D0017,D0044,D1027,D1028,D0033,D0211";
	txData.sprintf("%c%02d010WRR17%s%c%c",
				   STX,
				   Addr,
				   regs.c_str(),
				   ETX,
				   CR);
	list->Add(txData);
}

void UP550::DecodeMonitor()
{
	if( RxData.Length() != 74 )
		return;

	MAINTEMPDATA data;

	try
	{
		int status;
		int curSeg;
		int startIdx = 7;
		const int len = 4;
		AnsiString str;

		// Operation Mode : D0208
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.OpMode = HexToInt(str);
		else
			return;
		startIdx += len;

		// HOLD : D0209
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.IsHold = HexToInt(str)==1?true:false;
		else
			return;
		startIdx += len;

		// PV1 Error : D0002
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.PV1Error = HexToInt(str);
		else
			return;
		startIdx += len;

		// PV : D0003
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.PV = HexToInt(str);
		else
			return;
		startIdx += len;

		// CSP : D0004
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.SP = HexToInt(str);
		else
			return;
		startIdx += len;

		// Heater Output : D0005
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.HeaterOutput = HexToInt(str);
		else
			return;
		startIdx += len;

		// MOD.1 (Operation mode of loop 1) : D0008
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.Mod1 = HexToInt(str);
		else
			return;
		startIdx += len;

		// PV Event Status : D0012
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
		{
			status = HexToInt(str);
			data.PVEventHigh = ((status & 0x01) != 0);
			data.PVEventLow = ((status & 0x02) != 0);
		}
		else
			return;
		startIdx += len;

		// Time Event Status : D0013
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.TimeEventStatus = HexToInt(str);
		else
			return;
		startIdx += len;

		// Pattern No : D0015
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.PtnNo = HexToInt(str);
		else
			return;
		startIdx += len;

		// Current Segment No : D0016
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.SegNo = HexToInt(str);
		else
			return;
		startIdx += len;

		// Remaining Time : D0017
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.RemainingTime = HexToInt(str);
		else
			return;
		startIdx += len;

		// Total Seg. Count : D0044
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.TotalSeg = HexToInt(str);
		else
			return;
		startIdx += len;

		// RUN/RESET Key-lock : D1027
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.RunResetKeylock = HexToInt(str) == 1 ? true : false;
		else
			return;
		startIdx += len;

		// MODE Key-lock : D1028
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.ModeKeylock = HexToInt(str) == 1 ? true : false;
		else
			return;
		startIdx += len;

		// Status of DI : D0033
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.DIStatus = HexToInt(str);
		else
			return;
		startIdx += len;

		// AUTO/MAN modes for Loop-1 : D0211
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
		{
			int val = HexToInt(str);
			data.IsAuto = val == 0 ? true : false;
		}
		else
			return;

		MainTempData = data;
	}
	catch(...)
	{
		return;
	}

	InitRxValues();
}

void UP550::DoWriteParameter(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;
	int idx = param->Int;

	SaveResponseLog = true;
	txData.sprintf(UP550ParamStr[idx],
				   STX,
				   Addr,
				   ETX,
				   CR);

	TxData(txData);
	WaitRx1();

	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP550::DoDeletePattern(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;
	int ptnNo = param->Int;

	SaveResponseLog = true;
	// B0061, PTNO.A (Source pattern number)
	txData.sprintf("%c%02d010WWRB0061,01,%04X%c%c",
				   STX,
				   Addr,
				   ptnNo,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	SaveResponseLog = true;
	// B0063, PTE.TRG (Program pattern deleting trigger)
	txData.sprintf("%c%02d010WWRB0063,01,0001%c%c",
				   STX,
				   Addr,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP550::DoWriteSTC(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;
	PTN ptn = (*(PTN*)param->Param);

	//  [0]limit 리미트를 제일 처음에 넣는다.
	//	 리미트를 변경하면 온도값들이 초기화 되기 때문이다.
	//	D1204, High limit , ex) 250도 = 2500 = 0x9C4
	//	D1205, Low Limit
	//  [1]패턴 넘버 쓰기
	//	B0001, PTNO (Pattern number setting) , 0000 <- 4바이트로 쓴다.
	//  [2]SEG = 0, Start Condition Data 임을 알린다.
	//	B0002, SEGNO (Segment number setting) , 0000 으로 세팅
	//  [3]SSP1, STC Setting
	//	B0011, SSP1 (Start target setpoint)
	//	B0013, STC (Start Code)
	//  [4]WRTRG = 1
	//	B0004, WRTRG (Writing trigger) ,  1 넣어서 기록 신호 준다.
	//
	//  B0014, 1.WZ1 Wait Zone 온도
	//  B0016, 1.WTM  시간

	// 2020-02-05 yjyoo, Wait Zone 삭제
/*	txData.sprintf("%c%02d010WRW07B0001,%04X,B0002,0000,B0011,%04X,B0013,%04X,B0014,%04X,B0016,%04X,B0004,0001%c%c",
				   STX,
				   Addr,
				   ptn.PtnNo,
				   (int)(ptn.Stc.Ssp*10),
				   ptn.Stc.Stc,
				   (int)(ptn.Stc.Wz[0]*10),
				   ptn.Stc.WTm[0],
				   ETX,
				   CR); */

	// 2020-06-12 yjyoo, Local mode temp. 추가
    int localTemp = (int)(ptn.LocalTemp * 10);

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW06B0001,%04X,B0002,0000,B0011,%04X,B0013,%04X,B0004,0001,D0101,%04X%c%c",
				   STX,
				   Addr,
				   ptn.PtnNo,
				   (int)(ptn.Stc.Ssp*10),
				   ptn.Stc.Stc,
                   localTemp,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP550::DoWriteSeg(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData, tmp1, tmp2;
	int idx = param->Int;
	PTN ptn = (*(PTN*)param->Param);

	// [1]패턴 넘버 쓰기
	// B0001, PTNO (Pattern number setting) , 0000 <- 4바이트로 쓴다.
	// [2]Segment number setting
	// B0002, SEGNO (Segment number setting) , 0000 <- 4바이트로 쓴다.
	// [3]TSP1, TIME, JC
	// B0111 , TSP1 (Start target setpoint) , ex) 10  1.0도
	// B0113 , TIME
	// B0140 , JC
	// [4]타임 이벤트에 대하여...
	// B0116 ~ B0139 총 EV1~EV8, 8개의 이벤트 저장공간이 있다.
	// [4]WRTRG = 1
	// B0104, WRTRG (Writing trigger) ,  1 넣어서 기록 신호 준다.

	SaveResponseLog = true;
	tmp1.sprintf("B0001,%04X,B0002,%04X,B0111,%04X,B0113,%04X,B0140,%04X",
				 ptn.PtnNo,
				 idx+1,
				 (int)(ptn.Seg[idx].Tsp*10),
				 ptn.Seg[idx].Time,
				 ptn.Seg[idx].Jc);

	int cnt = 5;
	for(int j=0, n=16; j<8; j++)
	{
		WaitForSingleObject((void *)Handle, THREAD_WAIT_TIME);

		if( ptn.Seg[idx].EvKind[j] == ET_PVHIGH ||
			ptn.Seg[idx].EvKind[j] == ET_PVLOW )
		{
			tmp2.sprintf(",B01%02d,%04X,B01%02d,%04X,B01%02d,%04X",
						 n, ptn.Seg[idx].EvKind[j],
						 n+1, ptn.Seg[idx].OnType[j],
						 n+2, ptn.Seg[idx].OffPe[j] * 10);
		}
		else
		{
			tmp2.sprintf(",B01%02d,%04X,B01%02d,%04X,B01%02d,%04X",
						 n, ptn.Seg[idx].EvKind[j],
						 n+1, ptn.Seg[idx].OnType[j],
						 n+2, ptn.Seg[idx].OffPe[j]);
		}
		tmp1 += tmp2;
		n += 3;
		cnt += 3;
	}
	tmp1 += ",B0104,0001";
	cnt++;
	txData.sprintf("%c%02d010WRW%02d%s%c%c",
				   STX,
				   Addr,
				   cnt,
				   tmp1.c_str(),
				   ETX,
				   CR);

	TxData(txData);
	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP550::DoReset(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW01D0208,%04X%c%c",
				   STX,
				   Addr,
				   UP55X_RESET,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP550::DoLocal(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW01D0208,%04X%c%c",
				   STX,
				   Addr,
				   UP55X_LOCAL,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP550::DoHold(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW01D0209,%04X%c%c",
				   STX,
				   Addr,
				   param->Int,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP550::DoResume(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW01D0209,0000%c%c",
				   STX,
				   Addr,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP550::DoLockKey(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW02D1027,0001,D1028,0001%c%c",
				   STX,
				   Addr,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP550::DoUnlockKey(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW02D1027,0000,D1028,0000%c%c",
				   STX,
				   Addr,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP550::DoSetMode(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	if( param->Bool )
		txData.sprintf("%c%02d010WRW01D0211,0000%c%c", STX, Addr, ETX, CR);
	else
		txData.sprintf("%c%02d010WRW01D0211,0001%c%c", STX, Addr, ETX, CR);

	TxData(txData);
	WaitRx1();

	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP550::DoNextSeg(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW01D0210,%04X%c%c",
				   STX,
				   Addr,
				   1,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP550::DoRun(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW02D0214,%04X,D0208,%04X%c%c",
				   STX,
				   Addr,
				   param->Int,
				   UP55X_PROGRAM,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool UP550::WriteParameter(int idx)
{
	bool ret = false;
	if( idx >= FParamCount )
		return false;

	PARAM param;
	param.Int = idx;

	Param = &param;
	CommandFunc = DoWriteParameter;
	ret = RetryCommand();

	return ret;
}

bool UP550::DeletePattern(int ptnNo)
{
	bool ret = false;

	PARAM param;
	param.Int = ptnNo;

	Param = &param;
	CommandFunc = DoDeletePattern;
	ret = RetryCommand();

	return ret;
}

bool UP550::WriteSTC(PTN ptn)
{
	bool ret = false;

	PARAM param;
	param.Param = &ptn;

	Param = &param;
	CommandFunc = DoWriteSTC;
	ret = RetryCommand();

	return ret;
}

bool UP550::WriteSeg(int idx, PTN ptn)
{
	bool ret = false;

	PARAM param;
	param.Int = idx;
	param.Param = &ptn;

	Param = &param;
	CommandFunc = DoWriteSeg;
	ret = RetryCommand();

	return ret;
}

bool UP550::Reset()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoReset;
	ret = RetryCommand();

	return ret;
}

bool UP550::Local()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoLocal;
	ret = RetryCommand();

	return ret;
}

bool UP550::Run(int ptnNo)
{
	bool ret = false;

	PARAM param;
	param.Int = ptnNo;

	Param = &param;
	CommandFunc = DoRun;
	ret = RetryCommand();

	return ret;
}

bool UP550::Hold()
{
	bool ret = false;

	PARAM param;
	param.Int = 1;

	Param = &param;
	CommandFunc = DoHold;
	ret = RetryCommand();

	return ret;
}

bool UP550::Resume()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoResume;
	ret = RetryCommand();

	return ret;
}

bool UP550::LockKey()
{
	bool ret = false;

	PARAM param;
	param.Int = 0;

	Param = &param;
	CommandFunc = DoLockKey;
	ret = RetryCommand();

	return ret;
}

bool UP550::UnlockKey()
{
	bool ret = false;

	PARAM param;
	param.Int = 0;

	Param = &param;
	CommandFunc = DoUnlockKey;
	ret = RetryCommand();

	return ret;
}

bool UP550::SetAutoMode(bool autoMode/*=true*/)
{
	bool ret = false;

	PARAM param;
	param.Bool = autoMode;

	Param = &param;
	CommandFunc = DoSetMode;
	ret = RetryCommand();

	return ret;
}

bool UP550::GotoNextSeg()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoNextSeg;
	ret = RetryCommand();

	return ret;
}

// 20170623 - Manual Test - jhlim
void UP550::DoSetTestTempOn(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	int nTemp = param->Int * 10;
	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW04D2316,%04X,D2201,%04X,D2203,0001,D2204,0000%c%c",
					STX,
					Addr,
					UP55X_LOCAL,
					nTemp,
					ETX,
					CR);

	TxData(txData);
	WaitRx1();

	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP550::DoSetTestTempOff(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	int nTemp = param->Int * 10;
	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW04D2316,%04X,D2201,%04X,D2203,0001,D2204,0000%c%c",
					STX,
					Addr,
					UP55X_RESET,
					nTemp,
					ETX,
					CR);

	TxData(txData);
	WaitRx1();

	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool UP550::SetTestTempOn(int nTemp)
{
	bool ret = false;

	PARAM param;
	param.Int = nTemp;

	Param = &param;
	CommandFunc = DoSetTestTempOn;
	ret = RetryCommand();

	return ret;
}

bool UP550::SetTestTempOff(int nTemp)
{
	bool ret = false;

	PARAM param;
	param.Int = nTemp;

	Param = &param;
	CommandFunc = DoSetTestTempOff;
	ret = RetryCommand();

	return ret;
}

// 20181218 - DVF func Add - jhlim
void UP550::DoSelPIDZone(PARAM *param)
{
	// 기능 구현 안됨
}

void UP550::DoSelPIDNo(PARAM *param)
{
	// 기능 구현 안됨
}

bool UP550::SelPIDZone(int ZoneNo)
{
	bool ret = false;

	PARAM param;
	param.Int = ZoneNo;

	Param = &param;
	CommandFunc = DoSelPIDZone;
	ret = RetryCommand();

	return ret;
}

bool UP550::SelPIDNo(int PIDNo)
{
	bool ret = false;

	PARAM param;
	param.Int = PIDNo;

	Param = &param;
	CommandFunc = DoSelPIDNo;
	ret = RetryCommand();

	return ret;
}

// 20190319 - Pattern Check - jhlim
void UP550::DoReadSTC(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	int PtnNo = param->Int;
	AnsiString txData = "";
	strSTC = "";

	SaveResponseLog = true;
	// Ptn No (B0001), Seg No (B0002), Reading TRG (B0003) Setting
	txData.sprintf("%c%02d010WRW03B0001,%04X,B0002,0000,B0003,0001%c%c",
	//txData.sprintf("%c%02d010WWRB0001,03,%04X00000001%c%c",
					STX,
					Addr,
					PtnNo,
					ETX,
					CR);
	// Save Log
	SaveSerialLog(DevName, txData, TRUE);

	TxData(txData);
	WaitRx1();

	// Read STC Info
	// PTN.ERR (B0005), SSP1 (B0011), STC (B0013), 1.WZ1 (B0014), 1.WTM1 (B0016)
	// 2020-02-05 yjyoo, Wait zone 삭제
/*	txData.sprintf("%c%02d010WRR05B0005,B0011,B0013,B0014,B0016%c%c",
					STX,
					Addr,
					ETX,
					CR); */
	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRR03B0005,B0011,B0013%c%c",
					STX,
					Addr,
					ETX,
					CR);

	// Save Log
	SaveSerialLog(DevName, txData, TRUE);

	TxData(txData);
	if(WaitRx1())
	{
		strSTC = RxData;
	}

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

// 20190319 - Pattern Check - jhlim
void UP550::DoReadSeg(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	int PtnNo = param->Int;
	int idx = param->Unsigned;
	int cnt = 0;
	AnsiString txData = "";
	AnsiString Temp = "";
	AnsiString Temp1 = "";
	strSeg = "";

	SaveResponseLog = true;
	// Ptn No (B0001), Seg No (B0002), Reading TRG (B0103) Setting
	txData.sprintf("%c%02d010WRW03B0001,%04X,B0002,%04X,B0103,0001%c%c",
					STX,
					Addr,
					PtnNo,
					(idx + 1),
					ETX,
					CR);
	// Save Log
	SaveSerialLog(DevName, txData, TRUE);

	TxData(txData);
	WaitRx1();

	// Read SEG Info
	// PTN.ERR (B0105), TSP1 (B0111), TIME (B0113), Junction code (B0140)
	cnt = 4;
	Temp.sprintf("B0105,B0111,B0113,B0140,");
	// (Event - EvKind, OnTime, OffTime) * 8
	// B0116 ~ B0139
	for(int i=0, n=16; i<8; i++)
	{
		Temp1.sprintf("B01%02d,B01%02d,B01%02d", n, n+1, n+2);
		Temp += Temp1;
		if(i < 7)
			Temp += ",";
		n += 3;
		cnt += 3;
		Temp1 = "";
	}

	SaveResponseLog = true;
	// SEG INFO
	txData.sprintf("%c%02d010WRR%02d%s%c%c",
					STX,
					Addr,
					cnt,
					Temp.c_str(),
					ETX,
					CR);
	// Save Log
	SaveSerialLog(DevName, txData, TRUE);

	TxData(txData);
	if(WaitRx1())
	{
		strSeg = RxData;
	}

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool UP550::ReadSTC(int PtnNo)
{
	bool ret = false;
	PARAM param;
	param.Int = PtnNo;

	Param = &param;
	CommandFunc = DoReadSTC;
	ret = RetryCommand();

	return ret;
}

bool UP550::ReadSeg(int PtnNo, int idx)
{
	bool ret = false;
	PARAM param;
	param.Int = PtnNo;
	param.Unsigned = idx;

	Param = &param;
	CommandFunc = DoReadSeg;
	ret = RetryCommand();

	return ret;
}

bool UP550::CompareSTC(PTN ptn, AnsiString &returnMsg)
{
	// PTN.ERR (B0005), SSP1 (B0011), STC (B0013), 1.WZ1 (B0014), 1.WTM1 (B0016)
	bool bReturn = false;
	if(strSTC != "")
	{
		int StartIndex = 7;
		int ParsingIndex = 4;
		int OnOff = 0;
		AnsiString txData, rxData;

		// B0005
		rxData.sprintf("%s", strSTC.SubString(StartIndex, ParsingIndex).c_str());
		if(rxData != "0000")
		{
			returnMsg.sprintf("Main temp. controller STC Compare read fail. [PTN No=%d]", ptn.PtnNo);
			return bReturn;
        }
		StartIndex += ParsingIndex;

		// B0011, SSP1 (Start target setpoint)
		txData = ""; rxData = "";
		txData.sprintf("%04X", (int)(ptn.Stc.Ssp * 10));
		rxData.sprintf("%s", strSTC.SubString(StartIndex, ParsingIndex).c_str());
		if(txData.AnsiCompare(rxData) != 0)
		{
			returnMsg.sprintf("Main temp. controller SSP Compare fail. [PTN No=%d, Write=%s, Read=%s]", ptn.PtnNo, txData.c_str(), rxData.c_str());
			return bReturn;
		}
		StartIndex += ParsingIndex;

		// B0013, STC (Start Code)
		txData = ""; rxData = "";
		txData.sprintf("%04X", ptn.Stc.Stc);
		rxData.sprintf("%s", strSTC.SubString(StartIndex, ParsingIndex).c_str());
		if(txData.AnsiCompare(rxData) != 0)
		{
			returnMsg.sprintf("Main temp. controller STC Compare fail. [PTN No=%d, Write=%s, Read=%s]", ptn.PtnNo, txData.c_str(), rxData.c_str());
			return bReturn;
		}
		StartIndex += ParsingIndex;

		// 2020-02-05 yjyoo, Wait Zone 삭제
/*		// B0014
		txData = ""; rxData = "";
		txData.sprintf("%04X", (int)(ptn.Stc.Wz[0] * 10));
		rxData.sprintf("%s", strSTC.SubString(StartIndex, ParsingIndex));
		if(txData.AnsiCompare(rxData) != 0)
		{
			return bReturn;
		}
		StartIndex += ParsingIndex;

		// B0016
		txData = ""; rxData = "";
		txData.sprintf("%04X", ptn.Stc.WTm[0]);
		rxData.sprintf("%s", strSTC.SubString(StartIndex, ParsingIndex));
		if(txData.AnsiCompare(rxData) != 0)
		{
			return bReturn;
		}
		StartIndex += ParsingIndex; */

    	bReturn = true;
	}

	return bReturn;
}

bool UP550::CompareSeg(int idx, PTN ptn, AnsiString &returnMsg)
{
	bool bReturn = false;
	if(strSeg != "")
	{
		int StartIndex = 7;
		int ParsingIndex = 4;
		int cnt = 0;
		AnsiString txData, rxData;

	// Read SEG Info
	// PTN.ERR (B0105), TSP1 (B0111), TIME (B0113), Junction code (B0140)
	// (Event - EvKind, OnTime, OffTime) * 8
	// B0116 ~ B0139

		// B0105, PTN.ERR
		rxData = "";
		rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
		if(rxData != "0000")
		{
			returnMsg.sprintf("Main temp. controller STC Compare read fail. [PTN No=%d]", ptn.PtnNo);
			return bReturn;
		}
		StartIndex += ParsingIndex;

		// B0111, TSP1
		txData = ""; rxData = "";
		txData.sprintf("%04X", (int)(ptn.Seg[idx].Tsp * 10));
		rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
		if(txData.AnsiCompare(rxData) != 0)
		{
			returnMsg.sprintf("Main temp. controller SEG TSP Compare fail. [PTN No=%d, SEG No=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, txData.c_str(), rxData.c_str());
			return bReturn;
		}
		StartIndex += ParsingIndex;

		// B0113, TIME
		txData = ""; rxData = "";
		txData.sprintf("%04X", ptn.Seg[idx].Time);
		rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
		if(txData.AnsiCompare(rxData) != 0)
		{
			returnMsg.sprintf("Main temp. controller SEG TIME Compare fail. [PTN No=%d, SEG No=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, txData.c_str(), rxData.c_str());
			return bReturn;
		}
		StartIndex += ParsingIndex;

		// B0140, Junction code
		txData = ""; rxData = "";
		txData.sprintf("%04X", ptn.Seg[idx].Jc);
		rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
		if(txData.AnsiCompare(rxData) != 0)
		{
			returnMsg.sprintf("Main temp. controller SEG JC Compare fail. [PTN No=%d, SEG No=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, txData.c_str(), rxData.c_str());
			return bReturn;
		}
		StartIndex += ParsingIndex;

		// B0116 ~ B0139
		for(int i=0; i<8; i++)
		{
			if( ptn.Seg[idx].EvKind[i] == ET_PVHIGH || ptn.Seg[idx].EvKind[i] == ET_PVLOW )
			{
				// EvKind
				txData = ""; rxData = "";
				txData.sprintf("%04X", ptn.Seg[idx].EvKind[i]);
				rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
				if(txData.AnsiCompare(rxData) != 0)
				{
					returnMsg.sprintf("Main temp. controller SEG EvKind Compare fail. [PTN No=%d, SEG No=%d, EVT idx=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, i, txData.c_str(), rxData.c_str());
					return bReturn;
				}
				StartIndex += ParsingIndex;

				// OnTime
				txData = ""; rxData = "";
				txData.sprintf("%04X", ptn.Seg[idx].OnType[i]);
				rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
				if(txData.AnsiCompare(rxData) != 0)
				{
					returnMsg.sprintf("Main temp. controller SEG OnTime Compare fail. [PTN No=%d, SEG No=%d, EVT idx=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, i, txData.c_str(), rxData.c_str());
					return bReturn;
				}
				StartIndex += ParsingIndex;

				// OffTime
				txData = ""; rxData = "";
				txData.sprintf("%04X", (ptn.Seg[idx].OffPe[i] * 10));
				rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
				if(txData.AnsiCompare(rxData) != 0)
				{
					returnMsg.sprintf("Main temp. controller SEG OffTime Compare fail. [PTN No=%d, SEG No=%d, EVT idx=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, i, txData.c_str(), rxData.c_str());
					return bReturn;
				}
				StartIndex += ParsingIndex;
			}
			else
			{
				// EvKind
				txData = ""; rxData = "";
				txData.sprintf("%04X", ptn.Seg[idx].EvKind[i]);
				rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
				if(txData.AnsiCompare(rxData) != 0)
				{
					returnMsg.sprintf("Main temp. controller SEG EvKind Compare fail. [PTN No=%d, SEG No=%d, EVT idx=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, i, txData.c_str(), rxData.c_str());
					return bReturn;
				}
				StartIndex += ParsingIndex;

				// OnTime
				txData = ""; rxData = "";
				txData.sprintf("%04X", ptn.Seg[idx].OnType[i]);
				rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
				if(txData.AnsiCompare(rxData) != 0)
				{
					returnMsg.sprintf("Main temp. controller SEG OnTime Compare fail. [PTN No=%d, SEG No=%d, EVT idx=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, i, txData.c_str(), rxData.c_str());
					return bReturn;
				}
				StartIndex += ParsingIndex;

				// OffTime
				txData = ""; rxData = "";
				txData.sprintf("%04X", ptn.Seg[idx].OffPe[i]);
				rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
				if(txData.AnsiCompare(rxData) != 0)
				{
					returnMsg.sprintf("Main temp. controller SEG OffTime Compare fail. [PTN No=%d, SEG No=%d, EVT idx=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, i, txData.c_str(), rxData.c_str());
					return bReturn;
				}
				StartIndex += ParsingIndex;
			}
		}
		bReturn = true;
	}

	return bReturn;
}

UP55A::UP55A(BaseChamber *chamber, PORTINFO portInfo)
	: DevMainTempController(chamber, portInfo)
{
	MainTempData.DIStatus = 0xFFFF;
	MainTempData.SP = 300;
	MainTempData.IsAuto = true;

	FScaleFactor = 10.0;
	FDevName = "UP55A";
	FParamCount = UP55A_PARAM_COUNT;
}

UP55A::~UP55A()
{

}

int UP55A::GetDIStatus(int diNum)
{
	int tmp = MainTempData.DIStatus;

	tmp = tmp >> diNum;
	return tmp & 0x1;
}

int UP55A::GetSegNo()
{
	return MainTempData.SegNo;
}

int UP55A::GetTotalSeg()
{
	return MainTempData.TotalSeg;
}

int UP55A::GetRemainingTime()
{
	return MainTempData.RemainingTime;
}

double UP55A::GetHeaterOut()
{
	return MainTempData.HeaterOutput / FScaleFactor;
}

double UP55A::GetPV()
{
	return MainTempData.PV / FScaleFactor;
}

double UP55A::GetSP()
{
	return MainTempData.SP / FScaleFactor;
}

bool UP55A::GetPVEventHigh()
{
	return MainTempData.PVEventHigh;
}

bool UP55A::GetPVEventLow()
{
	return MainTempData.PVEventLow;
}

bool UP55A::GetIsRun()
{
	return MainTempData.OpMode == 1 ? true : false;
}

bool UP55A::GetIsReset()
{
	return MainTempData.OpMode == 0 ? true : false;
}

bool UP55A::GetIsHold()
{
	return MainTempData.IsHold;
}

bool UP55A::GetIsLocal()
{
	return MainTempData.OpMode == 2 ? true : false;
}

bool UP55A::GetIsBurnout()
{
	return MainTempData.PV1Error & 0x0100;
}

int UP55A::GetOpMode()
{
	return MainTempData.OpMode;
}

bool UP55A::GetIsKeyLocked()
{
	if( MainTempData.RunResetKeylock == true &&
		MainTempData.ModeKeylock == true )
		return true;

	return false;
}

bool UP55A::GetIsAutoTuningOn()
{
	return MainTempData.Mod1 == 1;
}

bool UP55A::GetTimeEvent(int eventNo)
{
	int mask[7] = { 0x0001, 0x0002, 0x0004, 0x0010, 0x0020, 0x0040, 0x0100 };
	bool ret = false;

	if( eventNo > 0 &&
		eventNo <= 7 )
		ret = MainTempData.TimeEventStatus & mask[eventNo-1];

	return ret;
}

bool UP55A::IsRxDone()
{
	if( RxBuffer.IsEmpty() )
		return false;

	try
	{
		AnsiString strStx, strEtx, strOk, strCr;

		RxData = "";
		strStx.sprintf("%c", STX);
		strEtx.sprintf("%c", ETX);
		strOk = "OK";
		strCr.sprintf("%c", CR);
		if( AnsiContainsStr(RxBuffer, strStx) &&
			AnsiContainsStr(RxBuffer, strEtx) &&
			AnsiContainsStr(RxBuffer, strOk) &&
			AnsiContainsStr(RxBuffer, strCr) )
		{
			// 20190228 - Save Log - jhlim
			if (SaveResponseLog)
			{
				SaveResponseLog = false;
				SaveSerialLog(DevName, RxBuffer, FALSE);
			}

			int idx = RxBuffer.AnsiPos(strEtx);
			RxData = RxBuffer.SubString(2, idx-2);

			if( IsValidHex(RxData.SubString(1, 2)) )
			{
				if( HexToInt(RxData.SubString(1, 2)) != Addr )
					return false;
			}
			else
				return false;

			ValidRxData = true;
			return true;
		}
	}
	catch(...)
	{
		SaveResponseLog = false;
		return false;
	}

	return false;
}

void UP55A::GetMonitorCommands(TStringList *list)
{
	// D2316 Operation Mode
	// 0: RESET (Stop of program operation)
	// 1: PROG (Start of program operation)
	// 2: LOCAL (Start of local-mode operation)
	// 3: REM (Start of remote-mode operation)

	// D2317 HOLD
	// 0: OFF (Cancel release (Program operation restart))
	// 1: ON (Pause)

	// D2001 AD1.E (AD converter error status 1)
	// D2003 PV_L1 (Loop-1 measurment value)
	// D2004 CSP_L1 (Loop-1 control setpoint)
	// D2005 OUT_L1 (Loop-1 control output)
	// D2039 PV_EV (PV Event Status)
	// D2040 TIME_EV_1 (Time Event Status)
	// D2015 C.PTNO. (Current program pattern number)
	// D2016 SEG.N (Current segment number)
	// D2017 SEG_RTIME (Remaining segment-time during operation/Elapsed time during wait.t)
	// D2044 SEG_USE (Number of segments contained in the selected pattern)
	// D5821 RUN-L Front panel RUN key lock
	// D5824 MODE-L Front panel MODE key lock

	// D7601 DI (DI1-DI3 terminal status: equipped as standard) : DI1 ~ DI3
	// D7605 DI_E4 (DI41-DI46 terminal status: E4-terminal area) : DI4 ~ DI8
	// D2308 AT_L1 (Loop-1 auto tuning switch)
	// D2319 A.M_L1 (Loop-1 AUTO/MAN switch)
	// D2304 S.R (STOP/RUN switch)	// 20170609 - Manual Test - jhlim
	// D5012 ZON (Zone PID selection)	// 20181218 - DVF func Add - jhlim
	// D8107 S.PID (Segment PID Number selection)	// 20181218 - DVF func Add - jhlim
	// D2117 OH_L1_* (Control output high limit)

	AnsiString regs;
	AnsiString txData;

	regs = "D2316,D2317,D2001,D2003,D2004,D2005,D2039,D2040,D2015,D2016,D2017,D2044,D5821,D5824,D7601,D7605,D2308,D2319,D5012,D8107,D2117";
	txData.sprintf("%c%02d010WRR21%s%c%c",
				   STX,
				   Addr,
				   regs.c_str(),
				   ETX,
				   CR);
	list->Add(txData);
}

void UP55A::DecodeMonitor()
{
	// 20181218 - DVF func Add - jhlim
//	if( RxData.Length() != 78 )
	if( RxData.Length() != 90 )
		return;

	MAINTEMPDATA data;

	try
	{
		int status;
		int curSeg;
		int startIdx = 7;
		int tmp = 0;
		const int len = 4;
		AnsiString str;

		str = RxData.SubString(startIdx, RxData.Length()-6);
		if( IsValidHex(str) == false )
			return;

		// Operation Mode : D2316
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.OpMode = HexToInt(str);
		else
			return;
		startIdx += len;

		if( data.OpMode > 4 )
			int i =0;

		// HOLD : D2317
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.IsHold = HexToInt(str)==1?true:false;
		else
			return;
		startIdx += len;

		// AD1.E : D2001
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.PV1Error = HexToInt(str);
		else
			return;
		startIdx += len;

		// PV_L1 : D2003
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.PV = HexToInt(str);
		else
			return;
		startIdx += len;

		// CSP_L1 : D2004
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.SP = HexToInt(str);
		else
			return;
		startIdx += len;

		// OUT_L1 : D2005
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.HeaterOutput = HexToInt(str);
		else
			return;
		startIdx += len;

		// PV_EV : D2039
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
		{
			status = HexToInt(str);
			data.PVEventHigh = ((status & 0x01) != 0);
			data.PVEventLow = ((status & 0x02) != 0);
		}
		else
			return;
		startIdx += len;

		// TIME_EV_1 : D2040
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.TimeEventStatus = HexToInt(str);
		else
			return;
		startIdx += len;

		// C.PTNO. : D2015
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.PtnNo = HexToInt(str);
		else
			return;
		startIdx += len;

		// SEG.N : D2016
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.SegNo = HexToInt(str);
		else
			return;
		startIdx += len;

		// SEG_RTIME : D2017
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.RemainingTime = HexToInt(str);
		else
			return;
		startIdx += len;

		// SEG_USE : D2044
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.TotalSeg = HexToInt(str);
		else
			return;
		startIdx += len;

		// RUN-L : D5821
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.RunResetKeylock = HexToInt(str) == 1 ? true : false;
		else
			return;
		startIdx += len;

		// MODE-L : D5824
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.ModeKeylock = HexToInt(str) == 1 ? true : false;
		else
			return;
		startIdx += len;

		// DI : D7601
		tmp = 0;
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			tmp = HexToInt(str) & 0x0007;
		else
			return;
		startIdx += len;

		// DI_E4 : D7605
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
		{
			int tmp2;
			tmp2 = HexToInt(str);
			tmp2 = tmp2 << 3;
			data.DIStatus = tmp | tmp2;
		}
		else
			return;
		startIdx += len;

		// AT_L1 : D2308
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
			data.Mod1 = HexToInt(str);
		else
			return;
		startIdx += len;

		// A.M_L1 : D2319
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
		{
			int val = HexToInt(str);
			data.IsAuto = val == 0 ? true : false;
		}
		else
			return;
		startIdx += len;

		// ZON : D5012
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
		{
			int val = HexToInt(str);
			data.SelPIDZone = val;
		}
		else
			return;
		startIdx += len;

		// S.PID : D8107
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
		{
			int val = HexToInt(str);
			data.SelPIDNo = val;
		}
		else
			return;
		startIdx += len;

		// OH_L1_* (Control output high limit) : D2117
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
		{
			int val = HexToInt(str);
			data.OutputLimit = val;
		}
		else
			return;

		MainTempData = data;
 	}
	catch(...)
	{
		return;
	}

	InitRxValues();
}

void UP55A::DoWriteParameter(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;
	int idx = param->Int;

	SaveResponseLog = true;
	txData.sprintf(UP55AParamStr[idx],
				   STX,
				   Addr,
				   ETX,
				   CR);

	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP55A::DoDeletePattern(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;
	int ptnNo = param->Int;

	SaveResponseLog = true;
	// D8991 : CLR.P (Program pattern clearance)
	// D8992 : CLR.TRG (Clearance trigger)
	txData.sprintf("%c%02d010WWRD8991,02,%04X0001%c%c",
				   STX,
				   Addr,
				   ptnNo,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

/*	// B0063, PTE.TRG (Program pattern deleting trigger)
	txData.sprintf("%c%02d010WWRB0063,01,0001%c%c",
				   STX,
				   Addr,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();*/

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP55A::DoWriteSTC(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData, tmpData1, tmpData2;
	PTN ptn = (*(PTN*)param->Param);

	// D8001 PTNO._C Program pattern number selection 0: Not select program pattern 1 to 30
	// D8002 SEGNO._C Segment number designation
	// D8003 SSP_L1 Loop-1 Starting target setpoint
	// D8004 SSP_L2 Loop-2 Starting target setpoint (Setting range: P.RL to P.RH)
	// D8005 STC Start code
	//	0: SSP (Program operation begins with the starting target setpoint.
	//	1: RAMP (Ramp-prioritized PV start)
	//	2: TIME (Time-prioritized PV start)
	//	4: LSP (Local-mode start)
	//	5: RSP (Remote-mode start)
	// D8006 WT.SW1 Wait function ON/OFF 1
	// D8007 WZ.UP1 Upper-side wait zone 1
	// D8008 WZ.LO1 Lower-side wait zone 1
	// D8009 WT.TM1 Wait time 1
	// D8010 WT.SW2 Wait function ON/OFF 2
	// D8011 WZ.UP2 Upper-side wait zone 2
	// D8012 WZ.LO2 Lower-side wait zone 2
	// D8013 WT.TM2 Wait time 2
	// D8014 WT.SW3 Wait function ON/OFF 3
	// D8015 WZ.UP3 Upper-side wait zone 3
	// D8016 WZ.LO3 Lower-side wait zone 3
	// D8017 WT.TM3 Wait time 3
	// D8018 WT.SW4 Wait function ON/OFF 4
	// D8019 WZ.UP4 Upper-side wait zone 4
	// D8020 WZ.LO4 Lower-side wait zone 4
	// D8021 WT.TM4 Wait time 4
	// D8022 WT.SW5 Wait function ON/OFF 5
	// D8023 WZ.UP5 Upper-side wait zone 5
	// D8024 WZ.LO5 Lower-side wait zone 5
	// D8025 WT.TM5 Wait time 5
	// D8026 R.CYCL Number of repeat cycles
	// D8027 R.STRT Repeat cycle start segment number
	// D8028 R.END Repeat cycle end segment number

//	tmpData1 = "010WWRD8001,28,";
/*	tmpData1 = "010WWRD8001,05,";

	//				D8001 8002 8003 8004 8005
	tmpData2.sprintf("%04X0000%04X0000%04X",
					 ptn.PtnNo,
					 (int)(ptn.Stc.Ssp * 10),
					 ptn.Stc.Stc);
	tmpData1 += tmpData2; */

	// 2020-02-06 yjyoo, Wait Zone, Repeat 삭제
/*
	for(int i=0; i<5; i++)
	{
		int onoff = 0;

		if( ptn.Stc.WTm[i] != 0 &&
			ptn.Stc.Wz[i] != 0.0 )
			onoff = 1;

		//			   WT.SW,WZ.UP,WZ.LO,WT.TM
		tmpData2.sprintf("%04X%04X%04X%04X",
						 onoff,
						 (int)(ptn.Stc.Wz[i] * 10),
						 (int)(ptn.Stc.Wz[i] * 10),
						 ptn.Stc.WTm[i]);
		tmpData1 += tmpData2;
	}

	//		  D8026 8027 8028
	// 20190305 - Parameter Value Chamber - jhlim
	//tmpData2 = "000000000000";
	tmpData2 = "000000010001";
	tmpData1 += tmpData2;
*/
	SaveResponseLog = true;
//	txData.sprintf("%c%02d%s%c%c",
//				   STX,
//				   Addr,
//				   tmpData1,
//				   ETX,
//				   CR);

	// 2020-06-12 yjyoo, Local mode temp. 추가
    int localTemp = (int)(ptn.LocalTemp * 10);
	txData.sprintf("%c%02d010WRW06D8001,%04X,D8002,0000,D8003,%04X,D8004,0000,D8005,%04X,D2201,%04X%c%c",
				   STX,
				   Addr,
				   ptn.PtnNo,
				   (int)(ptn.Stc.Ssp*10),
				   ptn.Stc.Stc,
                   localTemp,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP55A::DoWriteSeg(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData, tmp1, tmp2, tmp3;
	int idx = param->Int;
	PTN ptn = (*(PTN*)param->Param);

	tmp1 = "010WWRD8101,48,";
	//			 D8101 8102 8103 8104 8105 8106 8107 8108
	tmp2.sprintf("%04X%04X%04X0000%04XFFFF%04X%04X",
				 ptn.PtnNo,
				 idx+1,
				 (int)(ptn.Seg[idx].Tsp * 10),
				 ptn.Seg[idx].Time,
				 ptn.Seg[idx].Pid,
				 ptn.Seg[idx].Jc);
	tmp1 += tmp2;
					SaveSerialLog(DevName, tmp1, TRUE);
	int cnt = 0;
	int eventType = 1;
	int temp;
	int value;
	int Index = 0;

	// PV High Event
	if (HasPVHighEvent(ptn, idx+1, Index))
	{
		eventType = 1;
		temp = GetPVHighOffset(ptn, idx+1);
		value = (int)((ptn.Seg[idx].Tsp + temp) * 10.0);
		//			  PV.TY,PV.EV
		tmp2.sprintf("%04X%04X", eventType, value);
	}
	else
	{
		//	   PV.TY,PV.EV
		tmp2 = "00000000";
	}
	tmp1 += tmp2;
	cnt++;

	SaveSerialLog(DevName, tmp1, TRUE);
	// PV Low Event
	if (HasPVLowEvent(ptn, idx+1, Index))
	{
		eventType = 2;
		temp = GetPVLowOffset(ptn, idx+1);
		value = (int)((ptn.Seg[idx].Tsp - temp) * 10.0);
		//			  PV.TY,PV.EV
		tmp2.sprintf("%04X%04X", eventType, value);
	}
	else
	{
		//	   PV.TY,PV.EV
		tmp2 = "00000000";
	}
	tmp1 += tmp2;
	cnt++;
	SaveSerialLog(DevName, tmp1, TRUE);
	for(int i=cnt; i<8; i++)
	{
		//		PV.TY,PV.EV
		tmp2 = "00000000";
		tmp1 += tmp2;
	}

	// Time Event
/*	cnt = 0;
	for(int i=0; i<8; i++)
	{
		if( ptn.Seg[idx].EvKind[i] >= ET_TIMEEVENT1 &&
			ptn.Seg[idx].EvKind[i] <= ET_TIMEEVENT7 )
		{
			//			  TME, T.ON,T.OF
			tmp2.sprintf("0001%04X%04X",
						 ptn.Seg[idx].OnType[i],
						 ptn.Seg[idx].OffPe[i]);
			tmp1 += tmp2;
			cnt++;
		}
	}
	for(int i=cnt; i<8; i++)
	{
		//		TME, T.ON,T.OF
		tmp2 = "000000000000";
		tmp1 += tmp2;
	}*/

	AnsiString timeEvent[8];
	for(int i=0; i<8; i++)
		timeEvent[i] = "000000000000";
	for(int i=0; i<8; i++)
	{
		if( ptn.Seg[idx].EvKind[i] >= ET_TIMEEVENT1 &&
			ptn.Seg[idx].EvKind[i] <= ET_TIMEEVENT7 )
		{
			tmp2 = (ptn.Seg[idx].OnType[i] == 0) ? "0001" : "0000";
			
								//T.ON,T.OF
			tmp2 = tmp2 + tmp3.sprintf("%04X%04X", ptn.Seg[idx].OnType[i],
											 ptn.Seg[idx].OffPe[i]);
			timeEvent[ptn.Seg[idx].EvKind[i]-1] = tmp2;
		}
	}
	for(int i=0; i<8; i++)
		tmp1 += timeEvent[i];
				 SaveSerialLog(DevName, tmp1, TRUE);
	SaveResponseLog = true;
	txData.sprintf("%c%02d%s%c%c",
			   STX,
			   Addr,
			   tmp1.c_str(),
			   ETX,
			   CR);
	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP55A::DoReset(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW01D2316,%04X%c%c",
				   STX,
				   Addr,
				   UP55X_RESET,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP55A::DoLocal(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW01D2316,%04X%c%c",
				   STX,
				   Addr,
				   UP55X_LOCAL,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP55A::DoHold(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW01D2317,%04X%c%c",
				   STX,
				   Addr,
				   param->Int,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP55A::DoResume(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW01D2317,0000%c%c",
				   STX,
				   Addr,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP55A::DoLockKey(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW03D5821,0001,D5822,0001,D5824,0001%c%c",
				   STX,
				   Addr,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP55A::DoUnlockKey(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW03D5821,0000,D5822,0000,D5824,0000%c%c",
				   STX,
				   Addr,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP55A::DoSetMode(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	if( param->Bool )
		txData.sprintf("%c%02d010WRW01D2319,0000%c%c", STX, Addr, ETX, CR);
	else
		txData.sprintf("%c%02d010WRW01D2319,0001%c%c", STX, Addr, ETX, CR);

	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP55A::DoNextSeg(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW01D2318,%04X%c%c",
				   STX,
				   Addr,
				   1,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP55A::DoRun(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW02D2322,%04X,D2316,%04X%c%c",
				   STX,
				   Addr,
				   param->Int,
				   UP55X_PROGRAM,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool UP55A::WriteParameter(int idx)
{
	bool ret = false;
	if( idx >= FParamCount )
		return false;

	PARAM param;
	param.Int = idx;

	Param = &param;
	CommandFunc = DoWriteParameter;
	ret = RetryCommand();

	return ret;
}

bool UP55A::DeletePattern(int ptnNo)
{
	bool ret = false;

	PARAM param;
	param.Int = ptnNo;

	Param = &param;
	CommandFunc = DoDeletePattern;
	ret = RetryCommand();

	return ret;
}

bool UP55A::WriteSTC(PTN ptn)
{
	bool ret = false;

	PARAM param;
	param.Param = &ptn;

	Param = &param;
	CommandFunc = DoWriteSTC;
	ret = RetryCommand();

	return ret;
}

bool UP55A::WriteSeg(int idx, PTN ptn)
{
	bool ret = false;

	PARAM param;
	param.Int = idx;
	param.Param = &ptn;

	Param = &param;
	CommandFunc = DoWriteSeg;
	ret = RetryCommand();

	return ret;
}

bool UP55A::Reset()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoReset;
	ret = RetryCommand();

	WaitForSingleObject((void*)Handle, 500);

	return ret;
}

bool UP55A::Local()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoLocal;
	ret = RetryCommand();

	WaitForSingleObject((void*)Handle, 500);

	return ret;
}

bool UP55A::Resume()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoResume;
	ret = RetryCommand();

	WaitForSingleObject((void*)Handle, 500);

	return ret;
}

bool UP55A::Run(int ptnNo)
{
	bool ret = false;

	PARAM param;
	param.Int = ptnNo;

	Param = &param;
	CommandFunc = DoRun;
	ret = RetryCommand();

	WaitForSingleObject((void*)Handle, 500);

	return ret;
}

bool UP55A::Hold()
{
	bool ret = false;

	PARAM param;
	param.Int = 1;

	Param = &param;
	CommandFunc = DoHold;
	ret = RetryCommand();

	WaitForSingleObject((void*)Handle, 500);

	return ret;
}

bool UP55A::LockKey()
{
	bool ret = false;

	PARAM param;
	param.Int = 0;

	Param = &param;
	CommandFunc = DoLockKey;
	ret = RetryCommand();

	return ret;
}

bool UP55A::UnlockKey()
{
	bool ret = false;

	PARAM param;
	param.Int = 0;

	Param = &param;
	CommandFunc = DoUnlockKey;
	ret = RetryCommand();

	return ret;
}

bool UP55A::SetAutoMode(bool autoMode/*=true*/)
{
	bool ret = false;

	PARAM param;
	param.Bool = autoMode;

	Param = &param;
	CommandFunc = DoSetMode;
	ret = RetryCommand();

	return ret;
}

bool UP55A::GotoNextSeg()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoNextSeg;
	ret = RetryCommand();

	return ret;
}

// 20170613 - Manual Test - jhlim
void UP55A::DoSetTestTempOn(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	int nTemp = param->Int * 10;
	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW04D2316,%04X,D2201,%04X,D2203,0001,D2204,0000%c%c",
					STX,
					Addr,
					UP55X_LOCAL,
					nTemp,
					ETX,
					CR);

	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP55A::DoSetTestTempOff(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	int nTemp = param->Int * 10;
	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW04D2316,%04X,D2201,%04X,D2203,0001,D2204,0000%c%c",
					STX,
					Addr,
					UP55X_RESET,
					nTemp,
					ETX,
					CR);

	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool UP55A::SetTestTempOn(int nTemp)
{
	bool ret = false;

	PARAM param;
	param.Int = nTemp;

	Param = &param;
	CommandFunc = DoSetTestTempOn;
	ret = RetryCommand();

	return ret;
}

bool UP55A::SetTestTempOff(int nTemp)
{
	bool ret = false;

	PARAM param;
	param.Int = nTemp;

	Param = &param;
	CommandFunc = DoSetTestTempOff;
	ret = RetryCommand();

	return ret;
}

// 20181218 - DVF func Add - jhlim
void UP55A::DoSelPIDZone(PARAM *param)
{
	int nZoneNo = param->Int;
	if(nZoneNo < 0 && nZoneNo > 5)
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW01D5012,%04X%c%c",
				   STX,
				   Addr,
				   nZoneNo,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP55A::DoSelPIDNo(PARAM *param)
{
	// ZON Setting -> 0 ( Segment PID selection ) 아니면 기능 사용 불가
	if(IsSelPIDZone != 0)
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	int nPIDNo = param->Int;
	AnsiString txData;

	SaveResponseLog = true;
	// D8101 : Pattern No
	// D8102 : Seg No
	// D8107 : Pid No
	txData.sprintf("%c%02d010WRW03D8101,%04X,D8102,%04X,D8107,%04X%c%c",
				   STX,
				   Addr,
				   MainTempData.PtnNo,
				   MainTempData.SegNo,
				   nPIDNo,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool UP55A::SelPIDZone(int ZoneNo)
{
	bool ret = false;

	PARAM param;
	param.Int = ZoneNo;

	Param = &param;
	CommandFunc = DoSelPIDZone;
	ret = RetryCommand();

	return ret;
}

bool UP55A::SelPIDNo(int PIDNo)
{
	bool ret = false;

	PARAM param;
	param.Int = PIDNo;

	Param = &param;
	CommandFunc = DoSelPIDNo;
	ret = RetryCommand();

	return ret;
}

// 20190227 - Pattern Check - jhlim
void UP55A::DoReadSTC(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	int PtnNo = param->Int;
	AnsiString txData = "";
	strSTC = "";

	SaveResponseLog = true;
	// Ptn No, Seg No Setting
	txData.sprintf("%c%02d010WWRD8001,02,%04X0000%c%c",
					STX,
					Addr,
					PtnNo,
					ETX,
					CR);
	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	TxData(txData);
	WaitRx1();

	SaveResponseLog = true;
	// Read STC Info
	// 2020-02-06 yjyoo, Wait Zone, Repeat 삭제
/*	txData.sprintf("%c%02d010WRDD8003,26%c%c",
					STX,
					Addr,
					ETX,
					CR); */
	txData.sprintf("%c%02d010WRDD8003,03%c%c",
					STX,
					Addr,
					ETX,
					CR);

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	TxData(txData);
	if(WaitRx1())
	{
		strSTC = RxData;
	}

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void UP55A::DoReadSeg(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	int PtnNo = param->Int;
	int idx = param->Unsigned;
	AnsiString txData = "";
	strSeg = "";

	SaveResponseLog = true;
	// Ptn No, Seg No Setting
	txData.sprintf("%c%02d010WWRD8101,02,%04X%04X%c%c",
					STX,
					Addr,
					PtnNo,
					idx + 1,
					ETX,
					CR);
	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	TxData(txData);
	WaitRx1();

	SaveResponseLog = true;
	// Read SEG Info
	txData.sprintf("%c%02d010WRDD8103,46%c%c",
					STX,
					Addr,
					ETX,
					CR);
	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);
	SaveSerialLog(DevName, "DoReadSeg_2", TRUE);
	TxData(txData);
	if(WaitRx1())
	{
		strSeg = RxData;
	}

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool UP55A::ReadSTC(int PtnNo)
{
	bool ret = false;
	PARAM param;
	param.Int = PtnNo;

	Param = &param;
	CommandFunc = DoReadSTC;
	ret = RetryCommand();

	return ret;
}

bool UP55A::ReadSeg(int PtnNo, int idx)
{
	bool ret = false;
	PARAM param;
	param.Int = PtnNo;
	param.Unsigned = idx;

	Param = &param;
	CommandFunc = DoReadSeg;
	ret = RetryCommand();

	return ret;
}

bool UP55A::CompareSTC(PTN ptn, AnsiString &returnMsg)
{
	SaveSerialLog("Main temp", "CompareSTC", FALSE);
	// D8003 ~ D8028
	bool bReturn = false;
	if(strSTC != "")
	{
		int StartIndex = 7;
		int ParsingIndex = 4;
		int OnOff = 0;
		AnsiString txData, rxData;

		// D8003, SSP
		txData = ""; rxData = "";
		txData.sprintf("%04X", (int)(ptn.Stc.Ssp * 10));
		rxData.sprintf("%s", strSTC.SubString(StartIndex, ParsingIndex).c_str());
		SaveSerialLog("Main temp", rxData, FALSE);
		if(txData.AnsiCompare(rxData) != 0)
		{
			returnMsg.sprintf("Main temp. controller SSP Compare fail. [PTN No=%d, Write=%s, Read=%s]", ptn.PtnNo, txData.c_str(), rxData.c_str());
			SaveSerialLog("Main temp", returnMsg, FALSE);
			return bReturn;
		}
		StartIndex += ParsingIndex;
		// 20191028 - Parameter No Use - jhlim
#if 0
		// D8004
		txData = ""; rxData = "";
		txData.sprintf("0000");
		rxData.sprintf("%s", strSTC.SubString(StartIndex, ParsingIndex).c_str());
        SaveSerialLog("Main temp", rxData, FALSE);
 //		if(txData.AnsiCompare(rxData) != 0)
		{
//			return bReturn;
		}
#endif
		StartIndex += ParsingIndex;

		// D8005, STC
		txData = ""; rxData = "";
		txData.sprintf("%04X", ptn.Stc.Stc);
		rxData.sprintf("%s", strSTC.SubString(StartIndex, ParsingIndex).c_str());
		SaveSerialLog("Main temp", rxData, FALSE);
		if(txData.AnsiCompare(rxData) != 0)
		{
			returnMsg.sprintf("Main temp. controller STC Compare fail. [PTN No=%d, Write=%s, Read=%s]", ptn.PtnNo, txData.c_str(), rxData.c_str());
			SaveSerialLog("Main temp", returnMsg, FALSE);
			return bReturn;
		}
		StartIndex += ParsingIndex;

		// 2020-02-06 yjyoo, Wait Zone, Repeat 삭제
/*		// D8006 ~ D8025
		for(int i = 0; i<5; i++)
		{
			// OnOff
			if( ptn.Stc.WTm[i] != 0 && ptn.Stc.Wz[i] != 0.0 )
			{
				OnOff = 1;
			}
			else
			{
             	OnOff = 0;
            }
			// WT.SW
			txData = ""; rxData = "";
			txData.sprintf("%04X", OnOff);
			rxData.sprintf("%s", strSTC.SubString(StartIndex, ParsingIndex));
			if(txData.AnsiCompare(rxData) != 0)
			{
				return bReturn;
			}
			StartIndex += ParsingIndex;
			// WZ.UP
			txData = ""; rxData = "";
			txData.sprintf("%04X", (int)(ptn.Stc.Wz[i] * 10));
			rxData.sprintf("%s", strSTC.SubString(StartIndex, ParsingIndex));
			if(txData.AnsiCompare(rxData) != 0)
			{
				return bReturn;
			}
			StartIndex += ParsingIndex;
			// WZ.LO
			txData = ""; rxData = "";
			txData.sprintf("%04X", (int)(ptn.Stc.Wz[i] * 10));
			rxData.sprintf("%s", strSTC.SubString(StartIndex, ParsingIndex));
			if(txData.AnsiCompare(rxData) != 0)
			{
				return bReturn;
			}
			StartIndex += ParsingIndex;
			// WT.TM
			txData = ""; rxData = "";
			txData.sprintf("%04X", ptn.Stc.WTm[i]);
			rxData.sprintf("%s", strSTC.SubString(StartIndex, ParsingIndex));
			if(txData.AnsiCompare(rxData) != 0)
			{
				return bReturn;
			}
			StartIndex += ParsingIndex;
		}

		// D8026
		txData = ""; rxData = "";
		txData.sprintf("0000");
		rxData.sprintf("%s", strSTC.SubString(StartIndex, ParsingIndex));
		if(txData.AnsiCompare(rxData) != 0)
		{
			return bReturn;
		}
		StartIndex += ParsingIndex;
		// D8027
		txData = ""; rxData = "";
		txData.sprintf("0001");
		rxData.sprintf("%s", strSTC.SubString(StartIndex, ParsingIndex));
		if(txData.AnsiCompare(rxData) != 0)
		{
			return bReturn;
		}
		StartIndex += ParsingIndex;
		// D8028
		txData = ""; rxData = "";
		txData.sprintf("0001");
		rxData.sprintf("%s", strSTC.SubString(StartIndex, ParsingIndex));
		if(txData.AnsiCompare(rxData) != 0)
		{
			return bReturn;
		}
		StartIndex += ParsingIndex;
 */
    	bReturn = true;
	}

	return bReturn;
}

bool UP55A::CompareSeg(int idx, PTN ptn, AnsiString &returnMsg)
{
	// D8103 ~ D8148
	bool bReturn = false;
	if(strSeg != "")
	{
		int StartIndex = 7;
		int ParsingIndex = 4;
		int cnt = 0;
		AnsiString txData, rxData;

		// D8103, TSP_L1
		txData = ""; rxData = "";
		txData.sprintf("%04X", (int)(ptn.Seg[idx].Tsp * 10));
		rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
		if(txData.AnsiCompare(rxData) != 0)
		{
			returnMsg.sprintf("Main temp. controller SEG TSP Compare fail. [PTN No=%d, SEG No=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, txData.c_str(), rxData.c_str());
			return bReturn;
		}
		StartIndex += ParsingIndex;
		// 20191028 - Parameter No Use - jhlim
#if 0
		// D8104, TSP_L2
		txData = ""; rxData = "";
		txData.sprintf("0000");
		rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
		if(txData.AnsiCompare(rxData) != 0)
		{
//			return bReturn;
		}
#endif
		StartIndex += ParsingIndex;

		// D8105, TIME
		txData = ""; rxData = "";
		txData.sprintf("%04X", ptn.Seg[idx].Time);
		rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
		if(txData.AnsiCompare(rxData) != 0)
		{
			returnMsg.sprintf("Main temp. controller SEG TIME Compare fail. [PTN No=%d, SEG No=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, txData.c_str(), rxData.c_str());
			return bReturn;
		}
		StartIndex += ParsingIndex;

		// D8106, TM.RT
		txData = ""; rxData = "";
		txData.sprintf("FFFF");
		rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
		if(txData.AnsiCompare(rxData) != 0)
		{
			returnMsg.sprintf("Main temp. controller SEG TM.RT Compare fail. [PTN No=%d, SEG No=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, txData.c_str(), rxData.c_str());
			return bReturn;
		}
		StartIndex += ParsingIndex;

		// D8107, S.PID
		txData = ""; rxData = "";
		txData.sprintf("%04X", ptn.Seg[idx].Pid);
		rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
		if(txData.AnsiCompare(rxData) != 0)
		{
			returnMsg.sprintf("Main temp. controller SEG PID Compare fail. [PTN No=%d, SEG No=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, txData.c_str(), rxData.c_str());
			return bReturn;
		}
		StartIndex += ParsingIndex;

		// D8108
		txData = ""; rxData = "";
		txData.sprintf("%04X", ptn.Seg[idx].Jc);
		rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
		if(txData.AnsiCompare(rxData) != 0)
		{
			returnMsg.sprintf("Main temp. controller SEG JC Compare fail. [PTN No=%d, SEG No=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, txData.c_str(), rxData.c_str());
			return bReturn;
		}
		StartIndex += ParsingIndex;

		// D8109 ~ D8124
		cnt = 0;
		int temp;
		int Index = 0;

		if (HasPVHighEvent(ptn, idx+1, Index))
		{
			if(GetPVHighOnType(ptn, idx+1) == 0 )
			{
				// PV.TY
				txData = ""; rxData = "";
				txData.sprintf("0000");
				rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
				if(txData.AnsiCompare(rxData) != 0)
				{
					returnMsg.sprintf("Main temp. controller SEG PV.TY Compare fail. [PTN No=%d, SEG No=%d, EVT idx=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, Index, txData.c_str(), rxData.c_str());
					return bReturn;
				}
				StartIndex += ParsingIndex;

				//PV.EV
				txData = ""; rxData = "";
				txData.sprintf("0000");
				rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
				if(txData.AnsiCompare(rxData) != 0)
				{
					returnMsg.sprintf("Main temp. controller SEG PV.EV Compare fail. [PTN No=%d, SEG No=%d, EVT idx=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, Index, txData.c_str(), rxData.c_str());
					return bReturn;
				}
				StartIndex += ParsingIndex;
			}
			else
			{
				// PV.TY
				txData = ""; rxData = "";
				txData.sprintf("0001");
				rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
				if(txData.AnsiCompare(rxData) != 0)
				{
					returnMsg.sprintf("Main temp. controller SEG PV.TY Compare fail. [PTN No=%d, SEG No=%d, EVT idx=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, Index, txData.c_str(), rxData.c_str());
					return bReturn;
				}
				StartIndex += ParsingIndex;

				//PV.EV
				txData = ""; rxData = "";
			   //	txData.sprintf("%04X", (int)((ptn.Seg[idx].Tsp + ptn.Seg[idx].OffPe[i]) * 10));
				temp = GetPVHighOffset(ptn, idx+1);
				txData.sprintf("%04X", (int)((ptn.Seg[idx].Tsp + temp) * 10));
				rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
				if(txData.AnsiCompare(rxData) != 0)
				{
					returnMsg.sprintf("Main temp. controller SEG PV.EV Compare fail. [PTN No=%d, SEG No=%d, EVT idx=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, Index, txData.c_str(), rxData.c_str());
					return bReturn;
				}
				StartIndex += ParsingIndex;
			}
			cnt++;
		}

		if( HasPVLowEvent(ptn, idx+1, Index) )
		{
			if( GetPVLowOnType(ptn, idx+1) == 0 )
			{
				// PV.TY
				txData = ""; rxData = "";
				txData.sprintf("0000");
				rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
				if(txData.AnsiCompare(rxData) != 0)
				{
					returnMsg.sprintf("Main temp. controller SEG PV.TY Compare fail. [PTN No=%d, SEG No=%d, EVT idx=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, Index, txData.c_str(), rxData.c_str());
					return bReturn;
				}
				StartIndex += ParsingIndex;

				//PV.EV
				txData = ""; rxData = "";
				txData.sprintf("0000");
				rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
				if(txData.AnsiCompare(rxData) != 0)
				{
					returnMsg.sprintf("Main temp. controller SEG PV.EV Compare fail. [PTN No=%d, SEG No=%d, EVT idx=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, Index, txData.c_str(), rxData.c_str());
					return bReturn;
				}
				StartIndex += ParsingIndex;
			}
			else
			{
				// PV.TY
				txData = ""; rxData = "";
				txData.sprintf("0002");
				rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
				if(txData.AnsiCompare(rxData) != 0)
				{
					return bReturn;
				}
				StartIndex += ParsingIndex;

				//PV.EV
				txData = ""; rxData = "";
				//txData.sprintf("%04X", (int)((ptn.Seg[idx].Tsp - ptn.Seg[idx].OffPe[i]) * 10));
				temp = GetPVLowOffset(ptn, idx+1);
				txData.sprintf("%04X", (int)((ptn.Seg[idx].Tsp - temp) * 10));
				rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
				if(txData.AnsiCompare(rxData) != 0)
				{
					returnMsg.sprintf("Main temp. controller SEG PV.EV Compare fail. [PTN No=%d, SEG No=%d, EVT idx=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, Index, txData.c_str(), rxData.c_str());
					return bReturn;
				}
				StartIndex += ParsingIndex;
			}
			cnt++;
		}

		for(int i=cnt; i<8; i++)
		{
			// PV.TY
			txData = ""; rxData = "";
			txData.sprintf("0000");
			rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
			if(txData.AnsiCompare(rxData) != 0)
			{
				returnMsg.sprintf("Main temp. controller SEG PV.TY Compare fail. [PTN No=%d, SEG No=%d, EVT idx=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, i, txData.c_str(), rxData.c_str());
				return bReturn;
			}
			StartIndex += ParsingIndex;

			//PV.EV
			txData = ""; rxData = "";
			txData.sprintf("0000");
			rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
			if(txData.AnsiCompare(rxData) != 0)
			{
				returnMsg.sprintf("Main temp. controller SEG PV.EV Compare fail. [PTN No=%d, SEG No=%d, EVT idx=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, i, txData.c_str(), rxData.c_str());
				return bReturn;
			}
			StartIndex += ParsingIndex;
		}

		// D8125 ~ D8148
		int EventKind = 0;
		AnsiString txTimeEvent[8];
		AnsiString rxTimeEvent[8];
		// tx, rx TimeEvnet array init
		for(int i=0; i<8; i++)
		{
			txTimeEvent[i] = "000000000000";
			rxTimeEvent[i] = "000000000000";
		}
		// txTimeEvent parsing
		for(int i=0; i<8; i++)
		{
			EventKind = ptn.Seg[idx].EvKind[i];
			if( EventKind >= ET_TIMEEVENT1 && EventKind <= ET_TIMEEVENT7 )
			{
				// TME
				txData = ""; rxData = "";
				txData = (ptn.Seg[idx].OnType[i] == 0) ? "0001" : "0000";
				rxData += txData;
				// T.ON
				txData = "";
				txData.sprintf("%04X",  ptn.Seg[idx].OnType[i]);
				rxData += txData;
				// T.OF
				txData = "";
				txData.sprintf("%04X", ptn.Seg[idx].OffPe[i]);
				rxData += txData;
				// txTimeEvent save
				txTimeEvent[EventKind - 1] = rxData;
            }
		}

		// rxTimeEvent parsing
		ParsingIndex = 12;
		for(int i=0; i<8; i++)
		{
			// TME T.ON T.OF
			rxData = "";
			rxData.sprintf("%s", strSeg.SubString(StartIndex, ParsingIndex).c_str());
			rxTimeEvent[i] = rxData;
			StartIndex += ParsingIndex;
		}

		// tx, rx TimeEvent Compare
		for(int i=0; i<8; i++)
		{
			// TME T.ON T.OF
			txData = ""; rxData = "";

			txData = txTimeEvent[i];
			rxData = rxTimeEvent[i];
			if(txData.AnsiCompare(rxData) != 0)
			{
				returnMsg.sprintf("Main temp. controller TimeEvent Compare fail. [PTN No=%d, SEG No=%d, EVT idx=%d, Write=%s, Read=%s]", ptn.PtnNo, idx+1, i, txData.c_str(), rxData.c_str());
				return bReturn;
			}
		}

		bReturn = true;
	}

	return bReturn;
}

void UP55A::DoWriteOutputLimit(PARAM *param)
{
	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	SaveResponseLog = true;
	txData.sprintf("%c%02d010WRW01D2117,%04X%c%c",
				   STX,
				   Addr,
				   param->Int,
				   ETX,
				   CR);
	TxData(txData);
	WaitRx1();

	// 20190228 - Save Log - jhlim
	SaveSerialLog(DevName, txData, TRUE);

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool UP55A::WriteOutputLimit(int limit)
{
	bool ret = false;

	PARAM param;
	param.Int = (int)(limit * FScaleFactor);

	Param = &param;
	CommandFunc = DoWriteOutputLimit;
	ret = RetryCommand();

	return ret;
}
