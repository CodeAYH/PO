#pragma once
#include <Windows.h>

#ifdef EZI_SERVO_PLUSR_EXPORTS
	#define EZI_PLUSR_API extern "C" __declspec(dllexport)
	
	#include "MOTION_DEFINE.h"
#else
	#define EZI_PLUSR_API extern "C" __declspec(dllimport)

	#pragma comment( lib, "EziMOTIONPlusR.lib" )

	#include "ReturnCodes_Define.h"
	#include "MOTION_DEFINE.h"
	#include "COMM_Define.h"
#endif

//------------------------------------------------------------------------------
//					Connection Functions
//------------------------------------------------------------------------------
EZI_PLUSR_API BOOL WINAPI	FAS_Connect(BYTE nPortNo, DWORD dwBaud);

EZI_PLUSR_API BOOL WINAPI	FAS_OpenPort(BYTE nPortNo, DWORD dwBaud);
EZI_PLUSR_API BOOL WINAPI	FAS_AttachSlave(BYTE nPortNo, BYTE iSlaveNo);

EZI_PLUSR_API void WINAPI	FAS_Close(BYTE nPortNo);

EZI_PLUSR_API BOOL WINAPI	FAS_IsSlaveExist(BYTE nPortNo, BYTE iSlaveNo);

//------------------------------------------------------------------------------
//					Log Functions
//------------------------------------------------------------------------------
EZI_PLUSR_API void WINAPI	FAS_EnableLog(BOOL bEnable);
EZI_PLUSR_API void WINAPI	FAS_SetLogLevel(enum LOG_LEVEL level);
EZI_PLUSR_API BOOL WINAPI	FAS_SetLogPath(LPCWSTR lpPath);

EZI_PLUSR_API void WINAPI	FAS_PrintCustomLog(BYTE nPortNo, enum LOG_LEVEL level, LPCTSTR lpszMsg);

//------------------------------------------------------------------------------
//					Info Functions
//------------------------------------------------------------------------------
EZI_PLUSR_API int WINAPI	FAS_GetSlaveInfo(BYTE nPortNo, BYTE iSlaveNo, BYTE* pType, LPSTR lpBuff, int nBuffSize);
EZI_PLUSR_API int WINAPI	FAS_GetMotorInfo(BYTE nPortNo, BYTE iSlaveNo, BYTE* pType, LPSTR lpBuff, int nBuffSize);
EZI_PLUSR_API int WINAPI	FAS_GetSlaveInfoEx(BYTE nPortNo, BYTE iSlaveNo, DRIVE_INFO* lpDriveInfo);

//------------------------------------------------------------------------------
//					Parameter Functions
//------------------------------------------------------------------------------
EZI_PLUSR_API int WINAPI	FAS_SaveAllParameters(BYTE nPortNo, BYTE iSlaveNo);
EZI_PLUSR_API int WINAPI	FAS_SetParameter(BYTE nPortNo, BYTE iSlaveNo, BYTE iParamNo, long lParamValue);
EZI_PLUSR_API int WINAPI	FAS_GetParameter(BYTE nPortNo, BYTE iSlaveNo, BYTE iParamNo, long* lParamValue);
EZI_PLUSR_API int WINAPI	FAS_GetROMParameter(BYTE nPortNo, BYTE iSlaveNo, BYTE iParamNo, long* lRomParam);

//------------------------------------------------------------------------------
//					IO Functions
//------------------------------------------------------------------------------
EZI_PLUSR_API int WINAPI	FAS_SetIOInput(BYTE nPortNo, BYTE iSlaveNo, DWORD dwIOSETMask, DWORD dwIOCLRMask);
EZI_PLUSR_API int WINAPI	FAS_GetIOInput(BYTE nPortNo, BYTE iSlaveNo, DWORD* dwIOInput);

EZI_PLUSR_API int WINAPI	FAS_SetIOOutput(BYTE nPortNo, BYTE iSlaveNo, DWORD dwIOSETMask, DWORD dwIOCLRMask);
EZI_PLUSR_API int WINAPI	FAS_GetIOOutput(BYTE nPortNo, BYTE iSlaveNo, DWORD* dwIOOutput);

