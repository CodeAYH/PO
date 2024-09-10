//---------------------------------------------------------------------------
#include <System.AnsiStrings.hpp>

#pragma hdrstop

#include "VSOvenBoard.h"
#include "HelperFunctions.h"
#include "SerialPortManager.h"
#include "Chamber.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

VSOvenBoard::VSOvenBoard(BaseChamber *chamber, PORTINFO portInfo)
	: SerialIODevice(chamber, portInfo)
{
	FDevName = "OVEN DIO";

	DIValue = 0;
//	DOValue = 0;
//	DOWrite = 0;
//	DOWriteCheck = false;

	ExpectedRcvLength = 0;
	CurrentRcvLength = 0;
}

__fastcall VSOvenBoard::~VSOvenBoard()
{
}

bool VSOvenBoard::TxBinData(unsigned char *data, int len)
{
	if( ComPort && IsOpened )
	{
		InitRxValues();

		Timeout = false;
		TimeoutTimer->Enabled = true;

		ComPort->ReadBufFree();
		ComPort->WriteBufFree();

		memset(RxData, 0x00, BUF_LEN);
		ComPort->WriteBuf(data, len);

	  	String str, tmp1, tmp2;
		str = L"Oven Board : Addr = ";
		str += IntToStr(Addr);
		str += " Tx = 0x";
		for(int i=0; i<len; i++)
		{
			tmp1.sprintf(L"%02X", data[i]);
			tmp2 += tmp1;
		}
		str += tmp2;
   //		SaveDebugLog(str);    hcw test.
		return true;
	}

	return false;
}

#define POLYNORMIAL 0xA001  // 0xA001   checksum : 71 A7
unsigned short VSOvenBoard::CRC16(unsigned char *puchMsg, int usDataLen)
{
	int i;
	unsigned short crc, flag;
	crc = 0xffff;
	while (usDataLen--)
	{
		crc ^= *puchMsg++;
		for (i=0; i<8; i++)
		{
			flag = crc & 0x0001;
			crc >>= 1;
			if (flag)
				crc ^= POLYNORMIAL;
		}
	}
	return crc;
}

void VSOvenBoard::DoCommunicate()
{
	DoMonitor();
	WaitForSingleObject((void *)Handle, 20);

	if (FReadValue != FWriteValue)
	{
		DoWriteOutput();
		WaitForSingleObject((void *)Handle, 20);
	}
}

bool VSOvenBoard::IsRxDone()
{
	unsigned char addr = RxData[0];
	unsigned char funcCode = RxData[1];
	unsigned short calcCrc = 0, rcvdCrc = 0, temp = 0;
	int crcStartIdx = 0;

	if (addr != Addr)
	{
		return false;
	}

	if (funcCode == 1)
	{
		// 바이트 수가 12가 아니면 오류
		if (RxData[2] != 12)
		{
			return false;
		}
		crcStartIdx = 15;
	}
	else if (funcCode == 15)
	{
		const int len = 6;
		unsigned char data[len];
		data[0] = Addr;
		data[1] = 15;							// Function Code = 15
		data[2] = 0;                            // 스타트 어드레스 상위
		data[3] = 0;							// 스타트 어드레스 하위
		data[4] = 0;                            // 엔드 어드레스 상위
		data[5] = 31;							// 엔드 어드레스 하위
		for (int i = 0; i < len; i++)
		{
			if (RxData[i] != data[i])
			{
				return false;
			}
		}
		crcStartIdx = 6;
	}
	else
	{
		return false;
	}

	calcCrc = CRC16(RxData, crcStartIdx);
	rcvdCrc = (unsigned short)RxData[crcStartIdx];
	temp = (unsigned short)RxData[crcStartIdx + 1];
	temp = temp << 8;
	rcvdCrc = rcvdCrc | temp;

	if (calcCrc != rcvdCrc)
	{
		return false;
	}

	ValidRxData = true;
	return true;
}

void VSOvenBoard::DecodeMonitor()
{
	unsigned char funcCode = RxData[1];
	if (funcCode == 1)	// DIO 읽기
	{
		unsigned int read = 0, tmp = 0;

		// Output
		tmp = (unsigned int)RxData[3];
		read = (tmp << 0) & 0x000000FF;
		tmp = (unsigned int)RxData[4];
		tmp = (tmp << 8) & 0x0000FF00;
		read = tmp | read;
		tmp = (unsigned int)RxData[5];
		tmp = (tmp << 16) & 0x00FF0000;
		read = tmp | read;
		tmp = (unsigned int)RxData[6];
		tmp = (tmp << 24) & 0xFF000000;
		read = tmp | read;
		//DOValue = read;
		FReadValue = read;
		if (IsFirstRun)
		{
			FWriteValue = FReadValue;
			IsFirstRun = false;
		}

		// Input
		tmp = (unsigned int)RxData[9];
		read = (tmp << 0) & 0x000000FF;
		tmp = (unsigned int)RxData[10];
		tmp = (tmp << 8) & 0x0000FF00;
		read = tmp | read;
		tmp = (unsigned int)RxData[11];
		tmp = (tmp << 16) & 0x00FF0000;
		read = tmp | read;
		tmp = (unsigned int)RxData[12];
		tmp = (tmp << 24) & 0xFF000000;
		read = tmp | read;
		DIValue = read;
	}
}

