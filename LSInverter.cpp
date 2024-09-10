//---------------------------------------------------------------------------
#include <System.SysUtils.hpp>
#include <memory>
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <map>
#include <System.AnsiStrings.hpp>

#pragma hdrstop

#include "LSInverter.h"
#include "HelperFunctions.h"
#include "SerialPortManager.h"
#include "Chamber.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

LSInverter::LSInverter(BaseChamber *chamber, PORTINFO portInfo)
	: SerialDevice(chamber, portInfo)
{
	FDevName = "008IG5A-2";
}

__fastcall LSInverter::~LSInverter()
{

}
//---------------------------------------------------------------------------
void LSInverter::DoCommunicate()
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
//---------------------------------------------------------------------------
void LSInverter::DoMonitor()
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
//---------------------------------------------------------------------------
bool LSInverter::IsRxDone()
{
	if( RxBuffer.IsEmpty() )
		return false;

//	"A" ENQ(1) 국번(2) CMD(1) 		  			  data(n*4) sum(2) EOT(1)
//	"N" ENQ(1) 국번(2) CMD(1)         에러코드(2) data(n*4) sum(2) EOT(1)

	try
	{
		AnsiString strEOT, strACK, strNAK;

		strACK.sprintf("%c", ACK);
		strNAK.sprintf("%c", NAK);
		strEOT.sprintf("%c", EOT);

		if ((AnsiContainsStr(RxBuffer, strACK) || AnsiContainsStr(RxBuffer, strNAK)) &&
			AnsiContainsStr(RxBuffer, strEOT))
		{
			int ackIdx;
			int eotIdx = RxBuffer.AnsiPos(strEOT);
			int len = RxBuffer.Length();

			if (AnsiContainsStr(RxBuffer, strACK))
			{
				ackIdx = RxBuffer.AnsiPos(strACK);
			}
			else if (AnsiContainsStr(RxBuffer, strNAK))
			{
				ackIdx = RxBuffer.AnsiPos(strNAK);
			}

			if (ackIdx == 1 && eotIdx == len)
			{
				RxData = RxBuffer.SubString(ackIdx, eotIdx-1);

				AnsiString str = RxData.SubString(2, RxData.Length()-3);
				AnsiString strChksum = MakeCheckSum(str);
				AnsiString rcvdChksum = RxBuffer.SubString(eotIdx-2, 2);
				if (strChksum == rcvdChksum)
				{
					ValidRxData = true;
					return true;
				}
			}
		}
	}
	catch(...)
	{
		return false;
	}
	return false;
}
//---------------------------------------------------------------------------
void LSInverter::DecodeMonitor()
{
	LSInverterDATA data;

	try
	{
		AnsiString strAddr = RxData.SubString(2, 2);
		if (IsValidHex(strAddr) == false)
		{
			return;
		}
		int addr = strAddr.ToInt();
		if (addr != Addr)
		{
			return;
		}

		if (RxData[1] == ACK)
		{
			AnsiString strValue = RxData.SubString(5, 4);
			int value;
			if (IsValidHex(strValue) == false)
			{
				return;
			}
			value = HexToInt(strValue);
			//value = strValue.ToInt();

			switch (DecodeMode)
			{
			case READ_PARAM_SET_ENABLE:
				ReceivedData.ParamSetEnable = (value == 1) ? true : false;
				break;
			case READ_REQUENCY:
				ReceivedData.Frequency = ((double)value) / 100.;
				break;
			case READ_DRV_COMMAND:
				ReceivedData.DrvCommand = value;
				break;
			case READ_INVERTER_STATUS:
				ReceivedData.InverterStatus = value;
				break;
	/*		case READ_TRIP_A:
				ReceivedData.TripA = value;
				break;
			case READ_TRIP_B:
				ReceivedData.TripB;
				break;*/
			}
		}
		else if (RxData[1] == NAK)
		{
			ReceivedData.ErrorCode = RxData.SubString(5, 2);
		}
	}
	catch(...)
	{
		return;
	}

}
//---------------------------------------------------------------------------
void LSInverter::GetMonitorCommands(TStringList *list)
{
	AnsiString command;

	command = MakeReadString(ADDR_PARAM_SET_ENABLE);
	list->Add(command);

	command = MakeReadString(ADDR_SET_FREQUENCY);
	list->Add(command);

	command = MakeReadString(ADDR_DRV_COMMAND);
	list->Add(command);

	command = MakeReadString(ADDR_INVERTER_STATUS);
	list->Add(command);

/*	command = MakeReadString(ADDR_TRIP_INFO_A);
	list->Add(command);

	command = MakeReadString(ADDR_TRIP_INFO_B);
	list->Add(command); */
}
//---------------------------------------------------------------------------
int LSInverter::CheckSum(AnsiString strData)
{
	int len, i, sum;
	char c;

	len = strData.Length();

	sum = 0;
	for(i=1; i<=len; i++)
	{
		c = strData[i];
		sum += c;
	}
	sum = sum & 0xff;

	return sum;
}
//---------------------------------------------------------------------------
AnsiString LSInverter::MakeCheckSum(AnsiString strData)
{
	int sum;
	AnsiString chksum, tmp;

	sum = CheckSum(strData);

	tmp.sprintf("%02X", sum);
	chksum = tmp.SubString(tmp.Length()-1, 2);
	return chksum;
}
//---------------------------------------------------------------------------
AnsiString LSInverter::MakeWriteString(int regAddr, AnsiString value)
{
	AnsiString str, chksum, command;

	//	ENQ(1) 국번(2) CMD(1) 번지(4) 번지개수(1) data(n*4) sum(2) EOT(1)
	str.sprintf("%02XW%04X1%s", Addr, regAddr, value.c_str());
	chksum = MakeCheckSum(str);
	command.sprintf("%c%s%s%c", ENQ, str.c_str(), chksum.c_str(), EOT);

	return command;
}
//---------------------------------------------------------------------------
AnsiString LSInverter::MakeReadString(int regAddr)
{
	AnsiString str, chksum, command;

	// ENQ(1) 국번(2) CMD(R) 번지(4) 번지개수(1) sum(2) EOT(1)
	str.sprintf("%02XR%04X1", Addr, regAddr);
	chksum = MakeCheckSum(str);
	command.sprintf("%c%s%s%c", ENQ, str.c_str(), chksum.c_str(), EOT);

	return command;
}
//---------------------------------------------------------------------------
bool LSInverter::SetMotorRunForward()
{
	bool ret = false;

	PARAM param;
	param.Int = FORWARD_DIR;

	Param = &param;
	CommandFunc = DoSetDrvCommnad;
	ret = RetryCommand();

	return ret;
}
//---------------------------------------------------------------------------
bool LSInverter::SetMotorRunReverse()
{
	bool ret = false;

	PARAM param;
	param.Int = REVERSE_DIR;

	Param = &param;
	CommandFunc = DoSetDrvCommnad;
	ret = RetryCommand();

	return ret;
}
//---------------------------------------------------------------------------
void LSInverter::DoSetDrvCommnad(PARAM *param)
{
	if( ComPort == nullptr)
	{
		return;
	}
	else if(ComPort->IsOpened == false)
	{
		return;
	}

	AnsiString txData;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;
	ComPort->OnRxChar = OnRxChar;

	txData = MakeWriteString(ADDR_DRV_COMMAND, param->Int);
	TxData(txData);
	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}
