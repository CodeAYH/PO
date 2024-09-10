//--------------------------------------------------------------------------
#include <System.AnsiStrings.hpp>
#include "FAS_EziMOTIONPlusR.h"

#pragma hdrstop

#include "EziStepMotor.h"
#include "HelperFunctions.h"
#include "SerialDevice.h"
#include "SerialPortManager.h"
#include "Chamber.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

int EziStepMotor::PortNo = -1;
bool EziStepMotor::IsConnected = false;
std::shared_ptr<TCriticalSection> EziStepMotor::csLock = std::shared_ptr<TCriticalSection>(new TCriticalSection());

EziStepMotor::EziStepMotor(BaseChamber *chamber, PORTINFO portInfo)
	: DamperMotor(chamber, portInfo, false)
{
	FHomeInitialized = false;
	FEnabled = false;
	FAlarmCode = 0;
	FCurrentPos = 0;

	FRevolutionCounter = 10000;
}

__fastcall EziStepMotor::~EziStepMotor()
{
}

bool EziStepMotor::IsAlarm()
{
	EZISTEP_AXISSTATUS axisStatus;
	axisStatus.dwValue = FAlarmCode;
	return (axisStatus.FFLAG_ERRORALL);
}

bool EziStepMotor::SetParameter(int axisCount)
{
	long lParamVal;
	int ret;
	for (int i=1; i <= axisCount; i++)
	{
		if (FAS_IsSlaveExist(PortNo, i))
		{
			ret = FAS_GetParameter(PortNo, i, STEP_STOPCURRENT, &lParamVal);
			if (ret == FMM_OK)
			{
				if (lParamVal != 50)
				{
					lParamVal = 50;
					ret = FAS_SetParameter(PortNo, i, STEP_STOPCURRENT, lParamVal);
					if (ret != FMM_OK)
					{
						return false;
					}
				}
			}
			else
			{
				return false;
            }
			ret = FAS_SaveAllParameters(PortNo, i);
			if (ret != FMM_OK)
			{
				return false;
			}
		}
		else
		{
			return false;
        }
	}
	return true;
}

bool EziStepMotor::Connect(int portNo)
{
	if (IsConnected)
	{
		return true;
	}

	PortNo = portNo;
	BOOL ret = FAS_Connect((BYTE)PortNo, 115200);
	if (ret == TRUE)
	{
		IsConnected = true;
	}
	return IsConnected;
}

bool EziStepMotor::Disconnect(int axisCount)
{
	if (IsConnected == false)
	{
		return true;
	}

	for (int i=1; i <= axisCount; i++)
	{
		if (FAS_IsSlaveExist(PortNo, i))
		{
			int ret = FAS_ServoEnable(PortNo, i, FALSE);
			if (ret == FMM_OK)
			{
				FAS_Close(PortNo);
				IsConnected = false;
				return true;
			}
		}
	}
	return false;
}

void __fastcall EziStepMotor::Execute()
{
	while( !Terminated )
	{
		WaitForSingleObject((void *)Handle, THREAD_WAIT_TIME);

		if (IsConnected)
		{
			csLock->Acquire();
			DoCommunicate();
			Sleep(100);
			csLock->Release();
		}
	}
}