EZI_PLUSR_API int WINAPI	FAS_GetIOAssignMap(BYTE nPortNo, BYTE iSlaveNo, BYTE iIOPinNo, DWORD* dwIOLogicMask, BYTE* bLevel);
EZI_PLUSR_API int WINAPI	FAS_SetIOAssignMap(BYTE nPortNo, BYTE iSlaveNo, BYTE iIOPinNo, DWORD dwIOLogicMask, BYTE bLevel);

EZI_PLUSR_API int WINAPI	FAS_IOAssignMapReadROM(BYTE nPortNo, BYTE iSlaveNo);

//------------------------------------------------------------------------------
//					Servo Driver Control Functions
//------------------------------------------------------------------------------		
EZI_PLUSR_API int WINAPI	FAS_ServoEnable(BYTE nPortNo, BYTE iSlaveNo, BOOL bOnOff);
EZI_PLUSR_API int WINAPI	FAS_ServoAlarmReset(BYTE nPortNo, BYTE iSlaveNo);
EZI_PLUSR_API int WINAPI	FAS_StepAlarmReset(BYTE nPortNo, BYTE iSlaveNo, BOOL bReset);

//------------------------------------------------------------------------------
//					Read Status and Position
//------------------------------------------------------------------------------
EZI_PLUSR_API int WINAPI	FAS_GetAxisStatus(BYTE nPortNo, BYTE iSlaveNo, DWORD* dwAxisStatus);
EZI_PLUSR_API int WINAPI	FAS_GetIOAxisStatus(BYTE nPortNo, BYTE iSlaveNo, DWORD* dwInStatus, DWORD* dwOutStatus, DWORD* dwAxisStatus);
EZI_PLUSR_API int WINAPI	FAS_GetMotionStatus(BYTE nPortNo, BYTE iSlaveNo, long* lCmdPos, long* lActPos, long* lPosErr, long* lActVel, WORD* wPosItemNo);
EZI_PLUSR_API int WINAPI	FAS_GetAllStatus(BYTE nPortNo, BYTE iSlaveNo, DWORD* dwInStatus, DWORD* dwOutStatus, DWORD* dwAxisStatus, long* lCmdPos, long* lActPos, long* lPosErr, long* lActVel, WORD* wPosItemNo);
EZI_PLUSR_API int WINAPI	FAS_GetAllStatusEx(BYTE nPortNo, BYTE iSlaveNo, BYTE* pTypes, long* pDatas);

EZI_PLUSR_API int WINAPI	FAS_SetCommandPos(BYTE nPortNo, BYTE iSlaveNo, long lCmdPos);
EZI_PLUSR_API int WINAPI	FAS_SetActualPos(BYTE nPortNo, BYTE iSlaveNo, long lActPos);
EZI_PLUSR_API int WINAPI	FAS_ClearPosition(BYTE nPortNo, BYTE iSlaveNo);
EZI_PLUSR_API int WINAPI	FAS_GetCommandPos(BYTE nPortNo, BYTE iSlaveNo, long* lCmdPos);
EZI_PLUSR_API int WINAPI	FAS_GetActualPos(BYTE nPortNo, BYTE iSlaveNo, long* lActPos);
EZI_PLUSR_API int WINAPI	FAS_GetPosError(BYTE nPortNo, BYTE iSlaveNo, long* lPosErr);
EZI_PLUSR_API int WINAPI	FAS_GetActualVel(BYTE nPortNo, BYTE iSlaveNo, long* lActVel);

EZI_PLUSR_API int WINAPI	FAS_GetAlarmType(BYTE nPortNo, BYTE iSlaveNo, BYTE* nAlarmType);

//------------------------------------------------------------------
//					Motion Functions.
//------------------------------------------------------------------
EZI_PLUSR_API int WINAPI	FAS_MoveStop(BYTE nPortNo, BYTE iSlaveNo);
EZI_PLUSR_API int WINAPI	FAS_EmergencyStop(BYTE nPortNo, BYTE iSlaveNo);

