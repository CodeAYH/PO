//---------------------------------------------------------------------------
#include <System.DateUtils.hpp>
#include <System.AnsiStrings.hpp>
#include <stdio.h>

#pragma hdrstop

#include "SerialDevice.h"
#include "HelperFunctions.h"
#include "SerialPortManager.h"
#include "Chamber.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

SerialDevice::SerialDevice(BaseChamber *chamber, PORTINFO portInfo, bool createPort)
	: TThread(false), MyChamber(chamber)
{
	MyIndex = -1;
	if (createPort)
	{
		ComPort = GetSerialManager()->GetPort(portInfo.portNo, portInfo.portSetting);
		if (ComPort == nullptr || ComPort->IsOpened == false)
		{
			FCommFail = true;
		}
		else
		{
			FCommFail = false;
		}
		MyIndex = ComPort->RegisteredDeviceCount;
		ComPort->AddDevice();
	}
	Addr = portInfo.addr;

	RxBuffer = "";
	RxData = "";
	FDevName = "Unknown";

	ValidRxData = false;
	Timeout = false;
	CommandMode = false;
	CommandDone = true;
	WaitCommandResult = false;

	RetryCount = MAX_RETRY_COUNT;
	CommFailCount = 0;
	FFailCount = 0;
	FSuccessCount = 0;
	MonitorCount = 0;

	FScaleFactor = 1.0;

	CommandFunc = nullptr;
	TimeoutTimer = nullptr;
	Param = nullptr;

	try
	{
		TimeoutTimer = new TTimer(nullptr);
		TimeoutTimer->Enabled = false;
		TimeoutTimer->Interval = portInfo.timeout;

		TimeoutTimer->OnTimer = OnTimeout;
	}
	catch (...)
	{
		TimeoutTimer = nullptr;
	}

	if( ComPort )
		ComPort->OnRxChar = OnRxChar;
}

__fastcall SerialDevice::~SerialDevice()
{
	if( TimeoutTimer )
	{
		TimeoutTimer->Enabled = false;
		Terminate();
		Sleep(300);
		delete TimeoutTimer;
	}
}

bool SerialDevice::GetIsOpened()
{
	if( ComPort == nullptr )
		return false;

	return ComPort->IsOpened;
}

void __fastcall SerialDevice::Execute()
{
	while( !Terminated )
	{
		WaitForSingleObject((void *)Handle, THREAD_WAIT_TIME);

		if( ComPort &&
			ComPort->IsOpened )
		{
			ComPort->Lock();
			if (ComPort->RegisteredDeviceCount > 0 && MyIndex == ComPort->CurrentDeviceIndex)
			{
				DoCommunicate();
				ComPort->IncCurrentDeviceIndex();
			}
			ComPort->Unlock();
		}
		else
			FCommFail = true;
	}
}

void SerialDevice::InitRxValues()
{
	RxBuffer = "";
	RxData = "";
	ValidRxData = false;
}

bool SerialDevice::TxData(const AnsiString data)
{
	if( ComPort && IsOpened )
	{
		InitRxValues();

		Timeout = false;
		TimeoutTimer->Enabled = true;
		TxBuffer = data;
		ComPort->WriteText(data);

		return true;
	}

	return false;
}

// 데이터 수신 실패
void __fastcall SerialDevice::OnTimeout(TObject *Sender)
{
	TimeoutTimer->Enabled = false;
	Timeout = true;
}

void __fastcall SerialDevice::OnRxChar(TObject *Sender, int Count)
{
	AnsiString rcvData;

	rcvData = ComPort->ReadText();

	RxBuffer += rcvData;

	if( !IsRxDone() )
		return;

	TimeoutTimer->Enabled = false;
}

void SerialDevice::DoMonitor()
{
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

void SerialDevice::DoCommunicate()
{
	if( CommandMode == true )
	{
		CommandFunc(Param);
		CommandMode = false;
	}
	else
	{
		DoMonitor();
    }
}

bool SerialDevice::WaitRx1()
{
	while( 1 )
	{
		WaitForSingleObject((void *)Handle, THREAD_WAIT_TIME);

		if( ValidRxData )
		{
			CommFailCount = 0;
			FCommFail = false;
			INC(FSuccessCount);
			break;
		}
		else
		if( Timeout )
		{
			CommFailCount++;
			INC(FFailCount);
			if( CommFailCount > MAX_RETRY_COUNT )
			{
				CommFailCount = 0;
				FCommFail = true;
			}
			return false;
		}
	}

	return true;
}

bool SerialDevice::WaitRx2()
{
	bool ret = false;

	while( 1 )
	{
		// 이 함수가 수행되는 곳은 메인 쓰레드 즉, GUI 쓰레드이다.
		// 아래 코드는 GUI가 멈추지 않도록 한다.
		Application->ProcessMessages();
		Sleep(THREAD_WAIT_TIME);

		// 커맨드 함수가 실행되길 기다린다.
		if( CommandMode == true )
			continue;

		if( ValidRxData )
		{
			ret = true;
			break;
		}
		else
		if( Timeout )
		{
			ret = false;
			break;
		}
	}

	CommandDone = true;
	return ret;
}

bool SerialDevice::RetryCommand()
{
	if( ComPort == nullptr || IsOpened == false )
		return false;

	for(int i=0; i<RetryCount; i++)
	{
		CommandMode = true;

		Application->ProcessMessages();
		Sleep(THREAD_WAIT_TIME);

		if( WaitRx2() )
		{
			return true;
		}
	}
	return false;
}

void SerialDevice::DelayTime(unsigned int milliSeconds, bool workInThread/*=true*/)
{
	TDateTime now = Now();
	while( 1 )
	{
		if( workInThread )
			WaitForSingleObject((void*)Handle, THREAD_WAIT_TIME);
		else
		{
			Application->ProcessMessages();
			Sleep(THREAD_WAIT_TIME);
		}

		if( MilliSecondsBetween(now, Now()) >= milliSeconds )
			break;
	}
}

SerialIODevice::SerialIODevice(BaseChamber *chamber, PORTINFO portInfo, bool createPort)
	: SerialDevice(chamber, portInfo, createPort)
{
	FReadValue.reset();
	FWriteValue.reset();
	IsFirstRun = true;
}

__fastcall SerialIODevice::~SerialIODevice()
{

}