bool VSOvenBoard::GetReadValue(int idx)
{
	unsigned int mask = 0, tmp = 0;
	bool ret = false;

	if (idx >=0 && idx < 32) 	// Input
	{
		mask = 0x01 << idx;
		tmp = DIValue & mask;
		ret = (tmp == mask);
	}
	else if (idx >= 32 && idx < 64)	// Output
	{
		unsigned int doValue = (unsigned int)FReadValue.to_ulong();
		mask = 0x01 << (idx - 32);
		tmp = doValue & mask;
		ret = (tmp == mask);
	}

	return ret;
}

void __fastcall VSOvenBoard::OnRxChar(TObject *Sender, int Count)
{
	ComPort->ReadBuf(RxData + CurrentRcvLength, Count);
	CurrentRcvLength += Count;

  /*	String str, tmp1, tmp2;
	str = L"Oven Board : Addr = ";
	str += IntToStr(Addr);
	str += " Rx = 0x";
	for(int i=0; i<CurrentRcvLength; i++)
	{
		tmp1.sprintf(L"%02X", RxData[i]);
		tmp2 += tmp1;
	}
	str += tmp2;
	SaveDebugLog(str);   */

 	if (IsRxDone() == false || CurrentRcvLength != ExpectedRcvLength)
	{
		return;
	}

	TimeoutTimer->Enabled = false;
}

void VSOvenBoard::DoMonitor(void)
{
	if( ComPort == nullptr )
		return;

	const int txLength = 8;
	unsigned char data[txLength] = { 0, };
	unsigned short crc16, tmp;

	ComPort->OnRxChar = OnRxChar;

	data[0] = (unsigned char) Addr;	// MCU Board Address
	data[1] = 1;							// Function Code = 1
	data[2] = 0;
	data[3] = 0;							// 읽기 시작주소 = 0
	data[4] = 0;
	data[5] = 96;							// 읽을 IO 수

	crc16 = CRC16(data, txLength - 2);
	tmp = ((crc16>>0) & 0x00FF);
	data[6] = (unsigned char)tmp;
	tmp = ((crc16>>8) & 0x00FF);
	data[7] = (unsigned char)tmp;

	ExpectedRcvLength = 17;
	CurrentRcvLength = 0;
	TxBinData(data, txLength);

	if (WaitRx1())
		DecodeMonitor();

	ComPort->OnRxChar = nullptr;
}

void VSOvenBoard::DoWriteOutput()
{
	const int txLength = 13;
	unsigned char data[txLength] = { 0, };
	unsigned short crc16, tmp;
	unsigned int doData;

	ComPort->OnRxChar = OnRxChar;

	data[0] = Addr;	// MCU Board Address
	data[1] = 15;							// Function Code = 15
	data[2] = 0;                            // 스타트 어드레스 상위
	data[3] = 0;							// 스타트 어드레스 하위
	data[4] = 0;                            // 엔드 어드레스 상위
	data[5] = 31;							// 엔드 어드레스 하위
	data[6] = 4;							// 바이트 수

	doData = (unsigned int)FWriteValue.to_ulong();
	data[7] = (unsigned char)((doData >> 0) & 0x000000FF);
	data[8] = (unsigned char)((doData >> 8) & 0x000000FF);
	data[9] = (unsigned char)((doData >> 16) & 0x000000FF);
	data[10] = (unsigned char)((doData >> 24) & 0x000000FF);

	crc16 = CRC16(data, txLength - 2);
	tmp = ((crc16>>0) & 0x00FF);
	data[11] = (unsigned char)tmp;
	tmp = ((crc16>>8) & 0x00FF);
	data[12] = (unsigned char)tmp;

	ExpectedRcvLength = 8;
	CurrentRcvLength = 0;
	//DOWriteCheck = false;
	TxBinData(data, txLength);
	WaitRx1();

//	DoMonitor();
//	if (DOValue == DOWrite)
//	{
//		DOWriteCheck = true;
//	}

	ComPort->OnRxChar = nullptr;
}

void VSOvenBoard::SetPortOn(int portIdx)
{
	FWriteValue[portIdx] = true;
}

void VSOvenBoard::SetPortOff(int portIdx)
{
	FWriteValue[portIdx] = false;
}
