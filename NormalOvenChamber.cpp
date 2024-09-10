//---------------------------------------------------------------------------
#include <System.DateUtils.hpp>

#pragma hdrstop

#include "NormalOvenSystemManager.h"
#include "NormalOvenChamber.h"
#include "HelperFunctions.h"
#include "MainTempController.h"
#include "ST340.h"
#include "I7018.h"
#include "I7000DIOModule.h"
#include "ZR5.h"
#include "MoonsStepMotor.h"
#include "DiffPressure.h"
#include "Database.h"
#include "Defines.h"
#include "GEMForm.h"
#include "BarunMES.h"
#include "VSOvenBoard.h"
#include "VSOvenBoardDIO.h"
#include "LSInverter.h"
#include "REX_P250.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

/*
DO1 : D1106 ~ DO7 : D1112
RESET		5073(13D1)	PV이벤트1	5113(13F9)	TIME이벤트1		5129(1409)
PROG		5074(13D2)	PV이벤트2	5114		TIME이벤트2		5130
LOCAL		5075    	PV이벤트3	5115		TIME이벤트3		5131
경보1		5097		PV이벤트4	5117		TIME이벤트4		5133
경보2		5098		PV이벤트5	5118		TIME이벤트5		5134
경보3		5099		PV이벤트6	5119		TIME이벤트6		5135
경보4		5101		PV이벤트7	5121		TIME이벤트7		5137
패턴 END	5157		PV이벤트8	5122		TIME이벤트8		5138

// 온도 상한 관련 레지스터
D1204(RH) :	300도=0BB8 500도=1388 600도= 1770
D1232(P.RH1)
D0933(SPH.1)
D1014(RTH1)
*/
const char UP550ParamStr[UP550_PARAM_COUNT][300] =
{
	"%c%02d010WRW13D1280,0001,D1281,0001,D1201,0003,D1202,0001,D1204,0BB8,D1205,0000,D1209,0001,D1210,0001,D0906,0000,D1230,0001,D1231,0001,D1232,0BB8,D1233,0000%c%c",
	"%c%02d010WRW02D1238,0001,D1240,0002%c%c",
	"%c%02d010WRW06D0902,0000,D0904,0000,D0905,0000,D0939,0000,D0933,0BB8,D0934,0000%c%c",
	"%c%02d010WRW17D0907,0005,D0908,0005,D0909,0005,D0910,0005,D0911,0005,D0912,0005,D0913,0005,D0914,0005,D0915,0001,D0916,0002,D0917,0001,D0918,0002,D0919,000F,D0920,000F,D0921,000F,D0922,000F,D0923,0000%c%c",
	"%c%02d010WRW04D1001,0000,D1002,0000,D1003,0000,D1004,000A%c%c",
	"%c%02d010WRW03D1013,0001,D1014,0BB8,D1015,0000%c%c",
	"%c%02d010WRW02D1021,0096,D1023,0005%c%c",
	"%c%02d010WRW11D1024,0000,D1026,0000,D1027,0000,D1028,0000,D1029,0000,D1030,0000,D1031,0001,D1032,0000,D1033,0001,D1034,0000,D1035,0001%c%c",
	"%c%02d010WRW05D1101,0000,D1102,0000,D1103,0000,D1104,0000,D1105,0000%c%c",

	// DO 설정		     DO1        DO2        DO3        DO4        DO5        DO6        DO7
	//					 PROG       TE1        TE2        PVE1       TE3        X          X
	"%c%02d010WRW07D1106,13D2,D1107,1409,D1108,140A,D1109,13F9,D1110,140B,D1111,0000,D1112,0000%c%c",
	"%c%02d010WRW21D1147,0000,D1148,0000,D1149,0000,D1150,0000,D1151,0000,D1152,0000,D1153,0000,D1154,0000,D1155,0000,D1156,0000,D1157,0000,D1158,0000,D1159,0000,D1160,0000,D1161,0000,D1162,0000,D1163,0000,D1166,0000,D1167,0000,D1168,0000,D1169,0000%c%c",
	"%c%02d010WRW01D0211,0000%c%c"
};

/*
UP55A DO Event
0: OFF (No function)
PV event-1: 4801(0x12C1)
PV event-2: 4802,
PV event-3: 4803,
PV event 4: 4805,
PV event 5: 4806,
PV event 6 4807,
PV event 7: 4809,
PV event 8: 4810,
Time event 1: 4817(0x12D1),
Time event 2: 4818,
Time event 3: 4819(0x12D3),
Time event 4: 4821(0x12D5),
Time event 5: 4822(0X12D6),
Time event 6: 4823,
Time event 7: 4825,
Time event 8: 4826,
Time event 9: 4833,
Time event 10: 4834,
Time event 11: 4835,
Time event 12: 4837,
Time event 13: 4838,
Time event 14: 4839,
Time event 15: 4841,
Time event 16: 4842,
Alarm 1: 4353, Alarm 2: 4354,
Alarm 3: 4355, Alarm 4: 4357,
Alarm 5: 4358, Alarm 6: 4359,
Alarm 7: 4361, Alarm 8: 4362,
AUTO (ON ) / MAN (OFF) status: 4177,
Program RESET status: 4181,
Program RUN status: 4182(0x1056),
Local operation status: 4183,
Remote operation status: 4185,
HOLD mode status: 4189,
Program advance status: 4187,
Pattern end signal (1 second): 4265,
Pattern end signal (3 seconds): 4266,
Pattern end signal (5 seconds): 4267,
Wait end signal (1 second) : 4257,
Wait end signal (3 seconds) : 4258,
Wait end signal (5 seconds) : 4259,
Output tracking (ON) switching signal: 4186
*/
const char UP55AParamStr[UP55A_PARAM_COUNT][300] =
{
	"%c%02d010WRW13D5001,0001,D5019,0001,D5101,0003,D5102,0001,D5104,0BB8,D5105,0000,D5109,0001,D5110,0001,D5018,0000,D5201,0001,D5202,0001,D5203,0BB8,D5204,0000%c%c",
	"%c%02d010WRW02D5301,0001,D5303,0002%c%c",
	"%c%02d010WRW06D2710,0000,D2707,0000,D5016,0000,D2713,0000,D5210,0BB8,D5211,0000%c%c",
	"%c%02d010WRW17D2871,0005,D2872,0005,D2873,0005,D2874,0005,D2875,0005,D2876,0005,D2877,0005,D2878,0005,D2801,0001,D2805,0002,D2809,0001,D2813,0002,D2841,000F,D2842,000F,D2843,000F,D2844,000F,D2866,0000%c%c",
	"%c%02d010WRW04D5112,0000,D5113,0000,D5114,0000,D5115,000A%c%c",
	"%c%02d010WRW03D5321,0001,D5322,0BB8,D5323,0000%c%c",
//	"%c%02d010WRW02D1021,0096,D1023,0005%c%c",
//	"%c%02d010WRW04D5819,0000,D5823,0000,D5821,0000,D5824,0000/*,D1029,0000,D1030,0000,D1031,0001,D1032,0000,D1033,0001,D1034,0000,D1035,0001*/%c%c",
	"%c%02d010WRW04D5819,0000,D5823,0000,D5821,0000,D5824,0000%c%c",
	"%c%02d010WRW05D5761,0000,D5762,0000,D5763,0000,D5764,0000,D5765,0000%c%c",

	// DO 설정		     DO1        DO2        DO3        DO4        DO5        DO6        DO7
	//					 PROG       TE1        TE2        PVE1       TE3        X          X
	"%c%02d010WRW07D6201,1056,D6202,12D1,D6203,12D2,D6241,12C1,D6242,12D3,D6243,0000,D6244,0000%c%c",
	"%c%02d010WRW18D5926,0000,D5927,0000,D5928,0000,D5913,0000,D5912,0000,D5933,0000,D5934,0000,D5929,0000,D5954,0000,D5955,0000,D5956,0000,D5957,0000,D5958,0000,D5959,0000,D5922,0000,D5923,0000,D5924,0000,D5925,0000%c%c",
	"%c%02d010WRW01D2319,0000%c%c"
};

WatchSystemStatus::WatchSystemStatus(NormalOvenChamber *chamber)
	: TThread(false), Chamber(chamber)
{
}

__fastcall WatchSystemStatus::~WatchSystemStatus()
{
}

