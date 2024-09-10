#ifndef ServoMotorH
#define ServoMotorH

#include "Defines.h"
#include "AxtLIB.h"
#include "AxtCAMCFS.h"
#include "AxtCAMC5M.h"
#include "AxtCAMCFS20.h"
#include "AxtKeCAMCFS20.h"

const int RUN_IDLE		= 0;
const int RUN_HOME		= 1;
const int RUN_REPEAT	= 2;
const int RUN_STOP		= 3;

const int CAM5  		= 0;
const int CFS   		= 1;
const int CFS2  		= 2;

typedef struct tagMotorParams
{
	// 서보모터 설정 파라미터들
	double HighVel;
	double HighAccel;
	double LowVel;
	double LowAccel;
	double MinLimit;
	double MaxLimit;
} MOTORPARAMS;

class ServoMotor
{
private:
	INT16 FAxis;
	INT16 FMotorID;
	MOTORPARAMS FMotorParams;
	double FDestPos;

	int CardType;
	bool EzSvr;

	int TimerOneShotID;
	int TimerLoopID;
	int RunMode;	// Loop Timer 내에서 돌아가는 Running Mode Select
	bool NPLOrg;

	UINT LoopScanTime;

	bool FAlarmReset;
	BYTE AlarmResetLevel;

	UINT LoopDelayTime;
	UINT DelayTimeOut;

	// 원점 검색 관련 변수
	int FHomingStep;
	bool OrgLevel;
	bool HomingAbort;
	bool FHoming;
	bool FHomeDone;
	bool FHomeEnd;
	bool FHomeFail;

	bool UseZPhase;

	double HomeVel1;
	double HomeVel2;
	double HomeAccel;
	double HomeOffset;
	double HomeOrgOffset;
	double OffsetVel;
	double OffsetAcc;

	// 테스트 구동 관련 함수
	bool FRepeatDone;
	bool RepeatAbort;
	int RepeatStep;

	double RepeatDist;
	double RepeatVel;
	double RepeatAcc;
	DWORD  RepeatWait;

	//모타기어비에 관한함수
	double FResolution;
	double FGearRate;
	double FLeadScrewPitch;

public:
	bool CW;

	__property bool IsRepeatDone = { read = FRepeatDone };
	__property int HomingStep = { read = FHomingStep };

	__property bool IsHoming = { read = FHoming, write = FHoming };
	__property bool IsHomeDone = { read = FHomeDone, write = FHomeDone };
	__property bool IsHomeEnd = { read = FHomeEnd, write = FHomeEnd };
	__property bool IsHomeFail = { read = FHomeFail, write = FHomeFail };
	__property double Resolution = { read = FResolution, write = FResolution };
	__property double GearRate = { read = FGearRate, write = FGearRate };
	__property double LeadScrewPitch = { read = FLeadScrewPitch, write = FLeadScrewPitch };

	__property MOTORPARAMS MotorParams = { read = FMotorParams, write = FMotorParams };
	__property double DestPos = { read = FDestPos, write = FDestPos };

	__property INT16 MotorID = { read = FMotorID, write = FMotorID };
	__property INT16 Axis = { read = FAxis, write = FAxis };

private:
	void SetDelay(UINT uMSec);
	bool IsDelayTimeOut();
	BYTE GetEndStatus();

	void NextStep(int step = -1);

public:
	ServoMotor(const INT16 axis, const int cardType, const bool ezSvr);
	virtual ~ServoMotor();

	void SetAxisPara1(BYTE decelstartpoint = AutoDetect,
					  BYTE pulseoutmethod = TwoCcwCwHigh,
					  BYTE detectsignal = PElmNegativeEdge);
	void SetAxisPara2(BYTE enc_method = Sqr4Mode,
					  BYTE inp_activelevel = InpActiveHigh,
					  BYTE alarm_activelevel = AlmActiveHigh,
					  BYTE nslm_activelevel = NSlmActiveHigh,
					  BYTE pslm_activelevel = PSlmActiveHigh,
					  BYTE nelm_activelevel = NElmActiveHigh,
					  BYTE pelm_activelevel = PElmActiveHigh);
	void SetMoveUnitPerPulse(double dunitperpulse = 1.0);
	void SetStartStopSpeed(double dSpeed = 10.0);
	void SetMaxSpeed(double dSpeed = 500000.0);

	// I/O 관련 함수
	void ServoEnable(bool bStatus);
	bool IsServoEnabled();
	void AlarmReset();
	bool IsAlarmReset();
	bool IsReady();
	bool IsAlarm();
	bool IsPELM();
	bool IsNELM();
	bool IsORG();
	void ToggleIO(BYTE byOutputBit);

	// 위치 설정 관련 함수
	void SetCmdPos(double dPos);
	double GetCmdPos();
	void MatchPos();

	void SetActPos(double dPos);
	double GetActPos();	// 펄스 거리
	double GetCurrentPos();	// mm 거리

	// Motion 관련 함수.
	bool InMotion();
	bool Move(double dPos);
	bool Move(double dPos, double vel, double accel);
	bool MoveSlow(double dPos);

	bool SignalSearch1(double dVel, double dAccel, BYTE detect_signal);
	bool SignalSearch2(double dVel, BYTE detect_signal);
	void Stop();
	void EStop();
	bool InPosition(double dMargin);
	bool InPosition(double dPos, double dMargin);

	// 원점 검색 관련 함수
	void SetHomeParams(double dVel1 = 1000.0,
					   double dVel2 = 500.0,
					   double dAccel = 5000.0,
					   double orgOffset = 0.0,
					   double dOffset = 0.0,
					   double dOffsetVel = 5000.0,
					   double dOffsetAcc = 10000.0,
					   bool useZPhase = false);
	void AbortHoming();

	double MMToPulse(double pos);
	double PulseToMM(double pos);
};
//---------------------------------------------------------------------------
#endif
