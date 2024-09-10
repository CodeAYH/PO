//---------------------------------------------------------------------------
#include <System.AnsiStrings.hpp>

#pragma hdrstop

#include "VXSeries.h"
#include "HelperFunctions.h"
#include "SerialPortManager.h"
#include "Chamber.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

VXSeries::VXSeries(BaseChamber *chamber, PORTINFO portInfo, int limitType)
	: DevLimitController(chamber, portInfo, limitType)
{
	FDevName = "VXSeries";
}

VXSeries::~VXSeries()
{

}

void VXSeries::DecodeMonitor()
{
	if( RxData.Length() != 28 )
		return;

	/* VXSeries
	 * D0000 : 현재 PV (CPV)
	 * D0001 : 현재 SP (CSV)
	 * D0003 : 소수점 자리수 (DP-P)
	 * D0103 : 설정값1 (SV-1)
	*/

	try
	{
		int startIdx = 10;
		const int len = 4;
		AnsiString str;

		// CPV : D0000
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
		{
			FReceivedData.PV = HexToInt(str);
		}
		startIdx += len+1;

		// CSV : D0001
		str = RxData.SubString(startIdx, len);
		startIdx += len+1;

		// DP-P : D0003	소수점 위치
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
		{
			int dp = HexToInt(str);
			if( dp < 5 )
				DP = dp;
		}
		startIdx += len+1;

		// SV-1 : D0103
		str = RxData.SubString(startIdx, len);
		if( IsValidHex(str) )
		{
			FReceivedData.SP = HexToInt(str);
		}
		startIdx += len+1;
	}
	catch(...)
	{
		return;
	}

	FBiasUpdated = false;
}

bool VXSeries::IsRxDone()
{
	if( RxBuffer.IsEmpty() )
		return false;

	try
	{
		AnsiString strStx, strOk, strCr, strEtx;

		char c =  RxBuffer[1];

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

void VXSeries::GetMonitorCommands(TStringList *list)
{
	AnsiString regs, command;

	/* VXSeries
	 * D0000 : 현재 PV (CPV)
	 * D0001 : 현재 SP (CSV)
	 * D0003 : 소수점 자리수 (DP-P)
	 * D0103 : 설정값1 (SV-1)
	*/

	regs = "0000,0001,0003,0103";
	command.sprintf("%c%02dDRR,04,%s%c%c",
					STX,
					Addr,
					regs.c_str(),
					CR,
					LF);
	list->Add(command);
}

void VXSeries::DoWriteBias(PARAM *param)
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

	txData.sprintf("%c%02dDWR,01,0909,%s%c%c", STX, Addr, tmp.c_str(), CR, LF);
	TxData(txData);
	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void VXSeries::DoWriteSP(PARAM *param)
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

	txData.sprintf("%c%02dDWR,01,0103,%s%c%c", STX, Addr, tmp.c_str(), CR, LF);
	TxData(txData);
	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void VXSeries::DoWriteLimitConfig(PARAM *param)
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
			AnsiString reg, data;
			reg.sprintf("%04d", ConfigRegister[i]);
			data.sprintf("%04X", ConfigData[i]);
			if( ConfigData[i] < 0 )
			{
				data = data.SubString(5, 4);
			}
			tmp.sprintf(",%s,%s", reg.c_str(), data.c_str());
			tmp1 += tmp;
			regCount++;
		}
	}

	tmp.sprintf("%04X", FBias);
	if( FBias < 0 )
		tmp = tmp.SubString(5, 4);
	//             SV-1      AL-1      AL-2      BIAS
	tmp2.sprintf(",0103,%04X,0301,%04X,0305,%04X,0909,%s",
				 FSP, (int)limit1, (int)limit2, tmp.c_str());
	tmp1 += tmp2;

	int count = regCount+4;
	txData.sprintf("%c%02dDWR,%02d%s%c%c", STX, Addr, count, tmp1.c_str(), CR, LF);
	TxData(txData);
	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void VXSeries::DoReadLimitConfig(PARAM *param)
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
	//                               SV-1 AL-1 AL-2 BIAS
	txData.sprintf("%c%02dDRR,%02d%s,0103,0301,0305,0909%c%c", STX, Addr, ConfigDataCount+4, regs.c_str(), CR, LF);
	TxData(txData);

	WaitRx1();

	RxTempData = RxData;

	ComPort->OnRxChar = nullptr;
}

bool VXSeries::IsValidLimitParam()
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

	// SV-1 : D103
	str = RxTempData.SubString(startIdx, len);
	if( IsValidHex(str) )
	{
		iValue = HexToInt(str);
		if( FSP != iValue )
			ret = false;
	}
	startIdx += len+1;

	// AL-1 = Temp. Limit
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

bool VXSeries::IsValidHexParam()
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

	// SV-1 : D0103
	str = RxTempData.SubString(startIdx, len);
	if( IsValidHex(str) == false )
		return false;
	startIdx += len+1;

	// AL-1 = Temp. Limit
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

void VXSeries::DoCommunicate()
{
	if( CommandMode == true )
	{
		CommandFunc(Param);
		CommandMode = false;
		MonitorCount = 0;

		WaitForSingleObject((void *)Handle, 250);
	}
	else
	{
		DoMonitor();
		WaitForSingleObject((void *)Handle, 250);

		if( FAutoConfig == true )
		{
			MonitorCount++;
			if( MonitorCount > 5 )
			{
				DoReadLimitConfig(nullptr);
				WaitForSingleObject((void *)Handle, 250);

				if( IsValidHexParam() )
				{
					if( IsValidLimitParam() == false )
					{
						DoWriteLimitConfig(nullptr);
						WaitForSingleObject((void *)Handle, 250);
					}
				}
				MonitorCount = 0;
			}
		}
	}
}

void VXSeries::DoMonitor()
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