#define	IO_DELAY	300		// 인텔 베트남 이슈로 인해 모든 알람 감지에 딜레이 추가함(2017.09.01)
void __fastcall WatchSystemStatus::Execute()
{
	while (Terminated == false)
	{
		WaitForSingleObject((void *)Handle, THREAD_WAIT_TIME);
		if (Chamber == nullptr)
		{
			continue;
		}

		if (Chamber->DamperMotor)
		{
			if (Chamber->DamperMotor->Enabled == false)
			{
				Chamber->DamperMotor->AlarmClear();
				Chamber->DamperMotor->MotorEnable();
			}

			if (Chamber->DamperMotor->HomeInitialized == false)
			{
				Chamber->DamperMotor->SearchHome();
			}
		}
		Chamber->DoDamperControl();
		Chamber->DoInverterControl();
		Chamber->DoN2FlowControl();

		if (GetManager()->UseO2Analyzer && Chamber->O2Analyzer)
		{
			if (Chamber->O2Analyzer->IsCommandInit == false)
			{
				Chamber->O2Analyzer->CommandInit();
			}
		}

		Chamber->DoMainTempControllerKeyLock();
		WaitForSingleObject((void*)Handle, 500);
		Chamber->DoMainTempControllerSetAuto();
		WaitForSingleObject((void*)Handle, 500);

		if (Chamber->IsBuzzerOffSwitchPushed())
		{
			Chamber->BuzzerOff();
		}

 		Chamber->AlarmStatus[ERR_EMERGENCY_SW] = Chamber->DIO[I_EMERGENCY]->IsOn(IO_DELAY);
		Chamber->AlarmStatus[ERR_BLOWER_OVERLOAD] = Chamber->DIO[I_BLOWER_OVERLOAD_DETECTED]->IsOn(IO_DELAY);
		Chamber->AlarmStatus[ERR_CHAMBER_OVERTEMP] = Chamber->DIO[I_CHAMBER_OVERTEMP_DETECTED]->IsOn(IO_DELAY);

		if (GetManager()->UseHeaterOverloadDetector)
		{
			Chamber->AlarmStatus[ERR_HEATER_OVERLOAD] = Chamber->DIO[I_HEATER_OVERLOAD_DETECTED]->IsOn(IO_DELAY);
		}
		if (GetManager()->UseBlowMotorLowCurrentDetector)
		{
			Chamber->AlarmStatus[ERR_BLOWER_LOW_CURRENT] = Chamber->DIO[I_BLOWER_LOW_CURRENT_DETECTED]->IsOn(IO_DELAY);
		}
		if (GetManager()->UseDiffPressureDetector)
		{
			Chamber->AlarmStatus[ERR_HEPAFILTER_CHANGE] = Chamber->DIO[I_DIFFERENTIAL_PRESSURE_DETECTED]->IsOn(30000);
		}
		if (GetManager()->UseSSROverTempDetector)
		{
			Chamber->AlarmStatus[ERR_SSR_OVERTEMP] = Chamber->DIO[I_SSR_OVERTEMP_DETECTED]->IsOn(IO_DELAY);
		}
		if (GetManager()->UseChamberDoorTempInterlock && (Chamber->Step == STEP_DONE || Chamber->Step == STEP_IDLE))
		{
			Chamber->AlarmStatus[ERR_CHAMBER_TEMP_DOOR_INTERLOCK]= Chamber->DIO[I_CHAMBER_DOOR_TEMP_INTERLOCK_DETECTED]->IsOn(IO_DELAY);
		}
		if (GetManager()->UseGroundAlarmDetector)
		{
			Chamber->AlarmStatus[ERR_GROUND_NOT_DETECTED] = Chamber->DIO[I_GROUND_ALARM_DETECTED]->IsOn(IO_DELAY);
		}
		if (GetManager()->DamperControlType != CYLINDER_DAMPER)
		{
			Chamber->AlarmStatus[ERR_DAMPER_MOTOR_ALARM] = Chamber->DIO[I_DAMPER_MOTOR_ALARM_DETECTED]->IsOn(IO_DELAY) || Chamber->DamperMotor->IsAlarm();
		}
		if (GetManager()->UseAirPressureDetector)
		{
			Chamber->AlarmStatus[ERR_AIR_PRESSURE_NOT_DETECTED]	= Chamber->DIO[I_AIR_PRESSURE_DETECTED]->IsOff(IO_DELAY);
		}
		if (GetManager()->UseElectricPanelDoorDetector)
		{
			Chamber->AlarmStatus[ERR_ELECTRONICS_DOORLOCK_NOT_DETECTED] = Chamber->DIO[I_ELECTRONICS_DOORLOCK_DETECTED]->IsOff(IO_DELAY);
		}

		if (Chamber->MainTempController)
		{
			Chamber->AlarmStatus[ERR_MAINTEMP_TIMEOUT] = Chamber->MainTempController->IsFail;
			if (GetManager()->UseWaterFlowDetector && Chamber->IsCoolingEventOn)
			{
				Chamber->AlarmStatus[ERR_CHAMBER_WATER_FLOW_NOT_DETECTED] = Chamber->DIO[I_CHAMBER_WATER_FLOW_DETECTED]->IsOff(10000);  //htest
			}
		}

		if (Chamber->ChamberTempLimitController)
		{
			Chamber->AlarmStatus[ERR_LIMIT_TIMEOUT] = Chamber->ChamberTempLimitController->IsFail;
		}
		if (Chamber->LoggerController[0])
		{
			Chamber->AlarmStatus[ERR_LOGGER_TIMEOUT] = Chamber->LoggerController[0]->IsFail;
		}
		if (GetManager()->DPType && Chamber->DPController)
		{
			Chamber->AlarmStatus[ERR_DP_TIMEOUT] = Chamber->DPController->IsFail;
		}
		if (GetManager()->DamperControlType != CYLINDER_DAMPER && Chamber->DamperMotor)
		{
			Chamber->AlarmStatus[ERR_DAMPER_MOTOR_TIMEOUT] = Chamber->DamperMotor->IsFail;
		}
		if (GetManager()->DIOType == ICPDAS)
		{
			if (Chamber->ModuleDI)
			{
				Chamber->AlarmStatus[ERR_DI_TIMEOUT] = Chamber->ModuleDI->IsFail;
			}

			if (Chamber->ModuleDO)
			{
				Chamber->AlarmStatus[ERR_DO_TIMEOUT] = Chamber->ModuleDO->IsFail;
			}

			if (Chamber->ModuleDIO)
			{
				Chamber->AlarmStatus[ERR_DIO_TIMEOUT] = Chamber->ModuleDIO->IsFail;
			}
		}
		else
		{
			if (Chamber->OvenBoard)
			{
				Chamber->AlarmStatus[ERR_DIO_TIMEOUT] = Chamber->OvenBoard->IsFail;
			}
		}
		if (GetManager()->UseO2Analyzer && Chamber->O2Analyzer)
		{
			Chamber->AlarmStatus[ERR_O2ANALYZER_TIMEOUT] = Chamber->O2Analyzer->IsFail;
		}

		if (GetManager()->UseInverter && Chamber->Inverter)
		{
			Chamber->AlarmStatus[ERR_INVERTER_TIMEOUT] = Chamber->Inverter->IsFail;
			Chamber->AlarmStatus[ERR_INVERTER_TRIP_DETECTED] = Chamber->Inverter->IsTrip();
		}

		if (GetManager()->UseLocalN2Alarm) //2021-09-09 AMD China Local Mode N2 Alarm Check
		{
			if (Chamber->IsLocal)
			{
				if (GetManager()->UseN2PressureDetector)
				{
					Chamber->AlarmStatus[ERR_N2PRESS_NOT_DETECTED] = Chamber->DIO[I_N2_PRESSURE_DETECTED]->IsOff(10000);
				}
				if (GetManager()->UseN2FlowDetector)
				{
					Chamber->AlarmStatus[ERR_N2FLOW_NOT_DETECTED] = Chamber->DIO[GetManager()->I_N2_FLOW_DETECTED]->IsOff(10000);
				}
			}
		}

		if (Chamber->IsRun)
		{
			if (GetManager()->UseN2PressureDetector)
			{
				Chamber->AlarmStatus[ERR_N2PRESS_NOT_DETECTED] = Chamber->DIO[I_N2_PRESSURE_DETECTED]->IsOff(10000);
			}
			if (GetManager()->UseN2FlowDetector)
			{
				Chamber->AlarmStatus[ERR_N2FLOW_NOT_DETECTED] = Chamber->DIO[GetManager()->I_N2_FLOW_DETECTED]->IsOff(10000);
			}
			if (GetManager()->UseO2Analyzer && Chamber->O2Analyzer )
			{
				if (SecondsBetween(Chamber->Recipe.StartTime, Now()) >= Chamber->O2DensityAlarmEnableTime)
				{
					if (Chamber->O2Analyzer->O2Density > Chamber->O2DensityLimit &&
						Chamber->Step > STEP_IDLE && Chamber->Step < STEP_DONE)
					{
						Chamber->AlarmStatus[ERR_O2_DENSITY_ALARM] = true;
					}
					if (Chamber->CureStopOnO2Alarm &&
						Chamber->AlarmStatus[ERR_O2_DENSITY_ALARM])
					{
						Chamber->DoCureStop(FORCED_STOP);
					}
				}
			}
            //2023-04-17 restore
//			if (GetManager()->UseN2PurgeDetector)//2023-03-09 Change from flow meter to flow switch
//			{
//				for (int i=0; i<8; i++)
//				{
//					if(Chamber->Recipe.TempPtn.Seg[Chamber->SegNo-1].EvKind[i] == ET_PURGE)
//						Chamber->AlarmStatus[ERR_N2PURGE_FLOW_SWITCH_NOT_DETECCTED] = Chamber->DIO[I_N2_PURGE_FLOW_DETECTED]->IsOff(10000);
//				}
//			}
//			if (GetManager()->UseCDAFlowDetector)  //2023-04-17 restore
//			{
//		//		Chamber->AlarmStatus[ERR_CDA_FLOW_SWITCH_NOT_DETECCTED] = Chamber->DIO[I_CDA_FLOW_DETECTED]->IsOff(10000);
//			}

			// 강제종료 순간에 알람 발생 방지
			if (Chamber->Step > STEP_IDLE && Chamber->Step < STEP_DONE)
			{
				Chamber->AlarmStatus[ERR_DOOR_OPENED] = Chamber->DIO[I_DOOR_CLOSED]->IsOff(3000);
				if (Chamber->O2AnalyzerUseRunLimit)
				{
					if (Chamber->Step > STEP_PURGE && Chamber->Step < STEP_DONE)
					{
						Chamber->AlarmStatus[ERR_DOOR_UNLOCKED] = Chamber->IsDoorUnlocked();
                    }
				}
				else
				{
					Chamber->AlarmStatus[ERR_DOOR_UNLOCKED] = Chamber->IsDoorUnlocked();
				}
				if (Chamber->MainTempController)
				{
					if (Chamber->MainTempControllerType == TEMPCTRL_UP550 &&
						Chamber->Step == STEP_COOLING)
					{
						Chamber->AlarmStatus[ERR_PV_HIGH] = false;
						Chamber->AlarmStatus[ERR_PV_LOW] = false;
					}
					else
					{
						Chamber->AlarmStatus[ERR_PV_HIGH]	= Chamber->MainTempController->PVEventHigh;
						Chamber->AlarmStatus[ERR_PV_LOW]	= Chamber->MainTempController->PVEventLow;
					}
				   /*	int nHighIndex = -1;
					int nLowIndex = -1;
					// Index Search
					for (int i=0; i<8; i++)
					{
						if (Chamber->Recipe.TempPtn.Seg[Chamber->SegNo-1].EvKind[i] == ET_PVHIGH)
						{
							nHighIndex = i;
						}
						else
						if (Chamber->Recipe.TempPtn.Seg[Chamber->SegNo-1].EvKind[i] == ET_PVLOW)
						{
							nLowIndex = i;
						}
					}
					// Status Check
					if (nHighIndex >= 0 && nLowIndex >= 0)
					{
						if (nHighIndex > nLowIndex)
						{
							Chamber->AlarmStatus[ERR_PV_HIGH] = Chamber->MainTempController->PVEventLow;
							Chamber->AlarmStatus[ERR_PV_LOW] = Chamber->MainTempController->PVEventHigh;
						}
						else
						{
							Chamber->AlarmStatus[ERR_PV_HIGH] = Chamber->MainTempController->PVEventHigh;
							Chamber->AlarmStatus[ERR_PV_LOW] = Chamber->MainTempController->PVEventLow;
						}
					}
					else
					if (nHighIndex >= 0 && nLowIndex < 0)
					{
						Chamber->AlarmStatus[ERR_PV_HIGH] = Chamber->MainTempController->PVEventHigh;
						Chamber->AlarmStatus[ERR_PV_LOW] = false;
					}
					else
					if (nHighIndex < 0 && nLowIndex >= 0)
					{
						Chamber->AlarmStatus[ERR_PV_HIGH] = false;
						Chamber->AlarmStatus[ERR_PV_LOW] = Chamber->MainTempController->PVEventHigh;
					}
					else
					{
						Chamber->AlarmStatus[ERR_PV_HIGH] = false;
						Chamber->AlarmStatus[ERR_PV_LOW] = false;
					} */
				}
				else
				{
					Chamber->AlarmStatus[ERR_PV_HIGH] = false;
					Chamber->AlarmStatus[ERR_PV_LOW] = false;
				}

				if (GetManager()->AutomationKind == SECS_GEM_COWELL)
				{
					if (GetManager()->UseRemoteCheck == true)
					{
						if (GemDlg->GemState[0].nControlState == CONTROL_EQUIPMENT_OFFLINE)
						{
							Chamber->AlarmStatus[ERR_RUN_STATE_GEM_REMOTE_CHECK] = true;
						}
					}
				}
			}

			if (Chamber->AlarmStatus[ERR_EMERGENCY_SW])
			{
				Chamber->DoCureStop(EMERGENCY_STOP);
			}
		}

		if (Chamber->Step == STEP_IDLE && Chamber->DoorOpenDetectedWhenIdle/* && Chamber->IsDoorClosed() == false*/)
		{
			if (Chamber->DIO[I_GROUND_ALARM_DETECTED]->IsOn() && Chamber->IsDoorClosed() == false)
			{
				Chamber->AlarmStatus[ERR_IDLE_STATUS_DOOR_CLOSED_ALARM] = Chamber->CheckDoorOpenDetectedWhenIdle();
			}
			else
			{
				Chamber->DoorOpenWhenIdleStart = false;
            }
		}

		for (int i=0; i<MAX_ALARM_COUNT; i++)
		{
			if (Chamber->AlarmStatus[i])
			{
				if (Chamber->SetAlarm(i))
				{
					// 프로그램 크래시 방지를 위해 메시지 방식으로 대체한다.
					PostMessage(Application->MainFormHandle, WM_GEM_ALARM_SET, Chamber->ChamberID, i);
//						if( GetManager()->UseAutomation )
//						{
//							if( GetManager()->AutomationKind == SECS_GEM &&
//								GemDlg )
//							{
//								GemDlg->SetAlarm(Chamber->ChamberID, i);
//							}
//							else
//							if( GetManager()->AutomationKind == BARUN_MES &&
//								Chamber->UseManualMode == false &&
//								BarunMESDlg->CimParam.OnlineMode == true &&
//								Chamber->BarunManager )
//							{
//								Chamber->BarunManager->SendAlarm(Chamber->OperatorID, i);
//							}
//						}
//
//						DB_ERROR error;
//						if( GetOvenDB()->GetError(i, error, GetManager()->Language) )
//							GetOvenDB()->SaveLog(error.Name, ALARM, Chamber->ChamberID);
				}
			}
		}
	}
}