EZI_PLUSR_API int WINAPI	FAS_MovePause(BYTE nPortNo, BYTE iSlaveNo, BOOL bPause);

EZI_PLUSR_API int WINAPI	FAS_MoveOriginSingleAxis(BYTE nPortNo, BYTE iSlaveNo);
EZI_PLUSR_API int WINAPI	FAS_MoveSingleAxisAbsPos(BYTE nPortNo, BYTE iSlaveNo, long lAbsPos, DWORD lVelocity);
EZI_PLUSR_API int WINAPI	FAS_MoveSingleAxisIncPos(BYTE nPortNo, BYTE iSlaveNo, long lIncPos, DWORD lVelocity);
EZI_PLUSR_API int WINAPI	FAS_MoveToLimit(BYTE nPortNo, BYTE iSlaveNo, DWORD lVelocity, int iLimitDir);
EZI_PLUSR_API int WINAPI	FAS_MoveVelocity(BYTE nPortNo, BYTE iSlaveNo, DWORD lVelocity, int iVelDir);

EZI_PLUSR_API int WINAPI	FAS_PositionAbsOverride(BYTE nPortNo, BYTE iSlaveNo, long lOverridePos);
EZI_PLUSR_API int WINAPI	FAS_PositionIncOverride(BYTE nPortNo, BYTE iSlaveNo, long lOverridePos);
EZI_PLUSR_API int WINAPI	FAS_VelocityOverride(BYTE nPortNo, BYTE iSlaveNo, DWORD lVelocity);

EZI_PLUSR_API int WINAPI	FAS_MoveLinearAbsPos(BYTE nPortNo, BYTE nNoOfSlaves, BYTE* iSlavesNo, long* lAbsPos, DWORD lFeedrate, WORD wAccelTime);
EZI_PLUSR_API int WINAPI	FAS_MoveLinearIncPos(BYTE nPortNo, BYTE nNoOfSlaves, BYTE* iSlavesNo, long* lIncPos, DWORD lFeedrate, WORD wAccelTime);

EZI_PLUSR_API int WINAPI	FAS_MoveLinearAbsPos2(BYTE nPortNo, BYTE nNoOfSlaves, BYTE* iSlavesNo, long* lAbsPos, DWORD lFeedrate, WORD wAccelTime);
EZI_PLUSR_API int WINAPI	FAS_MoveLinearIncPos2(BYTE nPortNo, BYTE nNoOfSlaves, BYTE* iSlavesNo, long* lIncPos, DWORD lFeedrate, WORD wAccelTime);

EZI_PLUSR_API int WINAPI	FAS_TriggerOutput_RunA(BYTE nPortNo, BYTE iSlaveNo, BOOL bStartTrigger, long lStartPos, DWORD dwPeriod, DWORD dwPulseTime);
EZI_PLUSR_API int WINAPI	FAS_TriggerOutput_Status(BYTE nPortNo, BYTE iSlaveNo, BYTE* bTriggerStatus);

EZI_PLUSR_API int WINAPI	FAS_SetTriggerOutputEx(BYTE nPortNo, BYTE iSlaveNo, BYTE nOutputNo, BYTE bRun, WORD wOnTime, BYTE nTriggerCount, long* arrTriggerPosition);
EZI_PLUSR_API int WINAPI	FAS_GetTriggerOutputEx(BYTE nPortNo, BYTE iSlaveNo, BYTE nOutputNo, BYTE* bRun, WORD* wOnTime, BYTE* nTriggerCount, long* arrTriggerPosition);

EZI_PLUSR_API int WINAPI	FAS_MovePush(BYTE nPortNo, BYTE iSlaveNo, DWORD dwStartSpd, DWORD dwMoveSpd, long lPosition, WORD wAccel, WORD wDecel, WORD wPushRate, DWORD dwPushSpd, long lEndPosition, WORD wPushMode);
EZI_PLUSR_API int WINAPI	FAS_GetPushStatus(BYTE nPortNo, BYTE iSlaveNo, BYTE* nPushStatus);

