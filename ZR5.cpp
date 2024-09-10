//--------------------------------------------------------------------------
#include <System.AnsiStrings.hpp>

#pragma hdrstop

#include "ZR5.h"
#include "HelperFunctions.h"
#include "SerialPortManager.h"
#include "Chamber.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

ZR5::ZR5(BaseChamber *chamber, PORTINFO portInfo)
	: SerialDevice(chamber, portInfo)
{
	FIsValid = false;
	FIsCommandInit = false;
	DecodeMode = DECODE_DENSITY;
}

__fastcall ZR5::~ZR5()
{

}

bool ZR5::CheckDoubleValid(AnsiString data)
{
	// ���� �����Ϳ� ������ ���� ���ڰ� �ִ��� �˻�
	// 0 ~ 9, e, E, +, -, . �̿� ���ڴ� ����
	int len = data.Length();
	char tmp;
	bool ret;
	for(int i=1; i<=len; i++)
	{
		tmp = data[i];
		ret = ((tmp >= '0' && tmp <= '9') ||
				tmp == 'e' || tmp == 'E' ||
				tmp == '+' || tmp == '-' ||
				tmp == '.') ? true : false;
		if( ret == false )
			return false;
	}

	return true;
}

void ZR5::DecodeMonitor()
{
	try
	{
		RxData = RxData.Trim();

		if( RxData.IsEmpty() )
			return;

		if( CheckDoubleValid(RxData) == false )
			return;

//		if( DecodeMode == DECODE_DENSITY )
			FO2Density = RxData.ToDouble();
//		else
//			FFlowRate = RxData.ToDouble();
	}
	catch(...)
	{
		return;
	}
}

bool ZR5::IsRxDone()
{
	if( RxBuffer.IsEmpty() )
		return false;

	AnsiString strEtx;
	AnsiString strStx;

	try
	{
		if( AnsiContainsStr(RxBuffer, "ERROR") )
		{
			RxData = "";
//			if( DecodeMode == DECODE_DENSITY )
				IsValidO2Value = false;
//			else
//				IsValidFlowRateValue = false;
			ValidRxData = true;
			return true;
		}

//		if( DecodeMode == DECODE_DENSITY )
			IsValidO2Value = true;
//		else
//			IsValidFlowRateValue = true;

		if( IsValidO2Value )//&&
//			IsValidFlowRateValue )
			FIsValid = true;
		else
			FIsValid = false;

		strEtx.sprintf("%c", CR);
//		if( DecodeMode == DECODE_DENSITY )
			strStx = "M2";
//		else
//			strStx = "u ";
		if( AnsiContainsStr(RxBuffer, strStx) &&
			AnsiContainsStr(RxBuffer, strEtx) )
		{
			int idx = RxBuffer.AnsiPos(strEtx);
			RxData = RxBuffer.SubString(3, idx-3);
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

void ZR5::GetMonitorCommands(TStringList *list)
{
	AnsiString command;

	// ��� �� �б�
	command.sprintf("M2%c", CR);
	list->Add(command);

	// ���� �б�
//	command.sprintf("u3%c", CR);
//	list->Add(command);
}

void ZR5::DoMonitor()
{
	if( ComPort == nullptr )
		return;

	ComPort->OnRxChar = OnRxChar;

	TStringList *list = new TStringList;
	AnsiString txData;

	GetMonitorCommands(list);
	for(int i=0; i<list->Count; i++)
	{
		DecodeMode = i;
		txData = list->Strings[i];
		TxData(txData);
		if( WaitRx1() )
			DecodeMonitor();
	}
	delete list;

	ComPort->OnRxChar = nullptr;
}

void ZR5::DoCommandInit(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	AnsiString txData;

	txData.sprintf("VV%c", CR);
	TxData(txData);

	DelayTime(1000, false);
	TimeoutTimer->Interval = oldTimeout;
	FIsCommandInit = true;
}

bool ZR5::CommandInit()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoCommandInit;
	ret = RetryCommand();

	return ret;
}