NormalOvenChamber::NormalOvenChamber(int chamberNo)
	: OvenChamber(chamberNo)
{
	DoNotChangeStep = false;
	FInitCompleted = false;
	FCannotStartWithoutLotInfo = false;
	FHeatingWithoutN2 = false;
	FAllowRecipeSelection = false;
    FDoorOpenDetectedWhenIdle = false;
	FGotoIdleInLocal = true;
	FRecipeMaintain = false;
	FIsAbleAutoStart = true;
	FUseManualMode = false;
	// 20180305 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
	IsO2AlarmStartTimeSet = false;
	FO2AnalyzerUseRunLimit = false;
	// 20180313 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
	FO2DensityRunLimit = 100;
	FLargeN2ValveOpenO2Density = 200;
	FO2DensityAlarmRunLimit = 10;
	FIsO2DensityRunLimitAlarm = false;
    FDoorOpenDetectedInterval = 2;

	IsDoorLockTempTimeSet = false;

	CheckResetCount = 0;	// 2022-02-14

	FDPController = nullptr;
	FModuleDI = nullptr;
	FModuleDO = nullptr;
	FModuleDIO = nullptr;
	O2Analyzer = nullptr;
	FOvenBoard = nullptr;
	FInverter = nullptr;
	FREX_P250 = nullptr;

	WatchSystemThread = nullptr;

	FStep = -1;

	FO2DensityAlarmEnableTime = 1200;
	FO2DensityLimit = 100;
	FCureStopOnO2Alarm = false;

	FDoBuzzerJobEnd = false;
	FDoBuzzerAlarm = false;
	FDoBuzzerOffSwitch = false;
	FDoTowerLampRed = false;

	DeviceRemoved = false;

	FIdleCoolingTemp = 40.0;
	FIdleCoolingOffset = 3.0;
	FIdleCoolingStarted = false;
	FUseIdleCooling = false;
    IdleCoolingDisabled = false;
	IdleCoolingDoorOpened = false;

	FHanaCureComplete = false;
	FHanaCureFinishStatus = false;
	FIsBlink = false;
	LotIDList = new TStringList;

	if( GetManager()->UseAutomation )
	{
		int kind = GetManager()->AutomationKind;
		if( kind == BARUN_MES )
		{
			BarunManager.reset(new BarunCommManager(this));
		}
	}

	for (int i = 0; i < 5; i++)
	{
		UseChannel[i] = true;
	}

	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++) //2021-08-12 hcw.
	{
		TempLoggerData[i] = 0.0;
		FIsLoggerDeviationAlarm[i] =  false;
	}

	CowellLotInfoClear();

	CreateDIO();
	DeviceIDList = new TStringList;
	MgzDeviceIDList = new TStringList;
	JsckPvDataList.clear();
	JsckGemParamClear();
}

__fastcall NormalOvenChamber::~NormalOvenChamber()
{
	if( FBarunManager )
		FBarunManager->Disconnect();

	Terminate();
	Sleep(100);

	if( WatchSystemThread )
	{
		WatchSystemThread->Terminate();
		Sleep(100);
		delete WatchSystemThread;
	}

	DestroyDIO();

	if (LotIDList)
	{
		delete LotIDList;
	}

	if (MgzDeviceIDList)
	{
		delete MgzDeviceIDList;
    }
}

void NormalOvenChamber::RunWatchAlarm()
{
	WatchSystemThread = new WatchSystemStatus(this);
}

bool NormalOvenChamber::LoadChamberParameters()
{
	AnsiString sec, key, str;
	String iniPath = GetDataFolder() + OVENSYSTEMINI;
	std::auto_ptr<TIniFile> SysFile(new TIniFile(iniPath));

	sec = ChamberStr[FChamberID];

	try
	{
		if( SysFile->ValueExists(sec, KEY_MAINTEMPCTRLTYPE) )
			FMainTempControllerType = SysFile->ReadInteger(sec, KEY_MAINTEMPCTRLTYPE, FMainTempControllerType);
		else
			SysFile->WriteInteger(sec, KEY_MAINTEMPCTRLTYPE, FMainTempControllerType);

		if( SysFile->ValueExists(sec, KEY_LIMITCTRLTYPE) )
			FLimitControllerType = SysFile->ReadInteger(sec, KEY_LIMITCTRLTYPE, FLimitControllerType);
		else
			SysFile->WriteInteger(sec, KEY_LIMITCTRLTYPE, FLimitControllerType);

		if( SysFile->ValueExists(sec, KEY_CHAMBERTEMPLIMITOFFSET) )
			FChamberTempLimitOffset = SysFile->ReadInteger(sec, KEY_CHAMBERTEMPLIMITOFFSET, FChamberTempLimitOffset);
		else
			SysFile->WriteInteger(sec, KEY_CHAMBERTEMPLIMITOFFSET, FChamberTempLimitOffset);

		if( SysFile->ValueExists(sec, KEY_CHAMBERTEMPLIMITBIAS) )
			FChamberTempLimitBias = SysFile->ReadInteger(sec, KEY_CHAMBERTEMPLIMITBIAS, FChamberTempLimitBias);
		else
			SysFile->WriteInteger(sec, KEY_CHAMBERTEMPLIMITBIAS, FChamberTempLimitBias);

		if( SysFile->ValueExists(sec, KEY_CANNOTSTARTWITHOUTLOTINFO) )
			FCannotStartWithoutLotInfo = SysFile->ReadBool(sec, KEY_CANNOTSTARTWITHOUTLOTINFO, FCannotStartWithoutLotInfo);
		else
			SysFile->WriteBool(sec, KEY_CANNOTSTARTWITHOUTLOTINFO, FCannotStartWithoutLotInfo);

		if( SysFile->ValueExists(sec, KEY_ALLOW_RECIPE_SELECTION) )
			FAllowRecipeSelection = SysFile->ReadBool(sec, KEY_ALLOW_RECIPE_SELECTION, FAllowRecipeSelection);
		else
			SysFile->WriteBool(sec, KEY_ALLOW_RECIPE_SELECTION, FAllowRecipeSelection);

		if (!(GetManager()->UseGroundAlarmDetector))
		{
			if( SysFile->ValueExists(sec, KEY_DOOR_OPEN_DETECTED_WHEN_IDLE) )
				FDoorOpenDetectedWhenIdle = SysFile->ReadBool(sec, KEY_DOOR_OPEN_DETECTED_WHEN_IDLE, FDoorOpenDetectedWhenIdle);
            else
				SysFile->WriteBool(sec, KEY_DOOR_OPEN_DETECTED_WHEN_IDLE, FDoorOpenDetectedWhenIdle);

			if( SysFile->ValueExists(sec, KEY_DOOR_OPEN_DETECTED_INTERVAL))
				FDoorOpenDetectedInterval = SysFile->ReadInteger(sec, KEY_DOOR_OPEN_DETECTED_INTERVAL, FDoorOpenDetectedInterval);
			else
				SysFile->WriteInteger(sec, KEY_DOOR_OPEN_DETECTED_INTERVAL, FDoorOpenDetectedInterval);
		}

		if( SysFile->ValueExists(sec, KEY_GOTO_IDLE_IN_LOCAL) )
			FGotoIdleInLocal = SysFile->ReadBool(sec, KEY_GOTO_IDLE_IN_LOCAL, FGotoIdleInLocal);
		else
			SysFile->WriteBool(sec, KEY_GOTO_IDLE_IN_LOCAL, FGotoIdleInLocal);

		if( SysFile->ValueExists(sec, KEY_MAINTAIN_RECIPE) )
			FRecipeMaintain = SysFile->ReadBool(sec, KEY_MAINTAIN_RECIPE, FRecipeMaintain);
		else
			SysFile->WriteBool(sec, KEY_MAINTAIN_RECIPE, FRecipeMaintain);

		if( SysFile->ValueExists(sec, KEY_O2_DENSITY_ALARM_ENABLE_TIME) )
			FO2DensityAlarmEnableTime = SysFile->ReadInteger(sec, KEY_O2_DENSITY_ALARM_ENABLE_TIME, FO2DensityAlarmEnableTime);
		else
			SysFile->WriteInteger(sec, KEY_O2_DENSITY_ALARM_ENABLE_TIME, FO2DensityAlarmEnableTime);

		if( SysFile->ValueExists(sec, KEY_CURE_STOP_ON_O2_DENSITY_ALARM) )
			FCureStopOnO2Alarm = SysFile->ReadBool(sec, KEY_CURE_STOP_ON_O2_DENSITY_ALARM, FCureStopOnO2Alarm);
		else
			SysFile->WriteBool(sec, KEY_CURE_STOP_ON_O2_DENSITY_ALARM, FCureStopOnO2Alarm);

		// 20180305 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
		if( SysFile->ValueExists(sec, KEY_USE_O2_ANALYZER_RUN_LIMIT) )
			FO2AnalyzerUseRunLimit = SysFile->ReadBool(sec, KEY_USE_O2_ANALYZER_RUN_LIMIT, FO2AnalyzerUseRunLimit);
		else
			SysFile->WriteBool(sec, KEY_USE_O2_ANALYZER_RUN_LIMIT, FO2AnalyzerUseRunLimit);

		// 20180313 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
		if( SysFile->ValueExists(sec, KEY_O2_DENSITY_RUN_LIMIT) )
			FO2DensityRunLimit = SysFile->ReadInteger(sec, KEY_O2_DENSITY_RUN_LIMIT, FO2DensityRunLimit);
		else
			SysFile->WriteInteger(sec, KEY_O2_DENSITY_RUN_LIMIT, FO2DensityRunLimit);

		// 20180313 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
		if( SysFile->ValueExists(sec, KEY_O2_DENSITY_ALARM_ENABLE_TIME1) )
			FO2DensityAlarmRunLimit = SysFile->ReadInteger(sec, KEY_O2_DENSITY_ALARM_ENABLE_TIME1, FO2DensityAlarmRunLimit);
		else
			SysFile->WriteInteger(sec, KEY_O2_DENSITY_ALARM_ENABLE_TIME1, FO2DensityAlarmRunLimit);

		if( SysFile->ValueExists(sec, KEY_HEATING_WITHOUT_N2) )
			FHeatingWithoutN2 = SysFile->ReadBool(sec, KEY_HEATING_WITHOUT_N2, FHeatingWithoutN2);
		else
			SysFile->WriteBool(sec, KEY_HEATING_WITHOUT_N2, FHeatingWithoutN2);

		if( SysFile->ValueExists(sec, KEY_O2_DENSITY_LIMIT) )
			FO2DensityLimit = SysFile->ReadInteger(sec, KEY_O2_DENSITY_LIMIT, FO2DensityLimit);
		else
			SysFile->WriteInteger(sec, KEY_O2_DENSITY_LIMIT, FO2DensityLimit);

		if( SysFile->ValueExists(sec, KEY_LARGE_N2VALVE_OPEN_O2_DENSITY) )
			FLargeN2ValveOpenO2Density = SysFile->ReadInteger(sec, KEY_LARGE_N2VALVE_OPEN_O2_DENSITY, FLargeN2ValveOpenO2Density);
		else
			SysFile->WriteInteger(sec, KEY_LARGE_N2VALVE_OPEN_O2_DENSITY, FLargeN2ValveOpenO2Density);

		if( SysFile->ValueExists(sec, KEY_AUTO_START) )
			FIsAbleAutoStart = SysFile->ReadBool(sec, KEY_AUTO_START, FIsAbleAutoStart);
		else
			SysFile->WriteBool(sec, KEY_AUTO_START, FIsAbleAutoStart);

		if( SysFile->ValueExists(sec, KEY_USE_MANUAL_MODE) )
			FUseManualMode = SysFile->ReadBool(sec, KEY_USE_MANUAL_MODE, FUseManualMode);
		else
			SysFile->WriteBool(sec, KEY_USE_MANUAL_MODE, FUseManualMode);

		for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
		{
			key.sprintf("Ch%d Offset", i+1);
			if( SysFile->ValueExists(sec, key) )
				FLoggerOffset[i] = SysFile->ReadInteger(sec, key, FLoggerOffset[i]);
			else
				SysFile->WriteInteger(sec, key, FLoggerOffset[i]);

			key.sprintf("Ch%d Use", i+1);
			if( SysFile->ValueExists(sec, key) )
				FUseChannel[i] = SysFile->ReadInteger(sec, key, FUseChannel[i]);
			else
				SysFile->WriteInteger(sec, key, FUseChannel[i]);
		}

		if( SysFile->ValueExists(sec, KEY_MACHINEID) )
			FMachineID = SysFile->ReadString(sec, KEY_MACHINEID, FMachineID);
		else
			SysFile->WriteString(sec, KEY_MACHINEID, FMachineID);

		if( SysFile->ValueExists(sec, KEY_USE_IDLE_COOLING) )
			FUseIdleCooling = SysFile->ReadBool(sec, KEY_USE_IDLE_COOLING, FUseIdleCooling);
		else
			SysFile->WriteBool(sec, KEY_USE_IDLE_COOLING, FUseIdleCooling);

		if( SysFile->ValueExists(sec, KEY_IDLECOOLINGTEMP) )
			FIdleCoolingTemp = SysFile->ReadFloat(sec, KEY_IDLECOOLINGTEMP, FIdleCoolingTemp);
		else
		{
			str.sprintf("%.1f", FIdleCoolingTemp);
			SysFile->WriteString(sec, KEY_IDLECOOLINGTEMP, str);
		}

		if( SysFile->ValueExists(sec, KEY_IDLECOOLINGOFFSET) )
			FIdleCoolingOffset = SysFile->ReadFloat(sec, KEY_IDLECOOLINGOFFSET, FIdleCoolingOffset);
		else
		{
			str.sprintf("%.1f", FIdleCoolingOffset);
			SysFile->WriteString(sec, KEY_IDLECOOLINGOFFSET, str);
		}
		//-2021-08-10 hcw
		if( SysFile->ValueExists(sec, KEY_CHAMBERLOGGERDEVIATION) )
			FChamberLoggerDeviation = SysFile->ReadInteger(sec, KEY_CHAMBERLOGGERDEVIATION, FChamberLoggerDeviation);
		else
			SysFile->WriteInteger(sec, KEY_CHAMBERLOGGERDEVIATION, FChamberLoggerDeviation);

		if( SysFile->ValueExists(sec, KEY_DOOROPENCONDITIONTEMP) )
			FDoorOpenConditionTemp = SysFile->ReadFloat(sec, KEY_DOOROPENCONDITIONTEMP, FDoorOpenConditionTemp);
		else
		{
			str.sprintf("%.1f", FDoorOpenConditionTemp);
			SysFile->WriteString(sec, KEY_DOOROPENCONDITIONTEMP, str);
		}

		if( SysFile->ValueExists(sec, KEY_WORKCOMPLETIONTEMP) )
			FWorkCompletionTemp = SysFile->ReadFloat(sec, KEY_WORKCOMPLETIONTEMP, FWorkCompletionTemp);
		else
		{
			str.sprintf("%.1f", FWorkCompletionTemp);
			SysFile->WriteString(sec, KEY_WORKCOMPLETIONTEMP, str);
		}

		//2021-09-29 Fujitsu
		if( SysFile->ValueExists(sec, KEY_CHAMBER_SELECT_BACODE) )
			FChamberSelect = SysFile->ReadString(sec, KEY_CHAMBER_SELECT_BACODE, FChamberSelect);
		else
			SysFile->WriteString(sec, KEY_CHAMBER_SELECT_BACODE, FChamberSelect);

		if( SysFile->ValueExists(sec, KEY_TCARD_START_CURE_BACODE) )
			FTCardStartCure = SysFile->ReadString(sec, KEY_TCARD_START_CURE_BACODE, FTCardStartCure);
		else
			SysFile->WriteString(sec, KEY_TCARD_START_CURE_BACODE, FTCardStartCure);

	}
	catch (Exception* e)
	{
		SetMessageStr(e->Message);
		ShowMessageDlg();
		return false;
	}

	return true;
}

