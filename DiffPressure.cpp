//---------------------------------------------------------------------------
#include <System.AnsiStrings.hpp>

#pragma hdrstop

#include "DiffPressure.h"
#include "HelperFunctions.h"
#include "SerialPortManager.h"
#include "Chamber.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

DiffPressure::DiffPressure(BaseChamber *chamber, PORTINFO portInfo)
	: SerialDevice(chamber, portInfo)
{
	FDevName = "DIFFPRESSURE";
	SkipCount = 0;
}

__fastcall DiffPressure::~DiffPressure()
{

}

bool DiffPressure::TxBinData(void *buf, int len)
{
	if( ComPort == nullptr )
		return false;

	for(int i=0; i<7; i++)
		RxDPData[i] = 0;
//	ZeroMemory(RxDPData, 7);
	ValidRxData = false;
	Timeout = false;
	TimeoutTimer->Enabled = true;
	ComPort->WriteBuf(buf, len);

	return true;
}

void DiffPressure::DoMonitor()
{
	if( ComPort == nullptr )
		return;

	SkipCount++;
	if( SkipCount == 20 )
		SkipCount = 0;
	if( SkipCount != 0 )
		return;

	ComPort->OnRxChar = OnRxChar;

	char command[8];

	command[0] = (char)Addr;
	command[1] = (char)0x3;
	command[2] = (char)0x0;
	command[3] = (char)0x0;
	command[4] = (char)0x0;
	command[5] = (char)0x1;
	command[6] = 0;
	command[7] = 0;

	TxBinData(command, 8);

	if( WaitRx1() )
		DecodeMonitor();

	ComPort->OnRxChar = nullptr;
}

bool DiffPressure::IsRxDone()
{
	int rAddr = RxDPData[0];
	int rFunc = RxDPData[1];
	int rCount = RxDPData[2];
	int rData;

	if( rAddr == Addr &&
		rFunc == 3 &&
		rCount == 2 )
	{
		ValidRxData = true;
		return true;
	}

	return false;
}

void DiffPressure::DecodeMonitor()
{
	int rData = 0;

	rData = 0xFF & RxDPData[3];
	rData = rData << 8;
	rData = rData | (0xFF & RxDPData[4]);
	if( (0x8000 & RxDPData[3]) == 0x8000 )
		rData |= 0xFFFF0000;

	FPV = (double)rData / 10.0;
}

void __fastcall DiffPressure::OnRxChar(TObject *Sender, int Count)
{
	if( Count != 7 )
		return;

	ComPort->ReadBuf(RxDPData, Count);

	if( !IsRxDone() )
		return;

	TimeoutTimer->Enabled = false;
}