//------------------------------------------------------------------
//					Ex-Motion Functions.
//------------------------------------------------------------------
EZI_PLUSR_API int WINAPI	FAS_MoveSingleAxisAbsPosEx(BYTE nPortNo, BYTE iSlaveNo, long lAbsPos, DWORD lVelocity, MOTION_OPTION_EX* lpExOption);
EZI_PLUSR_API int WINAPI	FAS_MoveSingleAxisIncPosEx(BYTE nPortNo, BYTE iSlaveNo, long lIncPos, DWORD lVelocity, MOTION_OPTION_EX* lpExOption);
EZI_PLUSR_API int WINAPI	FAS_MoveVelocityEx(BYTE nPortNo, BYTE iSlaveNo, DWORD lVelocity, int iVelDir, VELOCITY_OPTION_EX* lpExOption);

//------------------------------------------------------------------
//					All-Motion Functions.
//------------------------------------------------------------------
EZI_PLUSR_API int WINAPI	FAS_AllMoveStop(BYTE nPortNo);
EZI_PLUSR_API int WINAPI	FAS_AllEmergencyStop(BYTE nPortNo);
EZI_PLUSR_API int WINAPI	FAS_AllMoveOriginSingleAxis(BYTE nPortNo);
EZI_PLUSR_API int WINAPI	FAS_AllMoveSingleAxisAbsPos(BYTE nPortNo, long lAbsPos, DWORD lVelocity);
EZI_PLUSR_API int WINAPI	FAS_AllMoveSingleAxisIncPos(BYTE nPortNo, long lIncPos, DWORD lVelocity);

//------------------------------------------------------------------
//					Position Table Functions.
//------------------------------------------------------------------
EZI_PLUSR_API int WINAPI	FAS_PosTableReadItem(BYTE nPortNo, BYTE iSlaveNo, WORD wItemNo, LPITEM_NODE lpItem);
EZI_PLUSR_API int WINAPI	FAS_PosTableWriteItem(BYTE nPortNo, BYTE iSlaveNo, WORD wItemNo, LPITEM_NODE lpItem);
EZI_PLUSR_API int WINAPI	FAS_PosTableWriteROM(BYTE nPortNo, BYTE iSlaveNo);
EZI_PLUSR_API int WINAPI	FAS_PosTableReadROM(BYTE nPortNo, BYTE iSlaveNo);
EZI_PLUSR_API int WINAPI	FAS_PosTableRunItem(BYTE nPortNo, BYTE iSlaveNo, WORD wItemNo);

EZI_PLUSR_API int WINAPI	FAS_PosTableReadOneItem(BYTE nPortNo, BYTE iSlaveNo, WORD wItemNo, WORD wOffset, long* lPosItemVal);
EZI_PLUSR_API int WINAPI	FAS_PosTableWriteOneItem(BYTE nPortNo, BYTE iSlaveNo, WORD wItemNo, WORD wOffset, long lPosItemVal);

EZI_PLUSR_API int WINAPI	FAS_PosTableSingleRunItem(BYTE nPortNo, BYTE iSlaveNo, BOOL bNextMove, WORD wItemNo);

//------------------------------------------------------------------
//					Gap Control Functions.
//------------------------------------------------------------------
EZI_PLUSR_API int WINAPI	FAS_GapControlEnable(BYTE nPortNo, BYTE iSlaveNo, WORD wItemNo, long lGapCompSpeed, long lGapAccTime, long lGapDecTime, long lGapStartSpeed);
EZI_PLUSR_API int WINAPI	FAS_GapControlDisable(BYTE nPortNo, BYTE iSlaveNo);
EZI_PLUSR_API int WINAPI	FAS_IsGapControlEnable(BYTE nPortNo, BYTE iSlaveNo, BOOL* bIsEnable, WORD* wCurrentItemNo);

EZI_PLUSR_API int WINAPI	FAS_GapControlGetADCValue(BYTE nPortNo, BYTE iSlaveNo, long* lADCValue);
EZI_PLUSR_API int WINAPI	FAS_GapOneResultMonitor(BYTE nPortNo, BYTE iSlaveNo, BYTE* bUpdated, long* iIndex, long* lGapValue, long* lCmdPos, long* lActPos, long* lCompValue, long* lReserved);