bool NormalOvenChamber::SaveChamberParameters()
{
	AnsiString sec, key, str;
	String iniPath = GetDataFolder() + OVENSYSTEMINI;
	std::auto_ptr<TIniFile> SysFile(new TIniFile(iniPath));

	sec = ChamberStr[FChamberID];
	try
	{
		SysFile->WriteInteger(sec, KEY_MAINTEMPCTRLTYPE, FMainTempControllerType);
		SysFile->WriteInteger(sec, KEY_LIMITCTRLTYPE, FLimitControllerType);

		SysFile->WriteInteger(sec, KEY_CHAMBERTEMPLIMITOFFSET, FChamberTempLimitOffset);
		SysFile->WriteInteger(sec, KEY_CHAMBERTEMPLIMITBIAS, FChamberTempLimitBias);
		SysFile->WriteBool(sec, KEY_CANNOTSTARTWITHOUTLOTINFO, FCannotStartWithoutLotInfo);
		SysFile->WriteBool(sec, KEY_ALLOW_RECIPE_SELECTION, FAllowRecipeSelection);
		SysFile->WriteBool(sec, KEY_DOOR_OPEN_DETECTED_WHEN_IDLE, FDoorOpenDetectedWhenIdle);
		SysFile->WriteInteger(sec, KEY_DOOR_OPEN_DETECTED_INTERVAL, FDoorOpenDetectedInterval);
		SysFile->WriteBool(sec, KEY_GOTO_IDLE_IN_LOCAL, FGotoIdleInLocal);
		SysFile->WriteBool(sec, KEY_MAINTAIN_RECIPE, FRecipeMaintain);

		SysFile->WriteInteger(sec, KEY_O2_DENSITY_ALARM_ENABLE_TIME, FO2DensityAlarmEnableTime);
		SysFile->WriteBool(sec, KEY_CURE_STOP_ON_O2_DENSITY_ALARM, FCureStopOnO2Alarm);
		SysFile->WriteInteger(sec, KEY_O2_DENSITY_LIMIT, FO2DensityLimit);
		SysFile->WriteBool(sec, KEY_HEATING_WITHOUT_N2, FHeatingWithoutN2);
		SysFile->WriteBool(sec, KEY_AUTO_START, FIsAbleAutoStart);
		SysFile->WriteString(sec, KEY_MACHINEID, FMachineID);
		SysFile->WriteBool(sec, KEY_USE_MANUAL_MODE, FUseManualMode);

		// 20180305 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
		SysFile->WriteBool(sec, KEY_USE_O2_ANALYZER_RUN_LIMIT, FO2AnalyzerUseRunLimit);
		// 20180313 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
		SysFile->WriteInteger(sec, KEY_O2_DENSITY_RUN_LIMIT, FO2DensityRunLimit);
		SysFile->WriteInteger(sec, KEY_LARGE_N2VALVE_OPEN_O2_DENSITY, FLargeN2ValveOpenO2Density);
		SysFile->WriteInteger(sec, KEY_O2_DENSITY_ALARM_ENABLE_TIME1, FO2DensityAlarmRunLimit);

		for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
		{
			key.sprintf("Ch%d Offset", i+1);
			SysFile->WriteInteger(sec, key, FLoggerOffset[i]);

			key.sprintf("Ch%d Use", i+1);
			SysFile->WriteInteger(sec, key, FUseChannel[i]);
		}

 		SysFile->WriteBool(sec, KEY_USE_IDLE_COOLING, FUseIdleCooling);
		str.sprintf("%.1f", FIdleCoolingTemp);
        SysFile->WriteString(sec, KEY_IDLECOOLINGTEMP, str);
		str.sprintf("%.1f", FIdleCoolingOffset);
		SysFile->WriteString(sec, KEY_IDLECOOLINGOFFSET, str);
		//-2021-08-09 hcw
		SysFile->WriteInteger(sec, KEY_CHAMBERLOGGERDEVIATION, FChamberLoggerDeviation);
		str.sprintf("%.1f", FDoorOpenConditionTemp);
		SysFile->WriteString(sec, KEY_DOOROPENCONDITIONTEMP, str);
		str.sprintf("%.1f", FWorkCompletionTemp);
		SysFile->WriteString(sec, KEY_WORKCOMPLETIONTEMP, str);
		//2021-09-29 Fujitsu
		SysFile->WriteString(sec, KEY_CHAMBER_SELECT_BACODE, FChamberSelect);
		SysFile->WriteString(sec, KEY_TCARD_START_CURE_BACODE, FTCardStartCure);
	}
	catch (Exception* e)
	{
		SetMessageStr(e->Message);
		ShowMessageDlg();
		return false;
	}

	return true;
}

void NormalOvenChamber::DoDamperControl()
{
	if( GetManager()->DamperControlType == CYLINDER_DAMPER )
		return;

	if( FMainTempController == nullptr ||
		FMainTempController->IsFail )
		return;

	if( FDamperMotor == nullptr ||
		FDamperMotor->IsFail )
		return;

	if( FStep == STEP_DONE )
	{
		DamperMotor->Close();
	}
	else
	{
		if( SegNo <= 0 )
			return;

		DamperMotor->Open(Recipe.TempPtn.Damper[SegNo-1].InPercent);
	}
}

void NormalOvenChamber::DoInverterControl()
{
	if( GetManager()->UseInverter == false )
		return;

	if( FMainTempController == nullptr ||
		FMainTempController->IsFail )
		return;

	if( FInverter == nullptr ||
		FInverter->IsFail )
		return;

	if( SegNo <= 0 )
		return;

	double setFreq = Recipe.TempPtn.InverterFrequency[SegNo-1];
	double currFreq = FInverter->GetFrequency();
	if (setFreq != currFreq)
	{
		FInverter->SetRunFreq(setFreq);
		Sleep(1000);
	}
}

void NormalOvenChamber::OpenN2ValveAll()
{
	DIO[O_N2_LARGE]->On();
	DIO[O_N2_MEDIUM]->On();
	DIO[O_N2_SMALL]->On();
}

void NormalOvenChamber::CloseN2ValveAll()
{
	DIO[O_N2_LARGE]->Off();
	DIO[O_N2_MEDIUM]->Off();
	DIO[O_N2_SMALL]->Off();
}

void NormalOvenChamber::OpenN2LargeValveOnly()
{
	DIO[O_N2_LARGE]->On();
	DIO[O_N2_MEDIUM]->Off();
	DIO[O_N2_SMALL]->Off();
}

void NormalOvenChamber::OpenN2MediumValveOnly()
{
	DIO[O_N2_LARGE]->Off();
	DIO[O_N2_MEDIUM]->On();
	DIO[O_N2_SMALL]->Off();
}

