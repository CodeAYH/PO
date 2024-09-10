//---------------------------------------------------------------------------
#include <System.AnsiStrings.hpp>

#pragma hdrstop

#include "ST340.h"
#include "HelperFunctions.h"
#include "SerialPortManager.h"
#include "Chamber.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

ST340::ST340(BaseChamber *chamber, PORTINFO portInfo, int limitType)
	: DevLimitController(chamber, portInfo, limitType)
{
	FDevName = "ST340";
}

ST340::~ST340()
{

}

void ST340::DecodeMonitor()
{
	if( RxData.Length() != 23 )
		return;

	try
	{
		int startIdx = 10;
		const int len = 4;
		AnsiString str;

		// PV : D0001
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
		{
			FReceivedData.PV = HexToInt(str);
		}
		startIdx += len+1;

		// SP : D0201
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
		{
			FReceivedData.SP = HexToInt(str);
		}
		startIdx += len+1;

		// DP : D0605	소수점 위치
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
		{
			int dp = HexToInt(str);
			if( dp < 5 )
				DP = dp;
		}
	}
	catch(...)
	{
		return;
	}

	FBiasUpdated = false;
}

bool ST340::IsRxDone()
{
	if( RxBuffer.IsEmpty() )
		return false;

	try
	{
		AnsiString strStx, strOk, strCr, strEtx;

		strStx.sprintf("%c", STX);
		strEtx.sprintf("%c", CR);
		strOk = "OK";
		strCr.sprintf("%c%c", CR, LF);
		if( AnsiContainsStr(RxBuffer, strStx) &&
			AnsiContainsStr(RxBuffer, strEtx) &&
			AnsiContainsStr(RxBuffer, strOk) &&
			AnsiContainsStr(RxBuffer, strCr) )
		{
			int idx = RxBuffer.AnsiPos(strEtx);
			RxData = RxBuffer.SubString(2, idx-2);

			ValidRxData = true;
			return true;
		}
	}
	catch(...)
	{
		return false;
	}
	return false;
}

void ST340::GetMonitorCommands(TStringList *list)
{
	AnsiString regs, command;

	/* ST340
	 * D0001:현재PV
	 * D0002:현재SP (참고용)
	 * D0201:조절계에서 운전하고자 하는 설정치(SP1)를 설정
	 * D0401:경보-1의 종류를 설정
	 * D0406:ALT1에 의해 설정된 경보종류에 대한 경보점을 설정
	 * D0421:ATL1에서 편차알람 선택 시 경보점 High를 설정
	 * D0426:ATL1에서 편차알람 선택 시 경보점 Low를 설정
	*/

	regs = "0001,0201,0605";
	command.sprintf("%c%02dRRD,03,%s%c%c",
					STX,
					Addr,
					regs.c_str(),
					CR,
					LF);
	list->Add(command);
}

/*
주의
센서타입 업데이트하면, IN-U, IN-RH, IN-RL, SP, HYS-H, HYS-L, ONOFF 값들이 초기화 된다.
입력상한 하한 업데이트 하면, SP값이 0으로 초기화 된다.
따라서 센서타입을 가장 앞부분에 넣어야 한다.

Format : "%c|%02d|Cmd|Data|%c|%c", ST_STX, Address, ST_CR, ST_LF
typedef	enum { AUTOMAN, IN_TYPE, ONOFF, IN_UNIT, IN_RH, IN_RL, IN_DP, IN_SH, IN_SL, HYS_H, HYS_L }STCTRL_PARA;
				 0105     0601   0207     0602   0603   0604   0605   0607   0608   0615   0616
*/

/*
#define REG_COUNT	19
const int reg[REG_COUNT] = 		 { 105, 601, 207, 602,  603, 604, 605,   607, 608, 609, 610, 615, 616, 401, 402,  406, 412, 417, 422 };
const int valTemp[REG_COUNT] = 	 {   0,   1,   1,   0,  500,   0,   0,  5000,   0,   1,   1,   0,   0,   2,   1, 5000,   0,   0,   0 };
const int valPress1[REG_COUNT] = {   0,  21,   1,   0, 1000, 200,   1,   160,   0,   1,   1,   0,   0,   1,   1,  160,   0,   0,   0 };
const int valPress2[REG_COUNT] = {   0,  21,   1,   0, 1000, 200,   2,  1600,   0,   1,   1,   0,   0,   1,   1, 1600,   0,   0,   0 };
*/

