//--------------------------------------------------------------------------
#include <System.AnsiStrings.hpp>

#pragma hdrstop

#include "I7018.h"
#include "HelperFunctions.h"
#include "SerialPortManager.h"
#include "Chamber.h"
#include "NormalOvenSystemManager.h"
#include "NormalOvenChamber.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

I7018::I7018(BaseChamber *chamber, PORTINFO portInfo)
	: SerialDevice(chamber, portInfo)
{
	DecodeMode = READPV;
	FDevName = "I7018";
}

I7018::~I7018()
{

}

void I7018::DecodeMonitor()
{
	if( DecodeMode == READPV )
	{
		if( RxData.Length()!=56 )
			return;

		double pv[MAX_POINT_COUNT];

		// 0���� �ʱ�ȭ
		for(int i=0; i<MAX_POINT_COUNT; i++)
		{
			pv[i] = 0;
		}

		int startIdx = 1;
		const int len = 7;
		AnsiString str;

		try
		{
			if ( MyChamber && GetManager() && GetManager()->UseLoggerDataSave )		// 2021-03-16 ADD Save Serial RxData
			{
				String ChamberID;
				ChamberID.sprintf(L"%c", ((OvenChamber*)MyChamber)->ChamberID + 'A');
				AnsiString FileName = DevName + "_" + (AnsiString)ChamberID + "_" + Addr;
				SaveSerialLog(FileName, RxData, false);
			}
			for(int i=0; i<MAX_POINT_COUNT; i++)
			{
				str = RxData.SubString(startIdx, len);
				if( IsValidFloat(str) )
					pv[i] = StrToFloat(str);
				startIdx += len;
			}
		}
		catch(...)
		{
			return;
		}


		if( MyChamber )
		{
			for(int i=0; i<MAX_POINT_COUNT; i++)
			{
				((OvenChamber*)MyChamber)->LoggerData[i + MAX_POINT_COUNT * FLoggerIndex] = pv[i];
			}
		}
	}
	else
	if( DecodeMode == READCJC )
	{
    	if( RxData.Length()!=7 )
			return;

		FCJC = 0;

		try
		{
			if( IsValidFloat(RxData) )
			{
				FCJC = StrToFloat(RxData);
			}
		}
		catch(...)
		{
			return;
		}
	}
	// 20181218 - Decode Monitoring Logger Offset Value - mtcho
	else
	if( DecodeMode == READOFFSET )
	{
		// Response: !01+01F4 -> 01+01F4(Length = 7)
		if( RxData.Length() != 7 )
			return;

		String strTemp;

		try
		{
			// RxData : 01+01F4 -> 01F4
			strTemp = RxData.SubString(4,RxData.Length());
			if( IsValidHex(strTemp) )
			{
				// Logger Offset Value : 01F4 -> 500 -> 5
				FLoggerOffset = HexToInt(strTemp) / 100;
			}
		}
		catch(...)
		{
			return;
		}
	}
}