//---------------------------------------------------------------------------
bool LSInverter::SetMotorStop()
{
	bool ret = false;

	PARAM param;
	param.Int = INVERTER_STOP;

	Param = &param;
	CommandFunc = DoSetDrvCommnad;
	ret = RetryCommand();

	return ret;
}
//---------------------------------------------------------------------------
bool LSInverter::SetParameterChangePermit()
{
	bool ret = false;

	PARAM param;
	param.Bool = true;

	Param = &param;
	CommandFunc = DoSetParameterChangePermit;
	ret = RetryCommand();

	return ret;
}
//---------------------------------------------------------------------------
void LSInverter::DoSetParameterChangePermit(PARAM* param)
{
	if( ComPort == nullptr)
	{
		return;
	}
	else if(ComPort->IsOpened == false)
	{
		return;
	}

	AnsiString txData, value;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	if(	param->Bool )
		value = "0001";
	else
		value = "0000";
	txData = MakeWriteString(ADDR_PARAM_SET_ENABLE, value);

	TxData(txData);
	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}
//---------------------------------------------------------------------------
bool LSInverter::SetRunFreq(double freq)
{
	bool ret = false;

	PARAM param;
	param.Double = freq;

	Param = &param;
	CommandFunc = DoSetRunFreq;
	ret = RetryCommand();

	return ret;
}
//---------------------------------------------------------------------------
void LSInverter::DoSetRunFreq(PARAM *param)
{
	if( ComPort == nullptr)
	{
		return;
	}
	else if(ComPort->IsOpened == false)
	{
		return;
	}

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;
	ComPort->OnRxChar = OnRxChar;

	AnsiString txData, value;
	double freq1 = param->Double;
	int freq2 = (int)(freq1 * 100);	// Scale => 0.01 이라서 Hz 입력값에 100 곱함.
	value.sprintf("%04X", freq2);
	txData = MakeWriteString(ADDR_SET_FREQUENCY, value);

	TxData(txData);
	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}
//---------------------------------------------------------------------------
int LSInverter::GetMotorRunDir()
{
	unsigned int tmp = ReceivedData.DrvCommand & 0x0007;
	return tmp;
}

double LSInverter::GetFrequency()
{
	return ReceivedData.Frequency;
}

bool LSInverter::IsTrip()
{
	unsigned int tmp = ReceivedData.InverterStatus & 0x0008;
	return (tmp == 0) ? false : true;
}