void NormalOvenChamber::OpenN2SmallValveOnly()
{
	DIO[O_N2_LARGE]->Off();
	DIO[O_N2_MEDIUM]->Off();
	DIO[O_N2_SMALL]->On();
}

// ANST PO SPEC.
void NormalOvenChamber::DoN2FlowControl()
{
	if (GetManager()->UseN2FlowControl == false)
	{
		return;
	}
	if (GetManager()->UseO2Analyzer == false)
	{
		return;
	}
	if (O2Analyzer == nullptr ||
		O2Analyzer->IsFail)
	{
		return;
	}
	if (FInitCompleted == false)
	{
		return;
    }

	// idle 상태에서는 모든 밸브를 닫는다.
	if (Step == STEP_IDLE)
	{
		CloseN2ValveAll();
		return;
	}

	// 퍼지구간에서는 큰 밸브만 연다.
	if (Step == STEP_PURGE)
	{
		OpenN2LargeValveOnly();
		return;
	}

	// 작업이 끝난 상황이면 작은 밸브만 연다.
	if (Step == STEP_DONE)
	{
		OpenN2SmallValveOnly();
		return;
	}

	if (O2Analyzer->O2Density > LargeN2ValveOpenO2Density)
	{
		OpenN2LargeValveOnly();
	}
	else
	{
		OpenN2MediumValveOnly();
	}
}

// 2021-06-09 KJH 쿨링워터 배출
void NormalOvenChamber::DoWaterExaust()
{
	DIO[O_COOLING_WATER_VENT]->On();
	DelayTime(GetManager()->CoolingVentExuastTime*1000, false);
	DIO[O_COOLING_WATER_VENT]->Off();
}

void NormalOvenChamber::DoWorkSequence()
{
	if (FInitCompleted == false)
	{
		return;
	}
	DoHanaProtocalSeq(); //2021-04-13 HANA OVEN
	DoLoggerTempDeviation(); //2021-08-11 hcw.
	DoDoorLockControl();
	DoHeaterProblemLampControl();

	switch (FStep)
	{
	case STEP_IDLE:
		LogStepChanged();
		DoIdleCooling();
		break;

    // 20180305 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
	case STEP_PURGE:
		LogStepChanged();

		if (GetManager()->UseO2Analyzer && O2Analyzer)
		{
			if (O2AnalyzerUseRunLimit) //2022-05-18 Temp Controller state Alarm Check.
			{
				AlarmStatus[ERR_TEMP_CONTROLLER_NOT_HOLD] = (IsHold == false);
			}
		}
		else
		{
			AlarmStatus[ERR_TEMP_CONTROLLER_NOT_RUN] = (IsRun == false);
		}

		IdleCoolingDisabled = false;
		IdleCoolingDoorOpened = false;

		if( IsO2DensityRunLimitAlarm == false )
		{
			if( CheckO2DensityforCureStart() )
			{
				if( FMainTempController )
				{
					FMainTempController->Resume();
					Sleep(500);
				}

				SetStep(STEP_RUNNING);
				break;
			}
		}
		break;

	case STEP_RUNNING:
		LogStepChanged();

        IdleCoolingDisabled = false;
		IdleCoolingDoorOpened = false;

		if (DoNotChangeStep == false)
		{
			AlarmStatus[ERR_TEMP_CONTROLLER_NOT_RUN] = (IsRun == false);

			if (IsLocal)
			{
				if (GetManager()->LoginLevel > 0)
				{
					SetStep(STEP_LOCAL);
				}
			}
			else if (IsCoolingEventOn)
			{
				SetStep(STEP_COOLING);
			}
		}
		break;

	case STEP_COOLING:
		LogStepChanged();

		if (FMainTempController->RemainingTime > 1)
		{
			AlarmStatus[ERR_TEMP_CONTROLLER_NOT_RUN] = (IsRun == false);
		}

		IdleCoolingDisabled = false;
		IdleCoolingDoorOpened = false;

		if (DoNotChangeStep == false)
		{
			if (GetManager()->LoginLevel > 0)
			{
				if (WorkCompletionTemp > 0)
				{
				   if (PV < WorkCompletionTemp)
				   {
					   DoCureStop(FORCED_STOP);
				   }
				}
				if (IsReset)
				{
					SetStep(STEP_DONE);
				}
				else if (IsLocal)
				{
					SetStep(STEP_LOCAL);
				}
			}
		}
		break;

	case STEP_LOCAL:
		LogStepChanged();

		IdleCoolingDisabled = false;
		IdleCoolingDoorOpened = false;
		if (DoNotChangeStep == false)
		{
			if (GetManager()->LoginLevel > 0)
			{
				if (IsReset)
				{
					SetStep(STEP_DONE);
				}
				else if (FGotoIdleInLocal && IsDoorClosed() == false)
				{
					MainTempController->Reset();
					DeviceRemoved = true;
					SetStep(STEP_IDLE);
					DeleteStateFile();
				}
				CheckResetCount++;
			}
		}
		Sleep(500);
		break;

	case STEP_DONE:
		LogStepChanged();

		DoIdleCooling();
		if (DoorOpenConditionTemp > 0)
		{
		  if (PV > DoorOpenConditionTemp)
		  {
			 AlarmStatus[ERR_DOOR_OPEN_TEMP_CONDITION_ALARM] = true;
		  }
		}

		if (IsDoorClosed() == false && IsMagazineDetected() == false)
		{
			DeviceRemoved = true;
			SetStep(STEP_IDLE);
			DeleteStateFile();
		}
		break;
	}
}

void NormalOvenChamber::ApplyDataToController()
{
	if( FChamberTempLimitController )
	{
		FChamberTempLimitController->SP = Recipe.TempPtn.MaxTemp + ChamberTempLimitOffset;
		FChamberTempLimitController->Limit1 = Recipe.TempPtn.MaxTemp;
		FChamberTempLimitController->Limit2 = Recipe.TempPtn.MaxTemp;
		FChamberTempLimitController->LimitOffset = ChamberTempLimitOffset;
	}
}

bool NormalOvenChamber::DoCureStart(AnsiString &returnMsg, bool byRemote/*=false*/)
{
	ProgressValue = 0;

	DoIdleCoolingDisable();

	if( FMainTempController == nullptr )
		return false;

	if (byRemote && GetManager()->AutomationKind != SECS_GEM_SCK)
		Recipe.TempPtn.PtnNo = 1;

	if (byRemote == false || (GetManager()->UseAutomation &&
		(GetManager()->IsUseGem() == false && GetManager()->AutomationKind != HANA_CIM &&
		GetManager()->AutomationKind != SECS_GEM_SCK)))
	{
		if( LoadTempPtn(FChamberID, Recipe.TempPtn.PtnNo, Recipe.TempPtn, byRemote) == false )
		{
			returnMsg.sprintf("Pattern data load fail. [PTN No=%d]", Recipe.TempPtn.PtnNo);
			return false;
		}
	}

	ApplyDataToController();

	DoNotChangeStep = true;

	if( FMainTempController->Reset() )
		ProgressValue++;
	else
	{
		returnMsg = "Main temp. controller reset fail.";
		return false;
	}
	if( FMainTempController->DeletePattern(Recipe.TempPtn.PtnNo) )
		ProgressValue++;
	else
	{
		returnMsg.sprintf("Main temp. controller delete fail. [PTN No=%d]", Recipe.TempPtn.PtnNo);
		return false;
	}

	if( FMainTempController->WriteSTC(Recipe.TempPtn) )
	{
		// 2020-01-22 yjyoo, 다운로드한 패턴을 다시 읽어 제대로 전송되었는지 확인한다.
		// Read STC
		if( FMainTempController->ReadSTC(Recipe.TempPtn.PtnNo) )
		{
			// Compare STC
			if( FMainTempController->CompareSTC(Recipe.TempPtn, returnMsg) )
			{
				ProgressValue++;
			}
			else
			{
				return false;
			}
		}
		else
		{
			returnMsg.sprintf("Main temp. controller STC read fail. [PTN No=%d]", Recipe.TempPtn.PtnNo);
			return false;
		}
	}
	else
	{
		returnMsg.sprintf("Main temp. controller STC write fail. [PTN No=%d]", Recipe.TempPtn.PtnNo);
		return false;
	}

	for(int i=0; i<Recipe.TempPtn.Stc.SegCount; i++)
	{
		if( FMainTempController->WriteSeg(i, Recipe.TempPtn) )
		{
			// 2020-01-22 yjyoo, 다운로드한 패턴을 다시 읽어 제대로 전송되었는지 확인한다.
			// Read SEG
			if( FMainTempController->ReadSeg(Recipe.TempPtn.PtnNo, i) )
			{
				// Compare SEG
				if( FMainTempController->CompareSeg(i, Recipe.TempPtn, returnMsg) )
				{
					ProgressValue++;
				}
				else
				{
					return false;
				}
			}
			else
			{
				returnMsg.sprintf("Main temp. controller SEG read fail. [PTN No=%d, SEG No=%d]", Recipe.TempPtn.PtnNo, i+1);
				return false;
			}
		}
		else
		{
			returnMsg.sprintf("Main temp. controller SEG write fail. [PTN No=%d, SEG No=%d]", Recipe.TempPtn.PtnNo, i+1);
			return false;
		}
	}

	if( FMainTempController->Run(Recipe.TempPtn.PtnNo) )
	{
		// 20180305 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
		if( GetManager()->UseO2Analyzer && O2AnalyzerUseRunLimit )
		{
			Sleep(200);
			if( FMainTempController->Hold() == false )
			{
				returnMsg .sprintf("Main temp. controller HOLD fail. [PTN No=%d]", Recipe.TempPtn.PtnNo);
				return false;
			}
		}
		ProgressValue++;
	}
	else
	{
		returnMsg.sprintf("Main temp. controller RUN fail. [PTN No=%d]", Recipe.TempPtn.PtnNo);
		return false;
	}

	WaitSecond(5);
	InitCureValues();
	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
	{
		FIsLoggerDeviationAlarm[i] =  false;
	}
	DeviceRemoved = false;

	// 2021-06-09 KJH 쿨링워터 배출
	if( GetManager()->CoolingVentExuastTime > 0 )
	{
		DoWaterExaust();
	}

	// 20180305 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
	if( GetManager()->UseO2Analyzer && O2AnalyzerUseRunLimit )
	{
		IsO2AlarmStartTimeSet = false;
		IsO2DensityRunLimitAlarm = false;
		SetStep(STEP_PURGE);
	}
	else
	{
		String str = "Change to running step";
		GetOvenDB()->SaveLog(str, MESSAGES, ChamberID);
		SetStep(STEP_RUNNING);
		//2021-04-15 HANA OVEN
		if( GetManager()->AutomationKind == HANA_CIM && GetManager()->UseAutomation == true)
		{
			SaveHanaOvenPtn(ChamberID, Recipe.TempPtn, true);
		}
	}

	WaitSecond(1);
	DoNotChangeStep = false;

	return true;
}