bool I7018::IsRxDone()
{
	if( RxBuffer.IsEmpty() )
		return false;

	AnsiString strEtx;
	try
	{
		strEtx.sprintf("%c", CR);
		if( AnsiContainsStr(RxBuffer, RxStx) &&
			AnsiContainsStr(RxBuffer, strEtx) )
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

void I7018::GetMonitorCommands(TStringList *list)
{
	AnsiString command;

	// �µ��� �б�
	command.sprintf("%c%02d%c", '#', Addr, CR);
	list->Add(command);

	// CJC �� �б�
	command.sprintf("%c%02X3%c", '$', Addr, CR);
	list->Add(command);

    // 20181218 - CJC �϶� Logger Offset �� �б� - mtcho
    command.sprintf("%c%02X9%c", '$', Addr, CR);
	list->Add(command);
}

void I7018::DoMonitor()
{
	if( ComPort == nullptr )
		return;

	ComPort->OnRxChar = OnRxChar;

	TStringList *list = new TStringList;
	AnsiString txData;

	GetMonitorCommands(list);
	RxStx = '>';
	for(int i=0; i<list->Count; i++)
	{
		// 20181219 - DVF func Add - jhlim
		// DecodeMode = (i == 0) ? READPV : READCJC;
		if (i == 0)
		{
			DecodeMode = READPV;
		}
		else
		if (i == 1)
		{
			DecodeMode = READCJC;
		}
		else
		if (i == 2)
		{
			// Read CJC Offset Value : !AA9
			RxStx = '!';
			DecodeMode = READOFFSET;
		}

		txData = list->Strings[i];

		if (i == 0)
		{
			if ( MyChamber && GetManager() && GetManager()->UseLoggerDataSave )		// 2021-03-16 ADD Save Serial RxData
			{
				String ChamberID;
				ChamberID.sprintf(L"%c", ((OvenChamber*)MyChamber)->ChamberID + 'A');
				AnsiString FileName = DevName + "_" + (AnsiString)ChamberID + "_" + Addr;
				SaveSerialLog(FileName, txData, true);
			}
		}

		TxData(txData);
		if( WaitRx1() )
			DecodeMonitor();
	}
	delete list;

	ComPort->OnRxChar = nullptr;
}

void I7018::DoWriteCJCSetting(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	RxStx = '!';

	// CJC ��뿩�� ����
	// Format : ~AAC1
	// ~  : ������
	// AA : �ּ�
	// C  : ��ɾ�
	// 1  : �����
	txData.sprintf("%c%02dC1%c", '~', Addr, CR);
	TxData(txData);

	// ���� �����̸� '!', ���и� '?'�� �����ϴ� �����͸� �����Ѵ�.
	// ���� WaitRx �Լ��� true�� �����ϸ� ������ ������ ���̴�.
	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void I7018::DoWriteChannelSetting(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	RxStx = '!';

	// ä�� ��뿩�� ����
	// Format : $AA5FF
	// $  : ������
	// AA : �ּ�
	// 5  : ��ɾ�
	// FF : �� ��Ʈ�� ä���� �ǹ��ϰ� 1�̸� ���, 0�̸� ������� ����
	txData.sprintf("%c%02d5FF%c", '$', Addr, CR);
	TxData(txData);

	// ���� �����̸� '!', ���и� '?'�� �����ϴ� �����͸� �����Ѵ�.
	// ���� WaitRx �Լ��� true�� �����ϸ� ������ ������ ���̴�.
	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

void I7018::DoWriteCommSetting(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	RxStx = '!';

	// �ּ�, ����Ÿ��, ������ ���� ����
	// Format : %AANNTTCCFF
	// %  : ������
	// AA : �ּ�
	// NN : �� �ּ�
	// TT : ����Ÿ��
	// CC : ��� �ӵ�
	// FF : ������ ����
	// �ӵ� 19200, crc ������� ����, K-type ���� ���
	txData.sprintf("%c01%02d0F0700%c", '%', 1, CR);
	TxData(txData);

	// ���� �����̸� '!', ���и� '?'�� �����ϴ� �����͸� �����Ѵ�.
	// ���� WaitRx �Լ��� true�� �����ϸ� ������ ������ ���̴�.
	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool I7018::WriteCJCSetting()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoWriteCJCSetting;
	ret = RetryCommand();

	return ret;
}

bool I7018::WriteChannelSetting()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoWriteChannelSetting;
	ret = RetryCommand();

	return ret;
}

bool I7018::WriteCommSetting()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoWriteCommSetting;
	ret = RetryCommand();

	return ret;
}

// 20181218 - Logger Offset Setting - mtcho
bool I7018::WriteOffsetSetting(int LoggerOffset)
{
	bool ret = false;
	PARAM param;

	param.Int = LoggerOffset;

	Param = &param;
	CommandFunc = DoWriteOffsetSetting;
	ret = RetryCommand();

	return ret;
}

void I7018::DoWriteOffsetSetting(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;
    char cSign;
    int nOffset;

	RxStx = '!';

    // 1. Scale factor ( Hex 0010 = 0.16 degree Celsius )
    nOffset = param->Int * 100;

    // 2. sign ��������
    if(nOffset >= 0)
        cSign = '+';
    else
        cSign = '-';

    // 3. Absolute value
    nOffset = abs(nOffset);

    // �ּ�, ����Ÿ��, ������ ���� ����
	// Format : $AA9SNNNN
	// $  : ������
	// AA : �ּ�
    // S  : Sign byte, "+" or "-", of the offset value
	// NNNN : 4�ڸ� 16������ ������ ���밪(NNNN <= 1000h, 0001 = 0.01C)
	txData.sprintf("%c%02d9%c%04X%c", '$', Addr, cSign, nOffset, CR);
	TxData(txData);

  	// ���� �����̸� '!', ���и� '?'�� �����ϴ� �����͸� �����Ѵ�.
	// ���� WaitRx �Լ��� true�� �����ϸ� ������ ������ ���̴�.
	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}
