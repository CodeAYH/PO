//--------------------------------------------------------------------------
#include <System.AnsiStrings.hpp>

#pragma hdrstop

#include "MoonsStepMotor.h"
#include "HelperFunctions.h"
#include "SerialDevice.h"
#include "SerialPortManager.h"
#include "Chamber.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

MoonsStepMotor::MoonsStepMotor(BaseChamber *chamber, PORTINFO portInfo)
	: DamperMotor(chamber, portInfo)
{
	FHomeInitialized = false;
	FEnabled = false;
	FAlarmCode = 0;
	FCurrentPos = 0;

	FMoving = false;
	FDriveFault = false;
	FAlarmPresent = false;

	FVel = 10;
	FAccel = 100;
	FDecel = 100;
	FRevolutionCounter = 20000;
}

__fastcall MoonsStepMotor::~MoonsStepMotor()
{
	DecodeMode = DECODE_STATUS;
}

void MoonsStepMotor::DecodeMonitor()
{
	try
	{
		if( RxData.IsEmpty() )
			return;

		if( DecodeMode == DECODE_STATUS )
		{
			StatusCode = HexToInt(RxData);

			FEnabled = GetAbit(StatusCode, 0) == 1 ? true : false;
			FDriveFault = GetAbit(StatusCode, 2) == 1 ? true : false;
			FMoving = GetAbit(StatusCode, 4) == 1 ? true : false;
			FAlarmPresent = GetAbit(StatusCode, 9) == 1 ? true : false;
		}
		else
		if( DecodeMode == DECODE_CURRENTPOS )
		{
			FCurrentPos = RxData.ToInt();
		}
		else
		if( DecodeMode == DECODE_ALARM )
		{
			FAlarmCode = (unsigned long)HexToInt(RxData);
		}
	}
	catch(...)
	{
		return;
	}
}