void ST340::DoWriteBias(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	AnsiString txData, tmp;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	tmp.sprintf("%04X", FBias);
	if( FBias < 0 )
		tmp = tmp.SubString(5, 4);

	txData.sprintf("%c%02dWRD,01,0606,%s%c%c", STX, Addr, tmp.c_str(), CR, LF);
	TxData(txData);
	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void ST340::DoWriteSP(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	AnsiString txData, tmp;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	tmp.sprintf("%04X", FSP);
	if( FSP < 0 )
		tmp = tmp.SubString(5, 4);

	txData.sprintf("%c%02dWRD,01,0201,%s%c%c", STX, Addr, tmp.c_str(), CR, LF);
	TxData(txData);
	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void ST340::DoWriteLimitConfig(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;
	AnsiString tmp, tmp1, tmp2;
	double limit1, limit2;
	int regCount = 0;

	if( FLimitType == CHAMBER_TEMP_LIMIT )
	{
		limit1 = Limit1 + LimitOffset;
		if( limit1 > MaxLimit )
			limit1 = MaxLimit;

		limit2 = Limit2 + LimitOffset;
		if( limit2 > MaxLimit )
			limit2 = MaxLimit;
	}
	else
	if( FLimitType == PRESSURE_LIMIT )
	{
		limit1 = Limit1 + LimitOffset;
		if( limit1 > MaxLimit )
			limit1 = MaxLimit;

		limit2 = Limit2 + LimitOffset;
		if( limit2 > MaxLimit )
			limit2 = MaxLimit;
	}
	else
	if( FLimitType == HEATER_TEMP_LIMIT )
	{
		limit1 = MaxLimit;
		limit2 = MaxLimit;
	}
	else
	{
		limit1 = Limit1 + LimitOffset;
		if( limit1 > MaxLimit )
			limit1 = MaxLimit;

		limit2 = Limit2 + LimitOffset;
		if( limit2 > MaxLimit )
			limit2 = MaxLimit;
	}
	limit1 *= ScaleFactor;
	limit2 *= ScaleFactor;

	for(int i=0; i<ConfigDataCount; i++)
	{
		if( ConfigDataChanged[i] )
		{
			tmp.sprintf(",%04d,%04X", ConfigRegister[i], ConfigData[i]);
			tmp1 += tmp;
			regCount++;
		}
	}

	tmp.sprintf("%04X", FBias);
	if( FBias < 0 )
		tmp = tmp.SubString(5, 4);
	//             SP        AL-1      AL-2      BS
	tmp2.sprintf(",0201,%04X,0406,%04X,0407,%04X,0606,%s",
				 FSP, (int)limit1, (int)limit2, tmp.c_str());
	tmp1 += tmp2;

	int count = regCount+4;
	txData.sprintf("%c%02dWRD,%02d%s%c%c", STX, Addr, count, tmp1.c_str(), CR, LF);
	TxData(txData);
	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void ST340::DoReadLimitConfig(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData, regs, tmp;

	for(int i=0; i<ConfigDataCount; i++)
	{
		tmp.sprintf(",%04d", ConfigRegister[i]);
		regs += tmp;
	}
	txData.sprintf("%c%02dRRD,%02d%s,0201,0407,0606%c%c", STX, Addr, ConfigDataCount+3, regs.c_str(), CR, LF);
	TxData(txData);

	WaitRx1();

	RxTempData = RxData;

	ComPort->OnRxChar = nullptr;
}

bool ST340::IsValidLimitParam()
{
	int startIdx = 10;
	const int len = 4;
	int iValue;
	bool ret = true;
	AnsiString str;

	for(int i=0; i<ConfigDataCount; i++)
		ConfigDataChanged[i] = false;

	for(int i=0; i<ConfigDataCount; i++)
	{
		str = RxTempData.SubString(startIdx, len);
		if( IsValidHex(str) )
		{
			iValue = HexToInt(str);
			if( iValue != ConfigData[i] )
			{
				ConfigDataChanged[i] = true;
				ret = false;
			}
		}
		startIdx += len+1;
	}

	// SP : D0201
	str = RxTempData.SubString(startIdx, len);
	if( IsValidHex(str) )
	{
		iValue = HexToInt(str);
		if( FSP != iValue )
			ret = false;
	}
	startIdx += len+1;

	// AL-2 = Temp. Limit
	str = RxTempData.SubString(startIdx, len);
	if( IsValidHex(str) )
	{
		iValue = HexToInt(str);
		int temp = FLimit1 + FLimitOffset;
		if( temp > FMaxLimit )
			temp = FMaxLimit;
		if( temp != iValue )
			ret = false;
	}
	startIdx += len+1;

	// Bias
	str = RxTempData.SubString(startIdx, len);
	if( IsValidHex(str) )
	{
		iValue = HexToInt(str);
		if( FBias != iValue )
			ret = false;
	}

	return ret;
}

bool ST340::IsValidHexParam()
{
	int startIdx = 10;
	const int len = 4;
	int iValue;
	AnsiString str;

	for(int i=0; i<ConfigDataCount; i++)
	{
		str = RxTempData.SubString(startIdx, len);
		if( IsValidHex(str) == false )
			return false;
		startIdx += len+1;
	}

	// SP : D0201
	str = RxTempData.SubString(startIdx, len);
	if( IsValidHex(str) == false )
		return false;
	startIdx += len+1;

	// AL-2 = Temp. Limit
	str = RxTempData.SubString(startIdx, len);
	if( IsValidHex(str) == false )
		return false;
	startIdx += len+1;

	// Bias
	str = RxTempData.SubString(startIdx, len);
	if( IsValidHex(str) == false )
		return false;

	return true;
}

void ST340::DoCommunicate()
{
	if( CommandMode == true )
	{
		CommandFunc(Param);
		CommandMode = false;
		MonitorCount = 0;
	}
	else
	{
		DoMonitor();

		if( FAutoConfig == true )
		{
			MonitorCount++;
			if( MonitorCount > 5 )
			{
				DoReadLimitConfig(nullptr);
				if( IsValidHexParam() )
				{
					if( IsValidLimitParam() == false )
						DoWriteLimitConfig(nullptr);
				}
				MonitorCount = 0;
			}
		}
	}

	WaitForSingleObject((void *)Handle, 250);
}

void ST340::DoMonitor()
{
	if( ComPort == nullptr )
		return;

	ComPort->OnRxChar = OnRxChar;

	TStringList *list = new TStringList;
	AnsiString txData;

	GetMonitorCommands(list);
	for(int i=0; i<list->Count; i++)
	{
		txData = list->Strings[i];
		TxData(txData);
		if( WaitRx1() )
			DecodeMonitor();
	}
	delete list;

	ComPort->OnRxChar = nullptr;
}
