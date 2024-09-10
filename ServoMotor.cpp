//---------------------------------------------------------------------------
#include <vcl.h>
#include <mmsystem.h>
#include <math.h>

#pragma hdrstop

#include "ServoMotor.h"

#pragma link "AxtLib.lib"
#pragma package(smart_init)

//---------------------------------------------------------------------------
ServoMotor::ServoMotor(const INT16 axis, const int cardType, const bool ezSvr)
	: FAxis(axis), CardType(cardType), EzSvr(ezSvr)
{
	FMotorID = axis;
	OrgLevel = true;
	NPLOrg = false;

	SetMoveUnitPerPulse();
	SetStartStopSpeed();
	SetMaxSpeed();

	TimerOneShotID = 0;
	TimerLoopID = 0;

	// Alarm 설정 관련 변수
	FAlarmReset = false;
	AlarmResetLevel = HIGH;

	// 원점 검색 관련 변수
	FHoming = false;
	FHomeDone = false;
	FHomeEnd = false;
	HomingAbort = false;

	FRepeatDone = true;

	LoopDelayTime = 0;
	LoopScanTime = 50;

	RunMode = RUN_IDLE;
}
//---------------------------------------------------------------------------
ServoMotor::~ServoMotor()
{
}
//---------------------------------------------------------------------------
void ServoMotor::SetDelay(UINT uMSec)
{
	LoopDelayTime = 0;
	DelayTimeOut = uMSec;
}
//---------------------------------------------------------------------------
bool ServoMotor::IsDelayTimeOut()
{
	if( LoopDelayTime * LoopScanTime < DelayTimeOut )
		return false;

	return true;
}
//---------------------------------------------------------------------------
void ServoMotor::SetAxisPara1(BYTE decelstartpoint /* = AutoDetect */,
						 BYTE pulseoutmethod  /* = TwoCcwCwHigh : 2PULSE 방식, PULSE(CCW), DIR(CW), ACTIVE HIGH */,
						 BYTE detectsignal    /*= PElmNegativeEdge : +END LIMIT, DOWN EDGE*/)
{
	switch( CardType )
	{
	case CAM5:
		C5Mset_drive_mode1(FAxis,
						   decelstartpoint,
						   pulseoutmethod,
						   detectsignal);
		break;

	case CFS:
		CFSset_drive_mode1(FAxis,
						   decelstartpoint,		// 감속 시작 포인트 검출 방식
						   pulseoutmethod,		// 펄스 출력 방식
						   detectsignal);		// 검출 대상 신호
		break;

	case CFS2:
		CFS20set_drive_mode1(FAxis,
							 decelstartpoint,    // 감속 시작 포인트 검출 방식
							 pulseoutmethod,     // 펄스 출력 방식
							 detectsignal);	     // 검출 대상 신호
		break;

	default:
		break;
	}
}
//---------------------------------------------------------------------------
void ServoMotor::SetAxisPara2(BYTE enc_method			/* = Sqr4Mode */,
						 BYTE inp_activelevel		/* = InpActiveHigh */,
						 BYTE alarm_activelevel		/* = AlmActiveHigh */,
						 BYTE nslm_activelevel		/* = NSlmActiveHigh */,
						 BYTE pslm_activelevel		/* = PSlmActiveHigh */,
						 BYTE nelm_activelevel		/* = NElmActiveHigh */,
						 BYTE pelm_activelevel		/* = PElmActiveHigh	*/)
{
	switch( CardType )
	{
	case CAM5:
		C5Mset_drive_mode2(FAxis,
						   enc_method,
						   inp_activelevel,
						   alarm_activelevel,
						   nslm_activelevel,
						   pslm_activelevel,
						   nelm_activelevel,
						   pelm_activelevel);
		break;

	case CFS:
		CFSset_drive_mode2(FAxis,
						   enc_method,          /* = Sqr4Mode : 2PHASE SIGNAL*/
						   inp_activelevel,     /* = InpActiveHigh */
						   alarm_activelevel,   /* = AlmActiveHigh */
						   nslm_activelevel,    /* = NSlmActiveHigh */
						   pslm_activelevel,    /* = PSlmActiveHigh */
						   nelm_activelevel,    /* = NElmActiveHigh */
						   pelm_activelevel);   /* = PElmActiveHigh	*/
		break;

	case CFS2:
		CFS20set_drive_mode2(FAxis,
							 enc_method,          /* = Sqr4Mode : 2PHASE SIGNAL*/
							 inp_activelevel,     /* = InpActiveHigh */
							 alarm_activelevel,   /* = AlmActiveHigh */
							 nslm_activelevel,    /* = NSlmActiveHigh */
							 pslm_activelevel,    /* = PSlmActiveHigh */
							 nelm_activelevel,    /* = NElmActiveHigh */
							 pelm_activelevel);   /* = PElmActiveHigh */
		break;

	default:
		break;
	}
}
//---------------------------------------------------------------------------
void ServoMotor::SetMoveUnitPerPulse(double dunitperpulse)
{
	switch( CardType )
    {
	case CAM5:
		C5Mset_moveunit_perpulse(FAxis, dunitperpulse);
		break;
	case CFS:
		CFSset_moveunit_perpulse(FAxis, dunitperpulse);
		break;
	case CFS2:
		CFS20set_moveunit_perpulse(FAxis, dunitperpulse);
		break;
	default:
		break;
	}
}
//---------------------------------------------------------------------------
void ServoMotor::SetStartStopSpeed(double dSpeed)
{
	switch( CardType )
    {
	case CAM5:
		C5Mset_startstop_speed(FAxis, dSpeed);
		break;
	case CFS:
		CFSset_startstop_speed(FAxis, dSpeed);
		break;
	case CFS2:
		CFS20set_startstop_speed(FAxis, dSpeed);
		break;
	default:
		break;
	}
}
//---------------------------------------------------------------------------
void ServoMotor::SetMaxSpeed(double dSpeed)
{
	switch( CardType )
    {
	case CAM5:
		C5Mset_max_speed(FAxis, dSpeed);
		break;
	case CFS:
		CFSset_max_speed(FAxis, dSpeed);
		break;
	case CFS2:
		CFS20set_max_speed(FAxis, dSpeed);
		break;
	default:
		break;
	}
}
//---------------------------------------------------------------------------
void ServoMotor::ServoEnable(bool bStatus)
{
	if( EzSvr )
        bStatus = !bStatus;

	switch( CardType )
    {
	case CAM5:
		C5Mset_servo_enable(FAxis, bStatus);
		//C5Mchange_output_bit(Axis, 0, (UINT8)bStatus);
		break;
	case CFS:
		CFSset_servo_enable(FAxis, bStatus);
		break;
	case CFS2:
		CFS20set_servo_enable(FAxis, bStatus);
		break;
	default:
		return;
	}

	if( bStatus )
	{
        Sleep(500);
        SetCmdPos(GetActPos());
    }
}
//---------------------------------------------------------------------------
bool ServoMotor::IsServoEnabled()
{
	bool bRet;

	switch( CardType )
    {
	case CAM5:
		bRet = C5Moutput_bit_on(FAxis, 0);
		break;
	case CFS:
		bRet = CFSoutput_bit_on(FAxis, 0);
		break;
	case CFS2:
		bRet = CFS20output_bit_on(FAxis, 0);
		break;
	default:
		return false;
	}

	if( EzSvr )
        bRet = !bRet;

    return bRet;
}
//---------------------------------------------------------------------------
void ServoMotor::AlarmReset()
{
	FAlarmReset = true;
	switch( CardType )
    {
	case CAM5:
		C5Mchange_output_bit(FAxis, 1, AlarmResetLevel);
		Sleep(200);
		C5Mchange_output_bit(FAxis, 1, false);
		break;
	case CFS:
		CFSchange_output_bit(FAxis, 1, AlarmResetLevel);
		Sleep(200);
		CFSchange_output_bit(FAxis, 1, false);
		break;
	case CFS2:
		CFS20change_output_bit(FAxis, 1, AlarmResetLevel);
		Sleep(200);
		CFS20change_output_bit(FAxis, 1, false);
		break;
	default:
		break;
	}
}
//---------------------------------------------------------------------------
bool ServoMotor::IsAlarmReset()
{
	switch( CardType )
    {
	case CAM5:
		return C5Moutput_bit_on(FAxis, 1);
	case CFS:
		return CFSoutput_bit_on(FAxis, 1);
	case CFS2:
		return CFS20output_bit_on(FAxis, 1);
	default:
		return false;
	}
}
//---------------------------------------------------------------------------
bool ServoMotor::IsReady()
{
	switch( CardType )
    {
	case CAM5:
		return C5Minput_bit_on(FAxis, 2);
	case CFS:
		return CFSinput_bit_on(FAxis, 2);
	case CFS2:
		return CFS20input_bit_on(FAxis, 2);
	default:
		return false;
	}
}
//---------------------------------------------------------------------------
bool ServoMotor::IsAlarm()
{
	switch( CardType )
    {
	case CAM5:
		return C5Mget_alarm_switch(FAxis);
	case CFS:
		return CFSget_alarm_switch(FAxis);
	case CFS2:
		return CFS20get_alarm_switch(FAxis);
	default:
		return false;
	}
}
//---------------------------------------------------------------------------
bool ServoMotor::InPosition(double dMargin)
{
	//mm단위로 입력된 타겟위치에 마진 범위내로 로봇이 도착했으면 true, 아니면 false리턴
	double dActPos;
	double dGap;
	double dTargetPos;
	bool bRet, bStopMotion;

	dTargetPos = FDestPos; //이전에 내려진 모터 이동명령 위치 StartMotor 명령에의해 값이 저장된다.

	dActPos = GetActPos();  //  펄스단위의 현재 위치 가져옴
	dActPos = PulseToMM(dActPos);// mm로 단위환산
	dGap = dActPos - dTargetPos; //타겟위치와 실위치간의 차를 구하고

	if( dGap < 0 )
		dGap = dGap * (-1.0); //만일 두위치간의 차가 음수면 양수로 만들고

	if( dGap <= dMargin )
		bRet = true;
	else
		bRet = false;

	bStopMotion = !InMotion();

	return bRet && bStopMotion;
}
//---------------------------------------------------------------------------
bool ServoMotor::InPosition(double dPos, double dMargin)
{
	//mm단위로 입력된 타겟위치에 마진 범위내로 로봇이 도착했으면 true, 아니면 false리턴
	double dActPos;
	double dGap;
	bool bRet, bStopMotion;
	double dTargetPos;

	dTargetPos = dPos;

	dActPos = GetActPos();  //  펄스단위의 현재 위치 가져옴
	dActPos = PulseToMM(dActPos);// mm로 단위환산
	dGap = dActPos - dTargetPos; //타겟위치와 실위치간의 차를 구하고

	if( dGap < 0 )
		dGap = dGap * (-1.0); //만일 두위치간의 차가 음수면 양수로 만들고

	if( dGap <= dMargin )
		bRet = true;
	else
		bRet = false;

	bStopMotion = !InMotion();

	return bRet && bStopMotion;
}
//---------------------------------------------------------------------------
bool ServoMotor::IsPELM()
{
	switch( CardType )
    {
	case CAM5:
		return (C5Mget_pend_limit_switch(FAxis)==1);
	case CFS:
		return (CFSget_pend_limit_switch(FAxis)==1);
	case CFS2:
		return (CFS20get_pend_limit_switch(FAxis)==1);
	default:
		return false;
	}
}
//---------------------------------------------------------------------------
bool ServoMotor::IsNELM()
{
	switch( CardType )
    {
	case CAM5:
		return (C5Mget_nend_limit_switch(FAxis) == 1);
	case CFS:
		return (CFSget_nend_limit_switch(FAxis) == 1);
	case CFS2:
		return (CFS20get_nend_limit_switch(FAxis) == 1);
	default:
		return false;
	}
}
//---------------------------------------------------------------------------
bool ServoMotor::IsORG()
{
	switch( CardType )
    {
	case CAM5:
		return (C5Minput_bit_on(FAxis, 0) == OrgLevel);
	case CFS:
		return (CFSinput_bit_on(FAxis, 0) == OrgLevel);
	case CFS2:
		return (CFS20input_bit_on(FAxis, 0) == OrgLevel);
	default:
		return false;
	}
}
//---------------------------------------------------------------------------
void ServoMotor::ToggleIO(BYTE byOutputBit)
{
	BYTE bRead ;
	switch( CardType )
    {
	case CAM5:
		bRead = (C5Moutput_bit_on(FAxis, byOutputBit) == true);
		C5Mchange_output_bit(FAxis, byOutputBit, !bRead);
		break;
	case CFS:
		bRead = (CFSoutput_bit_on(FAxis, byOutputBit) == true);
		CFSchange_output_bit(FAxis, byOutputBit, !bRead);
		break;
	case CFS2:
		bRead = (CFS20output_bit_on(FAxis, byOutputBit) == true);
		CFS20change_output_bit(FAxis, byOutputBit, !bRead);
		break;
	default:
		break;
	}
}
//----------------------- 위치 설정 관련 함수 ----------------------//
void ServoMotor::SetCmdPos(double dPos)
{
	switch( CardType )
    {
	case CAM5:
		C5Mset_command_position(FAxis, dPos);
		break;
	case CFS:
		CFSset_command_position(FAxis, dPos);
		break;
	case CFS2:
		CFS20set_command_position(FAxis, dPos);
		break;
	default:
		break;
	}
}
//---------------------------------------------------------------------------
double ServoMotor::GetCmdPos()
{
	switch( CardType )
    {
	case CAM5:
		return C5Mget_command_position(FAxis);
	case CFS:
		return CFSget_command_position(FAxis);
	case CFS2:
		return CFS20get_command_position(FAxis);
	default:
		return -1.0;
	}
}
//---------------------------------------------------------------------------
void ServoMotor::SetActPos(double dPos)
{
	switch( CardType )
    {
	case CAM5:
		C5Mset_actual_position(FAxis, dPos);
		C5Mset_command_position(FAxis, dPos);
		break;
	case CFS:
		CFSset_actual_position(FAxis, dPos);
		CFSset_command_position(FAxis, dPos);
		break;
	case CFS2:
		CFS20set_actual_position(FAxis, dPos);
		CFS20set_command_position(FAxis, dPos);
		break;
	default:
		break;
	}
}
//---------------------------------------------------------------------------
double ServoMotor::GetActPos()
{
	switch( CardType )
    {
	case CAM5:
		return C5Mget_actual_position(FAxis);
	case CFS:
		return CFSget_actual_position(FAxis);
	case CFS2:
		return CFS20get_actual_position(FAxis);
	default:
		return -1.0;
	}
}
//---------------------------------------------------------------------------
void ServoMotor::MatchPos()
{
	double act_pos = GetActPos();
	SetCmdPos(act_pos);
}
//------------------------- 모션 관련 함수 --------------------------------//
bool ServoMotor::InMotion()
{
	switch( CardType )
    {
	case CAM5:
		return C5Min_motion(FAxis);
	case CFS:
		return CFSin_motion(FAxis);
	case CFS2:
		return CFS20in_motion(FAxis);
	default:
		return false;
	}
}
//---------------------------------------------------------------------------
BYTE ServoMotor::GetEndStatus()
{
	switch( CardType )
    {
	case CAM5:
		return C5Mget_end_status(FAxis);
	case CFS:
		return CFSget_end_status(FAxis);
	case CFS2:
		return CFS20get_end_status(FAxis);
	default:
		return -1;
	}
}
//---------------------------------------------------------------------------
bool ServoMotor::SignalSearch1(double dVel, double dAccel, BYTE detect_signal)
{
	if( InMotion() )
		return false;

	switch( CardType )
    {
	case CAM5:
		return C5Mstart_signal_search1(FAxis, dVel, dAccel, detect_signal);
	case CFS:
		return CFSstart_signal_search1(FAxis, dVel, dAccel, detect_signal);
	case CFS2:
		return CFS20start_signal_search1(FAxis, dVel, dAccel, detect_signal);
	default:
		return false;
	}
}
//---------------------------------------------------------------------------
bool ServoMotor::SignalSearch2(double dVel, BYTE detect_signal)
{
	if( InMotion() )
		return false;

	switch( CardType )
    {
	case CAM5:
		return C5Mstart_signal_search2(FAxis, dVel, detect_signal);
	case CFS:
		return CFSstart_signal_search2(FAxis, dVel, detect_signal);
	case CFS2:
		return CFS20start_signal_search2(FAxis, dVel, detect_signal);
	default:
		return false;
	}
}
//---------------------------------------------------------------------------
bool ServoMotor::Move(double dPos)
{
	FDestPos = dPos; //mm단위 이동거리 저장

	switch( CardType )
    {
	case CAM5:
		return C5Mstart_move(FAxis, MMToPulse(dPos), FMotorParams.HighVel, FMotorParams.HighAccel);
	case CFS:
		return CFSstart_s_move(FAxis, MMToPulse(dPos), FMotorParams.HighVel, FMotorParams.HighAccel);
	case CFS2:
		return CFS20start_s_move(FAxis, MMToPulse(dPos), FMotorParams.HighVel, FMotorParams.HighAccel);
	default:
		return false;
	}
}
//---------------------------------------------------------------------------
bool ServoMotor::Move(double dPos, double vel, double accel)
{
	FDestPos = dPos; //mm단위 이동거리 저장

	switch( CardType )
    {
	case CAM5:
		return C5Mstart_move(FAxis, MMToPulse(dPos), vel, accel);
	case CFS:
		return CFSstart_s_move(FAxis, MMToPulse(dPos), vel, accel);
	case CFS2:
		return CFS20start_s_move(FAxis, MMToPulse(dPos), vel, accel);
	default:
		return false;
	}
}
//---------------------------------------------------------------------------
bool ServoMotor::MoveSlow(double dPos)
{
	FDestPos = dPos; //mm단위 이동거리 저장

	switch( CardType )
    {
	case CAM5:
		return C5Mstart_move(FAxis, MMToPulse(dPos), FMotorParams.LowVel, FMotorParams.LowAccel);
	case CFS:
		return CFSstart_s_move(FAxis, MMToPulse(dPos), FMotorParams.LowVel, FMotorParams.LowAccel);
	case CFS2:
		return CFS20start_s_move(FAxis, MMToPulse(dPos), FMotorParams.LowVel, FMotorParams.LowAccel);
	default:
		return false;
	}
}
//---------------------------------------------------------------------------
void ServoMotor::Stop()
{
	switch( CardType )
    {
	case CAM5:
		C5Mset_stop(FAxis);
		break;
	case CFS:
		CFSset_stop(FAxis);
		break;
	case CFS2:
		CFS20set_stop(FAxis);
		break;
	default:
		break;
	}
}
//---------------------------------------------------------------------------
void ServoMotor::EStop()
{
	switch( CardType )
    {
	case CAM5:
		C5Mset_e_stop(FAxis);
		break;
	case CFS:
		CFSset_e_stop(FAxis);
		break;
	case CFS2:
		CFS20set_e_stop(FAxis);
		break;
	default:
		break;
	}
}
//---------------------------------------------------------------------------
double ServoMotor::MMToPulse(double pos)
{
	return (((FResolution * pos) / FLeadScrewPitch) * FGearRate);
	// if FResolution = 10000, FLeadScrewPitch = 10mm,
	// 10000 (pulse) : x (pulse) = 10(mm) : pos(mm)
	// 10x (pulse)(mm) = 10000pos (pulse)(mm)
	// pos = 10x/10000 = x/1000
	// x = 1000pos
}
//---------------------------------------------------------------------------
double ServoMotor::PulseToMM(double pos)
{
	return ((pos * FLeadScrewPitch) / (FResolution * FGearRate));
}
//------------------------- 원점 검색 관련 함수 --------------------------//
void ServoMotor::SetHomeParams(double dVel1,			//= 1000.0,
						  double dVel2,			//= 500.0,
						  double dAccel,		//= 5000.0,
						  double orgOffset,		//= 0.0,
						  double dOffset,		//= 0.0,
						  double dOffsetVel,	//= 5000.0,
						  double dOffsetAcc,	//= 10000.0,
						  bool useZPhase)		//=false)
{
	HomeVel1       = fabs(dVel1);
	HomeVel2       = fabs(dVel2);
	HomeAccel      = fabs(dAccel);
	HomeOrgOffset  = orgOffset;
	HomeOffset     = dOffset;
	OffsetVel      = fabs(dOffsetVel);
	OffsetAcc      = fabs(dOffsetAcc);
	UseZPhase      = useZPhase;
}
//---------------------------------------------------------------------------
void ServoMotor::NextStep(int step /*= -1*/)
{
	switch( RunMode )
	{
	case RUN_HOME:
		if( step < 0 )
			FHomingStep++;
		else
			FHomingStep = step;
		break;
	case RUN_REPEAT:
		if( step < 0 )
			RepeatStep++;
		else
			RepeatStep = step;
		break;
	}
}
//---------------------------------------------------------------------------
void ServoMotor::AbortHoming()
{
	if( !HomingAbort )
		HomingAbort = true;
}
//---------------------------------------------------------------------------
double ServoMotor::GetCurrentPos()
{
	return PulseToMM(GetActPos());
}
