//--------------------------------------------------------------------------
#include <System.AnsiStrings.hpp>

#pragma hdrstop

#include "I7000DIOModule.h"
#include "HelperFunctions.h"
#include "SerialPortManager.h"
#include "Chamber.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

I7000DIOModule::I7000DIOModule(BaseChamber *chamber, PORTINFO portInfo, int moduleid)
	: SerialIODevice(chamber, portInfo, true)
{
	ModuleID = moduleid;
}

I7000DIOModule::~I7000DIOModule()
{

}

bool I7000DIOModule::GetReadValue(int idx)
{
	return FReadValue[idx];
}

//bool I7000DIOModule::GetWriteValue(int idx)
//{
//	return FWriteValue[idx];
//}
//
//void I7000DIOModule::SetWriteValue(int idx, bool value)
//{
//	FWriteValue[idx] = value;
//}

void I7000DIOModule::DoCommunicate()
{
	DoMonitor();
	WaitForSingleObject((void *)Handle, THREAD_WAIT_TIME);

	bool different = false;
	switch (ModuleID)
	{
	case I_7045:	// 모두 output
		if (FReadValue != FWriteValue)
		{
			different = true;
		}
	break;
	case I_7051:	// 모두 input
	break;
	case I_7055:	// 상위 8비트 output
	{
		for (int i = 8; i < 16; i++)
		{
			if (FReadValue[i] != FWriteValue[i])
			{
				different = true;
				break;
			}
		}
	}
	break;
	}

	if (different)
	{
		DoWriteOutput();
		WaitForSingleObject((void *)Handle, 20);
	}
}

void I7000DIOModule::DecodeMonitor()
{
	try
	{
		AnsiString answerMark = RxBuffer[1];

		if (answerMark != ">")
		{
			return;
		}

		AnsiString portDataStr = RxBuffer.SubString(2, 4);
		if (IsValidHex(portDataStr))
		{
			unsigned int portData = HexToInt(portDataStr);
			FReadValue = (unsigned int)portData;
			if (IsFirstRun)
			{
				FWriteValue = FReadValue;
				IsFirstRun = false;
			}
		}
	}
	catch(...)
	{
		return;
	}
}

bool I7000DIOModule::IsRxDone()
{
	if (RxBuffer.IsEmpty())
	{
		return false;
	}

	AnsiString strEtx;
	AnsiString strStx;

	try
	{
		strEtx.sprintf("%c", CR);
		strStx.sprintf("%c", '>');
		if (AnsiContainsStr(RxBuffer, strEtx) &&
			AnsiContainsStr(RxBuffer, strStx))
		{
			int idx = RxBuffer.AnsiPos(strEtx);
			RxData = RxBuffer.SubString(1, idx-1);
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

void I7000DIOModule::GetMonitorCommands(TStringList *list)
{
	AnsiString command;

	// 16 채널 읽기
	command.sprintf("%c%02X%c", '@', Addr, CR);
	list->Add(command);
}

void I7000DIOModule::DoMonitor()
{
	if (ComPort == nullptr)
	{
		return;
	}

	ComPort->OnRxChar = OnRxChar;

	TStringList *list = new TStringList;
	AnsiString txData;

	GetMonitorCommands(list);
	for (int i=0; i<list->Count; i++)
	{
		txData = list->Strings[i];
		TxData(txData);
		if (WaitRx1())
		{
			DecodeMonitor();
		}
	}
	delete list;

	ComPort->OnRxChar = nullptr;
}

void I7000DIOModule::DoWriteOutput()
{
	if (ComPort == nullptr)
	{
		return;
	}

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 300;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;
	int val = 0;
	switch (ModuleID)
	{
	case I_7045:
		for (int i=0; i<8; i++)
		{
			if (FWriteValue[i])
			{
				val += (int)pow(2.0, (double)i);
			}
		}
		txData.sprintf("%c%02X00%02X%c", '#', Addr, val, CR);
		TxData(txData);
		WaitRx1();

		val = 0;
		for (int i=8; i<16; i++)
		{
			if (FWriteValue[i])
			{
				val += (int)pow(2.0, (double)(i-8));
			}
		}
		txData.sprintf("%c%02X0B%02X%c", '#', Addr, val, CR);
		TxData(txData);
		WaitRx1();
	break;
	case I_7055:
		for (int i=8; i<16; i++)
		{
			if (FWriteValue[i])
			{
				val += (int)pow(2.0, (double)(i-8));
			}
		}
		txData.sprintf("%c%02X0B%02X%c", '#', Addr, val, CR);
		TxData(txData);
		WaitRx1();
	break;
	}

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void I7000DIOModule::SetPortOn(int portIdx)
{
	FWriteValue[portIdx] = true;
}

void I7000DIOModule::SetPortOff(int portIdx)
{
	FWriteValue[portIdx] = false;
}