bool NormalOvenChamber::DoCureStop(int stopType)
{
	Recipe.StopType = stopType;
	if (Recipe.StopType == EMERGENCY_STOP)
	{
		if (GetManager()->EmoOption == STOP_WORKING)
		{
			ForcedCureStop();
		}
	}
	else if (Recipe.StopType == LOCALMODE_STOP)
	{
		SetStep(STEP_IDLE);
		MainTempController->Reset();
    }
	else
	{
		if (Recipe.TempPtn.UseLocalMode)
		{
			DoNotChangeStep = true;		// 온도컨트롤러가 리셋되면 스텝이 변경되지 않도록 막는다.
			Sleep(1000);

			if (MainTempController->Reset())
			{
				int count = 0;
				bool ret = true;
				while(IsReset == false)
				{
					Sleep(100);
					Application->ProcessMessages();
					count++;
					if (count > 1800)
					{
						ret = false;
						break;
					}
				}
				if (ret == false)
				{
					return false;
				}
			}
			Sleep(1000);
			if (MainTempController->Local())
			{
				int count = 0;
				bool ret = true;
				while(IsLocal == false)
				{
					Sleep(100);
					Application->ProcessMessages();
					count++;
					if (count > 1800)
					{
						ret = false;
						break;
					}
				}
				if (ret == false)
				{
					return false;
				}
			}

			SetStep(STEP_LOCAL);
			Sleep(1000);
			DoNotChangeStep = false;
		}
		else
		{
			ForcedCureStop();
		}
	}

	return true;
}

bool NormalOvenChamber::SaveCurrentState()
{
	String strFile, str;
	strFile.sprintf(L"%s%s%c\\Current_T.ini",
					GetRootFolder().c_str(),
					L"PATTERN_CM_",
					'A'+FChamberID);

	const std::auto_ptr<TIniFile> statFile(new TIniFile(strFile));
	const String sec = "STATE";

	try
	{
		statFile->WriteDateTime(sec, "Last Save Time", Now());
		statFile->WriteInteger(sec, "Step", FStep);
		statFile->WriteString(sec, "Recipe", Recipe.RecipeName);
		statFile->WriteString(sec, "UserID", GetManager()->UserID);
		statFile->WriteDateTime(sec, "Start Time", Recipe.StartTime);
		statFile->WriteBool(sec, "Job End", Recipe.JobEnd);
		statFile->WriteInteger(sec, "Stop Type", Recipe.StopType);
		statFile->WriteBool(sec, "GEM Recipe", Recipe.IsGemRecipe);
	}
	catch (Exception* e)
	{
		return false;
	}

	return true;
}

bool NormalOvenChamber::LoadCurrentState()
{
	String strFile;
	strFile.sprintf(L"%s%s%c\\Current_T.ini",
					GetRootFolder().c_str(),
					L"PATTERN_CM_",
					'A'+FChamberID);

	if (FileExists(strFile) == false)
	{
		FStep = STEP_IDLE;
		return false;
	}

	const std::auto_ptr<TIniFile> statFile(new TIniFile(strFile));
	const String sSection = "STATE";

	try
	{
		FStep = statFile->ReadInteger(sSection, "Step", FStep);
		Recipe.RecipeName = statFile->ReadString(sSection, "Recipe", Recipe.RecipeName);
		GetManager()->UserID = statFile->ReadString(sSection, "User ID", GetManager()->UserID);
		Recipe.StartTime = statFile->ReadDateTime(sSection, "Start Time", Recipe.StartTime);
		Recipe.JobEnd = statFile->ReadBool(sSection, "Job End", Recipe.JobEnd);
		Recipe.StopType = statFile->ReadInteger(sSection, "Stop Type", Recipe.StopType);
		Recipe.IsGemRecipe = statFile->ReadBool(sSection, "GEM Recipe", Recipe.IsGemRecipe);
	}
	catch (Exception* e)
	{
		return false;
	}

	return true;
}

bool NormalOvenChamber::IsDoorLocked()
{
	bool ret = true;

    if (GetManager()->UseDoorLockCylinder)
    {
		if( DIO[I_DOOR_CLOSE_LOCK_ON]->IsOff() == true &&
			DIO[I_DOOR_OPEN_LOCK_OFF]->IsOn() == true )
            ret = true;
    }
	return ret;
}

bool NormalOvenChamber::IsDoorUnlocked()
{
	bool ret = false;

	if (GetManager()->UseDoorLockCylinder)
	{
		if( DIO[I_DOOR_CLOSE_LOCK_ON]->IsOn(3000) == true &&
			DIO[I_DOOR_OPEN_LOCK_OFF]->IsOff(3000) == true )
			ret = true;
    }
	return ret;
}

bool NormalOvenChamber::IsDoorClosed()
{
 	return DIO[I_DOOR_CLOSED]->IsOn();

//  if (FMainTempController->DIStatus[3] == 1)
//		return true;
//	return false; // hcw test.
}

bool NormalOvenChamber::IsMagazineDetected()
{
	if( GetManager()->UseMagazineDetector == false )
		return false;

	return DIO[I_MAGAZINE_DETECTED]->IsOn();
}

void NormalOvenChamber::CreateDIO()
{

	for (int i = 0; i < MAX_DIO_COUNT; i++)
	{
		if (GetManager()->DIOType == ICPDAS)
		{
			DIO[i] = new ICPDASDIO();
		}
		else if (GetManager()->DIOType == OVEN_DIO)
		{
			DIO[i] = new VSOvenBoardDIO();
		}
	}
}

void NormalOvenChamber::DestroyDIO()
{
	for (int i = 0; i < MAX_DIO_COUNT; i++)
	{
		delete DIO[i];
    }
}

void NormalOvenChamber::InitDIO()
{
	int ioIdx;

	if (GetManager()->DIOType == ICPDAS)
	{
		for(int i=0; i<16; i++)
		{
			DIO[i]->DIOModule = ModuleDI;
			DIO[i]->SetIO(i);

			DIO[i+DIO_COUNT/2]->DIOModule = ModuleDO;
			DIO[i+DIO_COUNT/2]->SetIO(i);
		}

		for(int i=16; i<24; i++)
		{
			DIO[i]->DIOModule = ModuleDIO;
			DIO[i]->SetIO(i);

			DIO[i+DIO_COUNT/2]->DIOModule = ModuleDIO;
			DIO[i+DIO_COUNT/2]->SetIO(i+8);
		}
	}
	else if (GetManager()->DIOType == OVEN_DIO)
	{
		for(int i=0; i<32; i++) //2023-03-09  24 -> 32
		{
			DIO[i]->DIOModule = OvenBoard;
			DIO[i]->SetIO(i);
		}

		for(int i=32; i<64; i++) //2023-03-09    24,48 -> 32, 64
		{
			DIO[i]->DIOModule = OvenBoard;
			DIO[i]->SetIO(i);  //2023-03-09  i+8 -> i
		}
	}
}

void NormalOvenChamber::BuzzerOn(int buzzerType)
{
	if( buzzerType == BUZZER_SOUND )
	{
		DIO[O_BUZZER_KIND]->Off();
	}
	else
	{
		DIO[O_BUZZER_KIND]->On();
	}
    Sleep(300);
	DIO[O_BUZZER_ON]->On();
}

void NormalOvenChamber::BuzzerOff()
{
	DIO[O_BUZZER_ON]->Off();
}

void NormalOvenChamber::BuzzerOffSwitchOn(bool on/*=true*/)
{
	if( on )
	{
		DIO[O_BUZZER_OFF_SWITCH]->On();
	}
	else
	{
		DIO[O_BUZZER_OFF_SWITCH]->Off();
	}
}

void NormalOvenChamber::TowerLampGreen(bool on)
{
	if( on )
	{
		DIO[O_TOWER_LAMP_GREEN]->On();
	}
	else
	{
		DIO[O_TOWER_LAMP_GREEN]->Off();
	}
}

void NormalOvenChamber::TowerLampRed(bool on)
{
	if( on )
	{
		DIO[O_TOWER_LAMP_RED]->On();
	}
	else
	{
		DIO[O_TOWER_LAMP_RED]->Off();
	}
}

void NormalOvenChamber::TowerLampYellow(bool on)
{
	if( on )
	{
		DIO[O_TOWER_LAMP_YELLOW]->On();
	}
	else
	{
		DIO[O_TOWER_LAMP_YELLOW]->Off();
	}
}

void NormalOvenChamber::ForcedCureStop()
{
	if( FMainTempController == nullptr )
		return;

	SetStep(STEP_DONE);
	FMainTempController->Reset();
}

int NormalOvenChamber::CalcRemainTime()
{
	if( FMainTempController == nullptr )
		return 0;

	if( FStep == STEP_IDLE ||
		FStep == STEP_DONE ||
		(FStep == STEP_COOLING && (IsReset || IsLocal)) )
		return 0;

	int time = 0;

	if( FStep > STEP_IDLE &&
		FStep <= STEP_DONE )
	{
		for(int i=SegNo; i<Recipe.TempPtn.Stc.SegCount; i++)
			time += Recipe.TempPtn.Seg[i].Time;
		time += FMainTempController->RemainingTime;
	}

	return time;
}

void NormalOvenChamber::MakeDataFilename()
{
	FILE * pFile;
	AnsiString file, name, folder;

	name.sprintf("%c_", 'A' + FChamberID);
	name += Recipe.StartTime.FormatString("yyyy_mm_dd_hh_nn_ss");

	folder = GetReportFolder();
	file = folder + "MainTemp_" + name + ".cht";
	pFile = fopen(file.c_str(), "a+t");
	if( pFile != nullptr )
		fclose(pFile);
	Recipe.MainTempFilename = file;

	file = folder + "LIMIT_" + name + ".cht";
	pFile = fopen(file.c_str(), "a+t");
	if( pFile != nullptr )
		fclose(pFile);
	Recipe.LimitFilename = file;

	file = folder + "LOGGER_" + name + ".cht";
	pFile = fopen(file.c_str(), "a+t");
	if( pFile != nullptr )
		fclose(pFile);
	Recipe.LoggerFilename = file;

	file = folder + "O2_" + name + ".cht";
	pFile = fopen(file.c_str(), "a+t");
	if( pFile != nullptr )
		fclose(pFile);
	Recipe.O2Filename = file;
}

void NormalOvenChamber::DoMainTempControllerKeyLock()
{
	if( FMainTempController == nullptr )
		return;

	if( GetManager()->LoginLevel >= LV_ENGINEER )
	{
		if( IsKeyLocked )
			FMainTempController->UnlockKey();
	}
	else
	{
		if( IsKeyLocked == false )
			FMainTempController->LockKey();
	}
}

void NormalOvenChamber::DoMainTempControllerSetAuto()
{
	if( FMainTempController == nullptr )
		return;

	if( IsAutoMode == false )
		FMainTempController->SetAutoMode();
}

bool NormalOvenChamber::WriteOxygenData(TDateTime time)
{
	if( O2Analyzer == nullptr )
		return false;

	AnsiString data, tmp;

	tmp = (AnsiString)time.FormatString("hh:nn:ss");
	data = tmp + data.sprintf(",%.1f\n", O2Analyzer->O2Density);

	FILE * pFile;
	pFile = fopen(Recipe.O2Filename.c_str(), "a+t");
	if( pFile == nullptr )
		return false;
	std::fwrite(data.c_str(), data.Length(), 1, pFile);
	fclose(pFile);
	return true;
}

// 20180305 - O2 Analyzer Use Cure Start Limited (ChipPAC) - jhlim
bool NormalOvenChamber::CheckO2DensityforCureStart()
{
	if( O2Analyzer == nullptr )
		return false;

	if( IsO2AlarmStartTimeSet == false )
	{
		IsO2AlarmStartTimeSet = true;
		O2AlarmStartTime = Now();
	}

	if( O2Analyzer->IsValid &&
		O2Analyzer->O2Density <= O2DensityRunLimit )
	{
		return true;
	}

	if( MinutesBetween(O2AlarmStartTime, Now()) >= O2DensityAlarmRunLimit )
	{
		IsO2DensityRunLimitAlarm = true;
		AlarmStatus[ERR_O2_DENSITY_ALARM] = true;
	}
	return false;
}

// 20191216 - SCK MES - jhlim
void NormalOvenChamber::InitData_MESLotInfo()
{
	stSckMesInfo.Init();
}