bool EziStepMotor::WaitRx1()
{
	while( 1 )
	{
		WaitForSingleObject((void *)Handle, THREAD_WAIT_TIME);

		if( FuncResult == FMM_OK )
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

bool EziStepMotor::WaitRx2()
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

		if( FuncResult == FMM_OK )
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

bool EziStepMotor::RetryCommand()
{
	if (IsConnected == false)
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

void EziStepMotor::DoMonitor()
{
	if (FAS_IsSlaveExist(PortNo, Addr))
	{
		DWORD status;
		FuncResult = FAS_GetAxisStatus(PortNo, Addr, &status);
		if (FuncResult == FMM_OK)
		{
			FAlarmCode = status;
			CommFailCount = 0;
			FCommFail = false;
			INC(FSuccessCount);
		}
		else
		{
			CommFailCount++;
			INC(FFailCount);
			if( CommFailCount > MAX_RETRY_COUNT )
			{
				CommFailCount = 0;
				FCommFail = true;
			}
		}

		long cmdPos, actPos, posErr, actVel;
		Word posItemNo;
		FuncResult = FAS_GetMotionStatus(PortNo, Addr, &cmdPos, &actPos, &posErr, &actVel, &posItemNo);
		if (FuncResult == FMM_OK)
		{
			FCurrentPos = cmdPos;
			CommFailCount = 0;
			FCommFail = false;
			INC(FSuccessCount);
		}
		else
		{
			CommFailCount++;
			INC(FFailCount);
			if( CommFailCount > MAX_RETRY_COUNT )
			{
				CommFailCount = 0;
				FCommFail = true;
			}
		}
	}
}
/*
void __fastcall EziStepMotor::DoIsSlaveExist(PARAM *param)
{
	if (IsConnected == false)
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 500;

	FuncResult = FAS_IsSlaveExist(PortNo, Addr);

	TimeoutTimer->Interval = oldTimeout;
}

bool __fastcall EziStepMotor::IsSlaveExist()
{
	bool ret = false;

	PARAM param;

	FuncResult = -1;
	Param = &param;
	CommandFunc = DoIsSlaveExist;
	ret = RetryCommand();

	return ret;
}
*/
void EziStepMotor::DoMotorEnable(PARAM *param)
{
	if (IsConnected == false)
		return;
	if (FAS_IsSlaveExist(PortNo, Addr) == FALSE)
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 500;

	BOOL enable = (param->Bool == true) ? TRUE : FALSE;
	if (enable == TRUE)
	{
		int ret;
		EZISTEP_AXISSTATUS AxisStatus;
		ret = FAS_GetAxisStatus(PortNo, Addr, &(AxisStatus.dwValue));
		if (ret == FMM_OK)
		{
			if (AxisStatus.FFLAG_ERRORALL || AxisStatus.FFLAG_ERROVERCURRENT || AxisStatus.FFLAG_ERRSTEPALARM)
			{
				ret = FAS_StepAlarmReset(PortNo, Addr, TRUE);
				Sleep(500);
				ret = FAS_StepAlarmReset(PortNo, Addr, FALSE);
			}

			FuncResult = FAS_ServoEnable(PortNo, Addr, enable);
		}
	}
	else
	{
		FuncResult = FAS_ServoEnable(PortNo, Addr, enable);
	}
	if (FuncResult == FMM_OK)
	{
		FEnabled = enable;
	}

	TimeoutTimer->Interval = oldTimeout;
}

bool EziStepMotor::MotorEnable(bool enable/*=true*/)
{
	bool ret = false;

	PARAM param;
	param.Bool = enable;

	FuncResult = -1;
	Param = &param;
	CommandFunc = DoMotorEnable;
	ret = RetryCommand();

	DelayTime(1000);

	return ret;
}

void EziStepMotor::DoStop(PARAM *param)
{
	if (IsConnected == false)
		return;
	if (FAS_IsSlaveExist(PortNo, Addr) == FALSE)
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 500;

	BOOL enable = (param->Bool == true) ? TRUE : FALSE;
	FuncResult = FAS_MoveStop(PortNo, Addr);

	TimeoutTimer->Interval = oldTimeout;
}

bool EziStepMotor::Stop()
{
	bool ret = false;

	FuncResult = -1;
	Param = nullptr;
	CommandFunc = DoStop;
	ret = RetryCommand();

	return ret;
}

void EziStepMotor::DoAlarmClear(PARAM *param)
{
	if (IsConnected == false)
		return;
	if (FAS_IsSlaveExist(PortNo, Addr) == FALSE)
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 500;

	FuncResult = FAS_StepAlarmReset(PortNo, Addr, 1);
	Sleep(500);
	FuncResult = FAS_StepAlarmReset(PortNo, Addr, 0);

	TimeoutTimer->Interval = oldTimeout;
}

bool EziStepMotor::AlarmClear()
{
	bool ret = false;

	FuncResult = -1;
	Param = nullptr;
	CommandFunc = DoAlarmClear;
	ret = RetryCommand();

	return ret;
}

void EziStepMotor::Homing()
{
	if (IsConnected == false)
		return;
	if (FAS_IsSlaveExist(PortNo, Addr) == FALSE)
		return;

	FAS_StepAlarmReset(PortNo, Addr, TRUE);
	Sleep(500);
	FAS_StepAlarmReset(PortNo, Addr, FALSE);
	FAS_ServoEnable(PortNo, Addr, TRUE);

	//Org Method.
	long lParamVal;
	if(FAS_GetParameter(PortNo, Addr, STEP_ORGMETHOD, &lParamVal) == FMM_OK)
	{
		if(lParamVal != 0)
		{
			lParamVal = 0;
			FAS_SetParameter(PortNo, Addr, STEP_ORGMETHOD, lParamVal);
		}
	}
	else
	{
		return;
	}
	//Org Speed
	if(FAS_GetParameter(PortNo, Addr, STEP_ORGSPEED, &lParamVal) == FMM_OK)
	{
		if(lParamVal != 5000)
		{
			lParamVal = 5000;
			FAS_SetParameter(PortNo, Addr, STEP_ORGSPEED, lParamVal);
		}
	}
	else
	{
		return;
	}
	//Org Search Speed
	if(FAS_GetParameter(PortNo, Addr, STEP_ORGSEARCHSPEED, &lParamVal) == FMM_OK)
	{
		if(lParamVal != 1000)
		{
			lParamVal = 1000;
			FAS_SetParameter(PortNo, Addr, STEP_ORGSEARCHSPEED, lParamVal);
		}
	}
	else
	{
		return;
	}
	//Org Acc/Dec Time
	if(FAS_GetParameter(PortNo, Addr, STEP_ORGACCDECTIME, &lParamVal) == FMM_OK)
	{
		if(lParamVal != 50)
		{
			lParamVal = 50;
			FAS_SetParameter(PortNo, Addr, STEP_ORGACCDECTIME, lParamVal);
		}
	}
	else
	{
		return;
	}
	//Org Method
	if(FAS_GetParameter(PortNo, Addr, STEP_ORGMETHOD, &lParamVal) == FMM_OK)
	{
		if(lParamVal != 0)
		{
			lParamVal = 0;
			FAS_SetParameter(PortNo, Addr, STEP_ORGMETHOD, lParamVal);
		}
	}
	else
	{
		return;
	}
	//Org Dir.
	if(FAS_GetParameter(PortNo, Addr, STEP_ORGDIR, &lParamVal) == FMM_OK)
	{
		if(lParamVal != 0)
		{
			lParamVal = 0;
			FAS_SetParameter(PortNo, Addr, STEP_ORGDIR, lParamVal);
		}
	}
	else
	{
		return;
	}
	//Org Off-set
	if(FAS_GetParameter(PortNo, Addr, STEP_ORGOFFSET, &lParamVal) == FMM_OK)
	{
		if(lParamVal != 0)
		{
			lParamVal = 0;
			FAS_SetParameter(PortNo, Addr, STEP_ORGOFFSET, lParamVal);
		}
	}
	else
	{
		return;
	}
	//Org Position Set
	if(FAS_GetParameter(PortNo, Addr, STEP_ORGPOSITIONSET, &lParamVal) == FMM_OK)
	{
		if(lParamVal != 0)
		{
			lParamVal = 0;
			FAS_SetParameter(PortNo, Addr, STEP_ORGPOSITIONSET, lParamVal);
		}
	}
	else
	{
		return;
	}
	//Org Sensor Logic
	if(FAS_GetParameter(PortNo, Addr, STEP_ORGSENSORLOGIC, &lParamVal) == FMM_OK)
	{
		if(lParamVal != 0)
		{
			lParamVal = 0;
			FAS_SetParameter(PortNo, Addr, STEP_ORGSENSORLOGIC, lParamVal);
		}
	}
 	else
	{
		return;
	}

	if (FAS_MoveOriginSingleAxis(PortNo, Addr) != FMM_OK)
	{
		return;
	}


	// Check Homing complete
	EZISTEP_AXISSTATUS stAxisStatus;
	DWORD dwAxisStatus;
	int waitCount = 0;

	while (1)
	{
		Sleep(100);

		if (FAS_GetAxisStatus(PortNo, Addr, &dwAxisStatus)==FMM_OK)
		{
			stAxisStatus.dwValue = dwAxisStatus;
			if (stAxisStatus.FFLAG_ORIGINRETURNING || !stAxisStatus.FFLAG_ORIGINSENSOR)
			{
				waitCount++;
				if (waitCount > 100)//10sec.
				{
					FuncResult = -1;
					FHomeInitialized = false;
					break;
				}
			}
			else
			{
				FuncResult = 0;
				FHomeInitialized = true;
				break;
			}
		}
		else
		{
			break;
        }
	}
}

void EziStepMotor::DoSearchHome(PARAM *param)
{
	if (IsConnected == false)
		return;
	if (FAS_IsSlaveExist(PortNo, Addr) == FALSE)
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 20000;
	Homing();
	TimeoutTimer->Interval = oldTimeout;
}

bool EziStepMotor::SearchHome()
{
	bool ret = false;

	FuncResult = -1;
	Param = nullptr;
	CommandFunc = DoSearchHome;
	ret = RetryCommand();

	DelayTime(2000, true);

	return ret;
}

void EziStepMotor::DoMoveStepAbsolute(PARAM *param)
{
	if (IsConnected == false)
		return;
	if (FAS_IsSlaveExist(PortNo, Addr) == FALSE)
		return;

	int oldTimeout = TimeoutTimer->Interval;
	TimeoutTimer->Interval = 500;

	int pos = param->Int;

	EZISTEP_AXISSTATUS stAxisStatus;
	DWORD dwStatus, dwRet;
	int nRet;

	nRet = FAS_GetAxisStatus(PortNo, Addr, &dwStatus);
	if (nRet == FMM_OK)
	{
		// Alarm Reset - Step Disable 상태 시. (FFLAG_ALARMRESET)
		dwRet = dwStatus & 0x00200000;
		if (dwRet)
		{
			// 스텝 OFF -> Alarm Reset flag, 초기화 모션 시도.
			Homing();
		}
		else
		{
			// Retry Move.
			DWORD dwInput;
			nRet = FAS_GetIOInput(PortNo, Addr, &dwInput);
			if (nRet == FMM_OK)
			{
				// I/O Not Setting.
				if (dwInput & (STEP_IN_BITMASK_STOP | STEP_IN_BITMASK_PAUSE | SERVO_IN_BITMASK_ESTOP))
				{
					FAS_SetIOInput(PortNo, Addr, 0, STEP_IN_BITMASK_STOP | STEP_IN_BITMASK_PAUSE | STEP_IN_BITMASK_ESTOP);
				}

				if (FAS_MoveSingleAxisAbsPos(PortNo, Addr, (long)pos, 10000) == FMM_OK)
				{
					do
					{
						Sleep(1);
						nRet = FAS_GetAxisStatus(PortNo, Addr, &dwStatus);
						if (nRet == FMM_OK)
						{
							stAxisStatus.dwValue = dwStatus;
							FuncResult = 0;
						}
					}
					while (stAxisStatus.FFLAG_MOTIONING);
				}
				else
				{
				}
			}
		}
	}

	TimeoutTimer->Interval = oldTimeout;
}

bool EziStepMotor::MoveStepAbsolute(int pos)
{
	bool ret = false;

	PARAM param;
	param.Int = pos;

	FuncResult = -1;
	Param = &param;
	CommandFunc = DoMoveStepAbsolute;
	ret = RetryCommand();

	return ret;
}

bool EziStepMotor::Open(int percent)
{
	int step = FRevolutionCounter / 4;
	step = step * percent;
	step = step / 100;
	return MoveStepAbsolute(step);
}

bool EziStepMotor::Close()
{
	return MoveStepAbsolute(0);
}