bool MoonsStepMotor::IsRxDone()
{
	if( RxBuffer.IsEmpty() )
		return false;

	try
	{
		if( CommandMode )
		{
			AnsiString strAck;
			AnsiString strBufferedAck;
			AnsiString strNack;

			strAck.sprintf("%d%%%c", Addr, CR);
			strBufferedAck.sprintf("%d*%c", Addr, CR);
			strNack.sprintf("%d?%c", Addr, CR);
			if( AnsiContainsStr(RxBuffer, strAck) ||
				AnsiContainsStr(RxBuffer, strBufferedAck) )
			{
				ValidRxData = true;
				return true;
			}
			else
			if( AnsiContainsStr(RxBuffer, strNack) )
			{
				ValidRxData = false;
				return false;
			}
		}
		else
		{
			AnsiString str;
			int len;
			if( DecodeMode == DECODE_STATUS )
			{
				str.sprintf("%dSC=", Addr);
				if( AnsiContainsStr(RxBuffer, str) &&
					AnsiContainsStr(RxBuffer, CR) )
				{
					ValidRxData = true;
					len = str.Length();
					RxData = RxBuffer.SubString(len+1, 4);
					return true;
				}
			}
			else
			if( DecodeMode == DECODE_CURRENTPOS )
			{
				str.sprintf("%dSP=", Addr);
				if( AnsiContainsStr(RxBuffer, str) &&
					AnsiContainsStr(RxBuffer, CR) )
				{
					ValidRxData = true;
					int idx = RxBuffer.AnsiPos(CR);
					len = str.Length();
					RxData = RxBuffer.SubString(len+1, idx-len-1);
 					return true;
				}
			}
			else
			if( DecodeMode == DECODE_ALARM )
			{
				str.sprintf("%dAL=", Addr);
				if( AnsiContainsStr(RxBuffer, str) &&
					AnsiContainsStr(RxBuffer, CR) )
				{
					ValidRxData = true;
					len = str.Length();
					RxData = RxBuffer.SubString(len+1, 4);
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

void MoonsStepMotor::GetMonitorCommands(TStringList *list)
{
	AnsiString command;

	// 드라이버 상태정보 얻기
	command.sprintf("%dSC%c", Addr, CR);
	list->Add(command);

	// 현재 위치 정보 얻기
	command.sprintf("%dSP%c", Addr, CR);
	list->Add(command);

	// 알람 코드 얻기
	command.sprintf("%dAL%c", Addr, CR);
	list->Add(command);
}

void MoonsStepMotor::DoMonitor()
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

void MoonsStepMotor::DoMotorEnable(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;
	bool enable = param->Bool;

	if( enable )
		txData.sprintf("%dME%c", Addr, CR);
	else
		txData.sprintf("%dMD%c", Addr, CR);
	TxData(txData);

	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool MoonsStepMotor::MotorEnable(bool enable/*=true*/)
{
	bool ret = false;

	PARAM param;
	param.Bool = enable;

	Param = &param;
	CommandFunc = DoMotorEnable;
	ret = RetryCommand();

	DelayTime(1000);

	return ret;
}

void MoonsStepMotor::DoStop(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	txData.sprintf("%dSTD%c", Addr, CR);
	TxData(txData);

	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool MoonsStepMotor::Stop()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoStop;
	ret = RetryCommand();

	return ret;
}

void MoonsStepMotor::DoAlarmClear(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	txData.sprintf("%dAR%c", Addr, CR);
	TxData(txData);

	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool MoonsStepMotor::AlarmClear()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoAlarmClear;
	ret = RetryCommand();

	return ret;
}

void MoonsStepMotor::DoSetVelocity(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;
	int vel = param->Int;

	txData.sprintf("%dVE%d%c", Addr, vel, CR);
	TxData(txData);

	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool MoonsStepMotor::SetVelocity(int vel)
{
	bool ret = false;

	PARAM param;
	param.Int = vel;

	Param = &param;
	CommandFunc = DoSetVelocity;
	ret = RetryCommand();

	return ret;
}

void MoonsStepMotor::DoSetAccel(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;
	int accel = param->Int;

	txData.sprintf("%dAC%d%c", Addr, accel, CR);
	TxData(txData);

	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool MoonsStepMotor::SetAccel(int accel)
{
	bool ret = false;

	PARAM param;
	param.Int = accel;

	Param = &param;
	CommandFunc = DoSetAccel;
	ret = RetryCommand();

	return ret;
}

void MoonsStepMotor::DoSetDecel(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;
	int decel = param->Int;

	txData.sprintf("%dDE%d%c", Addr, decel, CR);
	TxData(txData);

	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool MoonsStepMotor::SetDecel(int decel)
{
	bool ret = false;

	PARAM param;
	param.Int = decel;

	Param = &param;
	CommandFunc = DoSetDecel;
	ret = RetryCommand();

	return ret;
}

void MoonsStepMotor::DoSetDistance(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;
	int pos = param->Int;

	txData.sprintf("%dDI%d%c", Addr, pos, CR);
	TxData(txData);

	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool MoonsStepMotor::SetDistance(int pos)
{
	bool ret = false;

	PARAM param;
	param.Int = pos;

	Param = &param;
	CommandFunc = DoSetDistance;
	ret = RetryCommand();

	return ret;
}

void MoonsStepMotor::DoFindLimitSensor(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 5000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	txData.sprintf("%dFS1L%c", Addr, CR);
	TxData(txData);

	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool MoonsStepMotor::FindLimitSensor()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoFindLimitSensor;
	ret = RetryCommand();

	return ret;
}

void MoonsStepMotor::DoGoRelativeStep(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	txData.sprintf("%dFL%c", Addr, CR);
	TxData(txData);

	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool MoonsStepMotor::GoRelativeStep()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoGoRelativeStep;
	ret = RetryCommand();

	return ret;
}

void MoonsStepMotor::DoGoAbsoluteStep(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	txData.sprintf("%dFP%c", Addr, CR);
	TxData(txData);

	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool MoonsStepMotor::GoAbsoluteStep()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoGoAbsoluteStep;
	ret = RetryCommand();

	return ret;
}

void MoonsStepMotor::DoSetEncoderPosition(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;
	int pos = param->Int;

	txData.sprintf("%dEP%d%c", Addr, pos, CR);
	TxData(txData);

	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool MoonsStepMotor::SetEncoderPosition(int pos)
{
	bool ret = false;
	PARAM param;

	param.Int = pos;
	Param = &param;
	CommandFunc = DoSetEncoderPosition;
	ret = RetryCommand();

	return ret;
}

void MoonsStepMotor::DoSetPosition(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;
	int pos = param->Int;

	txData.sprintf("%dSP%d%c", Addr, pos, CR);
	TxData(txData);

	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool MoonsStepMotor::SetPosition(int pos)
{
	bool ret = false;
	PARAM param;

	param.Int = pos;
	Param = &param;
	CommandFunc = DoSetPosition;
	ret = RetryCommand();

	return ret;
}

void MoonsStepMotor::DoDefineLimit(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;
	int limit = param->Int;

	txData.sprintf("%dDL%d%c", Addr, limit, CR);
	TxData(txData);

	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool MoonsStepMotor::DefineLimit(int limit)
{
	bool ret = false;
	PARAM param;

	param.Int = limit;
	Param = &param;
	CommandFunc = DoDefineLimit;
	ret = RetryCommand();

	return ret;
}

void MoonsStepMotor::DoSeekHome(PARAM *param)
{
	if( ComPort == nullptr )
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 2000;

	ComPort->OnRxChar = OnRxChar;

	AnsiString txData;

	txData.sprintf("%dSH4L%c", Addr, CR);
	TxData(txData);

	WaitRx1();

	ComPort->OnRxChar = nullptr;
	TimeoutTimer->Interval = oldTimeout;
}

bool MoonsStepMotor::SeekHome()
{
	bool ret = false;

	Param = nullptr;
	CommandFunc = DoSeekHome;
	ret = RetryCommand();

	return ret;
}

bool MoonsStepMotor::SearchHome()
{
	if( Stop() == false )
		return false;

	if( DefineLimit(1) == false )
		return false;

	if( SetDistance(FRevolutionCounter) == false )
		return false;

	if( SetVelocity(FVel/4) == false )
		return false;

	if( SetAccel(FAccel/4) == false )
		return false;

	if( SetDecel(FDecel/4) == false )
		return false;

//	if( FindLimitSensor() == false )
//		return false;

	if( SeekHome() == false )
		return false;

	if( SetPosition(0) == false )
		return false;

	DelayTime(2000, true);
	FHomeInitialized = true;

	return true;
}

bool MoonsStepMotor::MoveStepRelative(int pos)
{
	if( Stop() == false )
		return false;

	if( SetDistance(pos) == false )
		return false;

	if( SetVelocity(FVel) == false )
		return false;

	if( SetAccel(FAccel) == false )
		return false;

	if( SetDecel(FDecel) == false )
		return false;

	if( GoRelativeStep() == false )
		return false;

	return true;
}

bool MoonsStepMotor::MoveStepAbsolute(int pos)
{
	if( CurrentPos == pos )
		return true;

	if( Stop() == false )
		return false;

	if( SetDistance(pos) == false )
		return false;

	if( SetVelocity(FVel) == false )
		return false;

	if( SetAccel(FAccel) == false )
		return false;

	if( SetDecel(FDecel) == false )
		return false;

	if( GoAbsoluteStep() == false )
		return false;

	return true;
}

bool MoonsStepMotor::Open(int percent)
{
	int step = FRevolutionCounter / 4;
	step = step * percent;
	step = step / 100;
	return MoveStepAbsolute(step);
}

bool MoonsStepMotor::Close()
{
	return MoveStepAbsolute(0);
}