void NormalOvenChamber::MakeFolder_MESLotInfo()
{
	String work, workdone;

	work = GetSCKMESWorkFileFolder();
	workdone = GetSCKMESWorkDoneFileFolder();
	if( !DirectoryExists(work) )
	{
		if( !CreateDir(work) )
			throw (Exception(work+" folder creation fail."));
	}

	if( !DirectoryExists(workdone) )
	{
		if( !CreateDir(workdone) )
			throw (Exception(workdone+" folder creation fail."));
	}
}

bool NormalOvenChamber::CheckExistFile_MESLotInfo()
{
	bool bReturn = false;

	String strPath, strFile1, strFile2;
	strPath = GetSCKMESWorkFileFolder();
	// Folder Check
	if( DirectoryExists(strPath) == true )
	{
		TSearchRec sr;
		if( FindFirst(strPath + "*.*", faArchive, sr) == 0)
		{
			do
			{
				// Recent File Search
				strFile2 = sr.Name.c_str();
				if( CompareStr(strFile1, strFile2) < 0 )
				{
					strFile1 = strFile2;
				}
			} while( FindNext(sr) == 0 );
		}
		FindClose(sr);
	}

	if(strFile1 != "")
	{
		InitData_MESLotInfo();

		stSckMesInfo.FileName = strPath + "//" + strFile1;
		bReturn = true;
	}

	return bReturn;
}

bool NormalOvenChamber::ReadFile_MESLotInfo()
{
	bool bReturn = false;
	AnsiString strPath = stSckMesInfo.FileName;

	FILE * pFile;
	pFile = std::fopen(strPath.c_str(), "r");
	if( pFile == nullptr )
		return bReturn;

	char * buff;
	long lSize;
	size_t result;
	AnsiString str[4];

	// File Size Check
	std::fseek(pFile, 0, SEEK_END);
	lSize = std::ftell(pFile);
	std::rewind(pFile);
	if(lSize > 0)
	{
		// File Read
		buff = (char*)malloc(sizeof(char) * lSize);
		result = std::fread(buff, sizeof(char), lSize, pFile);
		std::fclose(pFile);
		// File Size & Read Size Compare
		if((long)result == lSize)
		{
			int i = 0;
			char * ptr = strtok(buff, "/");
			while(ptr != nullptr)
			{
				str[i++] = ptr;
				ptr = strtok(nullptr, "/");
			}
		}
		// Lot Information Save
		stSckMesInfo.LotID = str[0];
		stSckMesInfo.ProductCode = str[1];
		stSckMesInfo.RecipeName = str[2];
		stSckMesInfo.EquipName = str[3];

		bReturn = true;
	}
	free(buff);

    return bReturn;
}

void NormalOvenChamber::StartIdleCooling()
{
	if( FMainTempController == nullptr )
		return;

	if( FMainTempController->IsFail )
		return;

	PTN ptn;

	ptn.TotalTime = 30;
	ptn.MaxTemp = 0;
	ptn.PtnNo = 30;
	ptn.Stc.SegCount = 1;
	ptn.Stc.Ssp = 0;
	ptn.Stc.Stc = 0;
	ptn.Seg[0].Tsp = 0;
	ptn.Seg[0].Time = 30;
	ptn.Seg[0].Pid = 1;
	ptn.Seg[0].Jc = 2;
	ptn.Seg[0].EvKind[0] = ET_COOLING;
	ptn.Seg[0].OnType[0] = 0;
	ptn.Seg[0].OffPe[0] = 30;

	FMainTempController->Reset();
	FMainTempController->DeletePattern(ptn.PtnNo);
	FMainTempController->WriteSTC(ptn);
	for(int i=0; i<ptn.Stc.SegCount; i++)
		FMainTempController->WriteSeg(i, ptn);

	FMainTempController->Run(ptn.PtnNo);
    FIdleCoolingStarted = true;
	WaitForSingleObject((void*)Handle, 500);
}

void NormalOvenChamber::StopIdleCooling()
{
	if( FMainTempController == nullptr )
		return;

	if( FMainTempController->IsFail )
		return;

	FMainTempController->Reset();
    FIdleCoolingStarted = false;
	WaitForSingleObject((void*)Handle, 500);
}

void NormalOvenChamber::DoIdleCooling()
{
	if (UseIdleCooling == false)
    {
		return;
	}

	if (AlarmStatus[ERR_EMERGENCY_SW])
	{
		StopIdleCooling();
		return;
    }

	if (IdleCoolingDisabled)
	{
    	if (IdleCoolingDoorOpened)
        {
        	if (IsDoorClosed())
            {
        		IdleCoolingDisabled = false;
                IdleCoolingDoorOpened = false;
            }
        }
        else
        {
            if (IsDoorClosed() == false)
            {
                IdleCoolingDoorOpened = true;
            }
        }
		return;
	}

	if (FIdleCoolingTemp == 0 || IsDoorClosed() == false)
	{
		if( IsRun )
		{
			StopIdleCooling();
			Sleep(2000);
		}
		return;
	}

	if( PV > FIdleCoolingTemp + FIdleCoolingOffset )
	{
		if( IsRun == false || IsCoolingEventOn == false )
		{
			StartIdleCooling();
			Sleep(2000);
        }
	}
	else
	if( PV < FIdleCoolingTemp - FIdleCoolingOffset )
	{
		if( IsRun )
        {
			StopIdleCooling();
			Sleep(2000);
		}
	}
}

void NormalOvenChamber::DoIdleCoolingDisable()
{
	IdleCoolingDisabled = true;
    IdleCoolingDoorOpened = false;
	Sleep(1000);

	StopIdleCooling();
}

void NormalOvenChamber::DoDoorLockControl()
{
   if (Step <= STEP_IDLE)
   {
      return;
   }

   if (Step >= STEP_PURGE && Step <= STEP_COOLING)
   {
	  DIO[O_DOOR_LOCK]->On();
   }
   else
   {
		if(GetManager()->UseDoorLockTempControl)   // 2020-12-18 Add 일정온도 이상일때 도어락
		{
			if (PV < GetManager()->DoorLockTemp)
			{
				if (IsDoorLockTempTimeSet == false)
				{
					DoorLockTempTime = Now();
					IsDoorLockTempTimeSet = true;
				}
				else
				{
					if (SecondsBetween(DoorLockTempTime, Now()) >= 10)
					{
						DIO[O_DOOR_LOCK]->Off();
						//IsDoorLockTempTimeSet = false;
					}
				}
			}
			else
			{
				DIO[O_DOOR_LOCK]->On();
				IsDoorLockTempTimeSet = false;
			}
		}
		else
		{
			DIO[O_DOOR_LOCK]->Off();
		}
    }
}

void NormalOvenChamber::DoHeaterProblemLampControl()
{
	if (GetManager()->DIOType == OVEN_DIO)
	{
		// 오븐제어보드 제작하면서 히터 오버로드 램프 점등기능 누락되어
		// 소프트웨어에서 램프 점등할 수 있도록 O_HEATER_PROBLEM_LAMP 추가함
		if (DIO[I_HEATER_OVERLOAD_DETECTED]->IsOn(IO_DELAY))
		{
			DIO[O_HEATER_PROBLEM_LAMP]->On();
		}
		else
		{
			DIO[O_HEATER_PROBLEM_LAMP]->Off();
		}
	}
}

void NormalOvenChamber::DoTowerLamp(int towerLamp, int mode, bool isBlink)
{
	switch (mode)
	{
	case LAMP_BLINK:
		if (isBlink)
		{
			DIO[towerLamp]->On();
		}
		else
		{
			DIO[towerLamp]->Off();
		}
	break;
	case LAMP_ON:
		DIO[towerLamp]->On();
	break;
	case LAMP_OFF:
		DIO[towerLamp]->Off();
	break;
	}
}