//------------------------------------------------------------------
//					Alarm Type History Functions.
//------------------------------------------------------------------
EZI_PLUSR_API int WINAPI	FAS_GetAlarmLogs(BYTE nPortNo, BYTE iSlaveNo, ALARM_LOG* pAlarmLog);
EZI_PLUSR_API int WINAPI	FAS_ResetAlarmLogs(BYTE nPortNo, BYTE iSlaveNo);

//------------------------------------------------------------------
//					I/O Module Functions.
//------------------------------------------------------------------
EZI_PLUSR_API int WINAPI	FAS_GetInput(BYTE nPortNo, BYTE iSlaveNo, unsigned long* uInput, unsigned long* uLatch);

EZI_PLUSR_API int WINAPI	FAS_ClearLatch(BYTE nPortNo, BYTE iSlaveNo, unsigned long uLatchMask);
EZI_PLUSR_API int WINAPI	FAS_GetLatchCount(BYTE nPortNo, BYTE iSlaveNo, unsigned char iInputNo, unsigned long* uCount);
EZI_PLUSR_API int WINAPI	FAS_GetLatchCountAll(BYTE nPortNo, BYTE iSlaveNo, unsigned long** ppuAllCount);
EZI_PLUSR_API int WINAPI	FAS_GetLatchCountAll32(BYTE nPortNo, BYTE iSlaveNo, unsigned long** ppuAllCount);
EZI_PLUSR_API int WINAPI	FAS_ClearLatchCount(BYTE nPortNo, BYTE iSlaveNo, unsigned long uInputMask);

EZI_PLUSR_API int WINAPI	FAS_GetOutput(BYTE nPortNo, BYTE iSlaveNo, unsigned long* uOutput, unsigned long* uStatus);
EZI_PLUSR_API int WINAPI	FAS_SetOutput(BYTE nPortNo, BYTE iSlaveNo, unsigned long uSetMask, unsigned long uClearMask);

EZI_PLUSR_API int WINAPI	FAS_SetTrigger(BYTE nPortNo, BYTE iSlaveNo, unsigned char uOutputNo, TRIGGER_INFO* pTrigger);
EZI_PLUSR_API int WINAPI	FAS_SetRunStop(BYTE nPortNo, BYTE iSlaveNo, unsigned long uRunMask, unsigned long uStopMask);
EZI_PLUSR_API int WINAPI	FAS_GetTriggerCount(BYTE nPortNo, BYTE iSlaveNo, unsigned char uOutputNo, unsigned long* uCount);

EZI_PLUSR_API int WINAPI	FAS_GetIOLevel(BYTE nPortNo, BYTE iSlaveNo, unsigned long* uIOLevel);
EZI_PLUSR_API int WINAPI	FAS_SetIOLevel(BYTE nPortNo, BYTE iSlaveNo, unsigned long uIOLevel);
EZI_PLUSR_API int WINAPI	FAS_LoadIOLevel(BYTE nPortNo, BYTE iSlaveNo);
EZI_PLUSR_API int WINAPI	FAS_SaveIOLevel(BYTE nPortNo, BYTE iSlaveNo);

EZI_PLUSR_API int WINAPI	FAS_GetInputFilter(BYTE nPortNo, BYTE iSlaveNo, unsigned short* filter);
EZI_PLUSR_API int WINAPI	FAS_SetInputFilter(BYTE nPortNo, BYTE iSlaveNo, unsigned short filter);

//------------------------------------------------------------------
//					Ez-IO Plus-AD Functions
//------------------------------------------------------------------
EZI_PLUSR_API int WINAPI	FAS_GetAllADResult(BYTE nPortNo, BYTE iSlaveNo, AD_RESULT* result);
EZI_PLUSR_API int WINAPI	FAS_GetADResult(BYTE nPortNo, BYTE iSlaveNo, BYTE channel, float* adresult);
EZI_PLUSR_API int WINAPI	FAS_SetADRange(BYTE nPortNo, BYTE iSlaveNo, BYTE channel, AD_RANGE range);