void NormalOvenChamber::DoTowerLampControl(bool isBlink)
{
	int mode;
	if (GetAlarmCount() > 0)
	{
		if (GetManager()->UseTowerLampControl)
		{
			mode = GetManager()->TowerLampOption->GetTwrVal(TWR_RED, TWR_ERR);
			DoTowerLamp(O_TOWER_LAMP_RED, mode, isBlink);

			mode = GetManager()->TowerLampOption->GetTwrVal(TWR_YEL, TWR_ERR);
			DoTowerLamp(O_TOWER_LAMP_YELLOW, mode, isBlink);

			mode = GetManager()->TowerLampOption->GetTwrVal(TWR_GRN, TWR_ERR);
			DoTowerLamp(O_TOWER_LAMP_GREEN, mode, isBlink);
		}
		else
		{
			if (AlarmStatus[ERR_HEATER_OVERLOAD] ||
				AlarmStatus[ERR_BLOWER_OVERLOAD] ||
				AlarmStatus[ERR_BLOWER_LOW_CURRENT])
			{
				TowerLampRed(isBlink);
			}
			else
			{
				TowerLampRed(true);
			}
			TowerLampGreen(false);
			TowerLampYellow(false);
		}
	}
	else
	{
		if (Step >= STEP_PURGE && Step <= STEP_COOLING)
		{
			if (GetManager()->UseTowerLampControl)
			{
				mode = GetManager()->TowerLampOption->GetTwrVal(TWR_RED, TWR_AUTORUN);
				DoTowerLamp(O_TOWER_LAMP_RED, mode, isBlink);

				mode = GetManager()->TowerLampOption->GetTwrVal(TWR_YEL, TWR_AUTORUN);
				DoTowerLamp(O_TOWER_LAMP_YELLOW, mode, isBlink);

				mode = GetManager()->TowerLampOption->GetTwrVal(TWR_GRN, TWR_AUTORUN);
				DoTowerLamp(O_TOWER_LAMP_GREEN, mode, isBlink);
			}
			else
			{
				TowerLampRed(false);
				TowerLampGreen(true);
				TowerLampYellow(false);
			}
		}
		else if (Step >= STEP_DONE && Step <= STEP_LOCAL)
		{
			if (GetManager()->UseTowerLampControl)
			{
				mode = GetManager()->TowerLampOption->GetTwrVal(TWR_RED, TWR_LOTEND);
				DoTowerLamp(O_TOWER_LAMP_RED, mode, isBlink);

				mode = GetManager()->TowerLampOption->GetTwrVal(TWR_YEL, TWR_LOTEND);
				DoTowerLamp(O_TOWER_LAMP_YELLOW, mode, isBlink);

				mode = GetManager()->TowerLampOption->GetTwrVal(TWR_GRN, TWR_LOTEND);
				DoTowerLamp(O_TOWER_LAMP_GREEN, mode, isBlink);
			}
			else
			{
				TowerLampRed(false);
				TowerLampGreen(false);
				TowerLampYellow(isBlink);
			}
		}
		else
		{
			if (GetManager()->UseTowerLampControl)
			{
				mode = GetManager()->TowerLampOption->GetTwrVal(TWR_RED, TWR_STOP);
				DoTowerLamp(O_TOWER_LAMP_RED, mode, isBlink);

				mode = GetManager()->TowerLampOption->GetTwrVal(TWR_YEL, TWR_STOP);
				DoTowerLamp(O_TOWER_LAMP_YELLOW, mode, isBlink);

				mode = GetManager()->TowerLampOption->GetTwrVal(TWR_GRN, TWR_STOP);
				DoTowerLamp(O_TOWER_LAMP_GREEN, mode, isBlink);
			}
			else
			{
				TowerLampRed(false);
				TowerLampGreen(false);
				TowerLampYellow(true);
			}
		}
	}
}
//---------------------------------------------------------2021-04-13 HANA OVEN
void NormalOvenChamber::DoHanaProtocalSeq()
{
	//2021-04-12 HANA OVEN
	if (!(GetManager()->AutomationKind == HANA_CIM  && HanOvenCim))
		return;

	int EndState, OperationMode;
	int nEndState, nOperation;
	String str;

	HanOvenCim->GetOperationEndState(&EndState, &OperationMode);
	nEndState =  EndState;
	nOperation = OperationMode;

	if (FStep == STEP_IDLE)
	{
		if( IsDoorClosed() == false )
		{
			HanOvenCim->SegCount  = 0;
			HanOvenCim->DoHanaSegNo(0);
			HanOvenCim->DoHanaPatternNo(0);
			HanOvenCim->DoOperationMode(WAIT_HANA);
			HanOvenCim->DoEndState(INIT_HANA);
			FHanaCureComplete = false;
			FHanaCureFinishStatus = false;
		}
		else
		{
			HanOvenCim->SegCount  = 0;
			HanOvenCim->DoOperationMode(INIT_HANA);
			HanOvenCim->DoEndState(INIT_HANA);
			FHanaCureComplete = false;
			FHanaCureFinishStatus = false;
		}
	}
	else if (FStep == STEP_RUNNING)
	{
		HanOvenCim->DoHanaSegNo(SegNo);
		HanOvenCim->DoHanaPatternNo(Recipe.TempPtn.PtnNo);

		if (HanOvenCim->HanaData.CurrentSegNo == 1 && HanOvenCim->SegCount < 300)
		{
			FHanaCureComplete = true;
			HanOvenCim->DoEndState(0);
			HanOvenCim->SegCount += 1;
			if (HanOvenCim->SegCount > 299)
			{
				HanOvenCim->DoOperationMode(EXECUTION_HANA);

				str.sprintf(L"HANA OVEN STEP_RUN CHM[%d] : Operation[%d], EndState[%d]", FChamberID, 1, 0);
				SaveDebugLog(str);
			   //	HanOvenCim->m_nSegCount  = 0;
			}
		}
		else if (HanOvenCim->HanaData.CurrentSegNo == 0 && FHanaCureComplete == false)
		{
			HanOvenCim->SegCount  = 0;
			HanOvenCim->DoOperationMode(INIT_HANA);
			str.sprintf(L"HANA OVEN STEP_RUN CHM[%d] : CurrentSeg[%d], Operation[%d], EndState[%d]",
			FChamberID, HanOvenCim->HanaData.CurrentSegNo, 0, 0);
			SaveDebugLog(str);
		}
	}
	else if (FStep == STEP_LOCAL)
	{
		if (HanOvenCim->HanaData.CurrentSegNo == Recipe.TempPtn.Stc.SegCount && FHanaCureComplete == true)
		{

			if (FHanaCureComplete == true)
			{
				HanOvenCim->SegCount  = 0;
				HanOvenCim->DoOperationMode(EXECUTION_HANA);
				HanOvenCim->DoEndState(EXECUTION_HANA);
				FHanaCureComplete = false;

				HanaEndStatusTime = Now();
				FHanaCureFinishStatus = true;
				str.sprintf(L"HANA OVEN STEP_LOCAL CHM[%d] : Operation[%d], EndState[%d]", FChamberID, 0, 1);
				SaveDebugLog(str);
			}
		}
		else
		{
			if (FHanaCureFinishStatus == true)
			{
				if (MilliSecondsBetween(HanaEndStatusTime, Now()) >= 3000)
				{
					HanOvenCim->SegCount  = 0;
					HanOvenCim->DoOperationMode(WAIT_HANA);
					HanOvenCim->DoEndState(EXECUTION_HANA);
					//FHanaCureFinishStatus = false;

					//str.sprintf(L"HANA OVEN STEP_LOCAL CureFinish CHM[%d] : Operation[%d], EndState[%d]", FChamberID, 2, 1);
					//SaveDebugLog(str);
				}
			}
		}
	}
	else if (FStep == STEP_DONE)
	{
	   //	if (HanOvenCim->HanaData.CurrentSegNo == 0 && FHanaCureComplete == true)
	   if (FHanaCureComplete == true)
	   {
			HanOvenCim->SegCount  = 0;
			HanOvenCim->DoOperationMode(EXECUTION_HANA);
			HanOvenCim->DoEndState(EXECUTION_HANA);
			HanaEndStatusTime = Now();
			FHanaCureComplete = false;
			FHanaCureFinishStatus = true;
			str.sprintf(L"HANA OVEN STEP_DONE CHM[%d] : Operation[%d], EndState[%d]", FChamberID, 1, 1);
			SaveDebugLog(str);
	   }
	   else
		{
			if (FHanaCureFinishStatus == true)
			{
				if (MilliSecondsBetween(HanaEndStatusTime, Now()) >= 3000)
				{
					HanOvenCim->SegCount  = 0;
					HanOvenCim->DoOperationMode(WAIT_HANA);
					HanOvenCim->DoEndState(EXECUTION_HANA);
					//FHanaCureFinishStatus = false;
					//str.sprintf(L"HANA OVEN STEP_DONE CureFinish CHM[%d] : Operation[%d], EndState[%d]", FChamberID, 2, 1);
					//SaveDebugLog(str);
				}
			}
		}
	}
}

bool NormalOvenChamber::DoHanaCureStop()
{
	if( FMainTempController == nullptr )
		return false;

	SetStep(STEP_IDLE);
	FMainTempController->Reset();

	return true;
}

void NormalOvenChamber::HanaStopStatus()
{
	String str;

	HanOvenCim->SegCount  = 0;
	HanOvenCim->DoHanaSegNo(0);
	HanOvenCim->DoHanaPatternNo(0);
	//HanOvenCim->DoOperationMode(INIT_HANA);
	//HanOvenCim->DoEndState(INIT_HANA);
	//FHanaCureComplete = false;

	str.sprintf(L"HANA OVEN STOP BUTTON CHM[%d] : Operation[%d], EndState[%d]", FChamberID, 0, 0);
	SaveDebugLog(str);
}

void NormalOvenChamber::HanaCureFinishStatus()
{
	 HanaEndStatusTime = Now();
	 FHanaCureFinishStatus = true;
}
//-----------------------------------------------------------
void NormalOvenChamber::DoLoggerTempDeviation()
{
	if (ChamberLoggerTempDeviation == 0)
	{
		return;
	}

	int nSegStart = 1;
	int nSeg = 0; int nSegCount = 0;

	if (FStep == STEP_RUNNING && IsRun)
	{
		nSeg = FMainTempController->SegNo;
		nSegCount = FMainTempController->TotalSeg;

	  if ((nSegStart < SegNo)&&(SegNo < Recipe.TempPtn.Stc.SegCount))
	  {
		 if (Recipe.TempPtn.Seg[nSeg-2].Tsp == Recipe.TempPtn.Seg[nSeg-1].Tsp)
		 {
			DwellTimeLoggerCalc();
		 }
	  }
	}
	else
	{
		for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
		{
			if( UseChannel[i] )
			{
				TempLoggerTimeCheck[i] = false;
			}
		}
	}
}
 //------------------------------------------------------------------------
bool NormalOvenChamber::DwellTimeLoggerCalc()
{
	double SPTemp;
	double SPCalc;
	double data;
	String time, str;

	SPTemp = FMainTempController->SP;

	for(int i=0; i<MAX_LOGGERPOINT_COUNT; i++)
	{
		data = LoggerData[i];

		if( UseChannel[i] )
		{
			if( data <= 0.0 )
				data = TempLoggerData[i];

			TempLoggerData[i] = data;
			SPCalc = fabs(SP - data);

			if (TempLoggerTimeCheck[i] == true
				&& AlarmStatus[ERR_CHAMBER_LOGGER_DEVIATION_ALARM_1+i] == false)
			{
				if (MilliSecondsBetween(LoggerDeviationTime[i], Now()) >= 500)
				{
					data = LoggerData[i];

					if( data <= 0.0 )
						data = TempLoggerData[i];

					TempLoggerData[i] = data;
					SPCalc = fabs(SP - data);

					if (SPCalc > ChamberLoggerTempDeviation && IsLoggerDeviationAlarm[i] == false)
					{
						IsLoggerDeviationAlarm[i] = true;
						AlarmStatus[ERR_CHAMBER_LOGGER_DEVIATION_ALARM_1+i]= true;

						time = FormatDateTime(L"yyyymmddhhnnss", Now());
						str.sprintf(L"DEVIATION ALARM[TRUE]>=500,CHM[%d]:>[CH %d][CHECK %d], Time[%s], SPCalc[%lf]",
							FChamberID, i, TempLoggerTimeCheck[i], time.c_str(), SPCalc);
						SaveDebugLog(str);
					}
					else
					{
						IsLoggerDeviationAlarm[i] = false;
						AlarmStatus[ERR_CHAMBER_LOGGER_DEVIATION_ALARM_1+i]= false;

						time = FormatDateTime(L"yyyymmddhhnnss", Now());
						str.sprintf(L"DEVIATION ALARM[TRUE] >= 500,CHM[%d]:[CH %d][CHECK %d], Time[%s], SPCalc[%lf]",
							FChamberID, i, TempLoggerTimeCheck[i], time.c_str(), SPCalc);
						SaveDebugLog(str);
					}
					TempLoggerTimeCheck[i] = false;
				}
			}
			else
			{
				if (SPCalc > ChamberLoggerTempDeviation
					&& AlarmStatus[ERR_CHAMBER_LOGGER_DEVIATION_ALARM_1+i] == false)
				{
					LoggerDeviationTime[i] = Now();
					TempLoggerTimeCheck[i] = true;

					time = FormatDateTime(L"yyyymmddhhnnss", Now());
					str.sprintf(L"DEVIATION ALARM[FALSE] CHM[%d]:>[CH %d][CHECK %d], Time[%s], SPCalc[%lf]",
						FChamberID, i, TempLoggerTimeCheck[i], time.c_str(), SPCalc);
					SaveDebugLog(str);
				}
				else
				{
					IsLoggerDeviationAlarm[i] = false;
					TempLoggerTimeCheck[i] = false;
				}
			}
		}
	}
	return false;
}

bool NormalOvenChamber::GetLoggerDeviation(int idx)
{
	bool nTest;
	if( idx >=0 && idx < MAX_LOGGERPOINT_COUNT )
	{
		nTest = FIsLoggerDeviationAlarm[idx];
		return FIsLoggerDeviationAlarm[idx];
	}
	return false;
}

void NormalOvenChamber::SetLoggerDeviation(int idx, bool status)
{
	if( idx >=0 && idx < MAX_LOGGERPOINT_COUNT )
	{
		FIsLoggerDeviationAlarm[idx] = status;
	}
}

void NormalOvenChamber::ResetSafetyRelay()
{
	int count = 0;
	while (true)
	{
		Application->ProcessMessages();
		if (DIO[I_SAFETY_STATUS_ALARM_DETECTED]->IsOff)
		{
 //			DIO[O_SAFETY_RESET_ON]->Off();
			break;
		}
		else
		{
            DIO[O_SAFETY_RESET_ON]->On();
        }
		count++;
		if (count >= 1000)
		{
			break;
		}
	}
}
void NormalOvenChamber::ResetSafetyOn()
{
	DIO[O_SAFETY_RESET_ON]->On();
}

void NormalOvenChamber::CowellLotInfoClear()
{
	ValidationRequested = false;
	ValidationSuccess = false;
	ErrorDetail = "";
	if (LotIDList)
	{
		LotIDList->Clear();
	}
}

void NormalOvenChamber::JsckGemParamClear()
{
	MesMode = -1;
	ProcessCode = "";
	ValidationRequested = false;
	ValidationSuccess = false;

	DeviceIDList->Clear();
}

