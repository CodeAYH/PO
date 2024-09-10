//---------------------------------------------------------------------------

#ifndef MoonsStepMotorH
#define MoonsStepMotorH
//---------------------------------------------------------------------------
#include "DamperMotor.h"

enum ST_DRIVE_WARNING_CODE
{
	STWC_CCW_LIMIT			= 0x0002,
	STWC_CW_LIMIT			= 0x0004,
	STWC_UNDER_VOLTAGE		= 0x0040,
	STWC_COMM_ERROR			= 0x0400,
	STWC_BAD_FLASH			= 0x0800,
	STWC_NO_MOVE			= 0x1000,
	STWC_BLANK_Q_SEGMENT	= 0x4000
};
enum ST_DRIVE_ALARM_CODE
{
	STAC_OVER_TEMP			= 0x0008,
	STAC_INTERNAL_VOLTAGE	= 0x0010,
	STAC_OVER_VOLTAGE		= 0x0020,
	STAC_OVER_CURRENT		= 0x0080,
	STAC_OPEN_MOTOR_WINDING	= 0x0100,
	STAC_BAD_ENCODER		= 0x0200
};

class MoonsStepMotor : public DamperMotor
{
enum { DECODE_STATUS=0, DECODE_CURRENTPOS, DECODE_ALARM };

private:
	int DecodeMode;
	unsigned short StatusCode;

	bool FMoving;
	bool FDriveFault;
	bool FAlarmPresent;

	int FVel;
	int FAccel;
	int FDecel;
	int FRevolutionCounter;	// 한바퀴 돌리는데 필요한 펄스 수

	void DoMonitor();
	void DecodeMonitor();
	bool IsRxDone();
	void GetMonitorCommands(TStringList *list);

	void DoAlarmClear(PARAM *param);
	void DoMotorEnable(PARAM *param);
	void DoStop(PARAM *param);
	void DoSetVelocity(PARAM *param);
	void DoSetAccel(PARAM *param);
	void DoSetDecel(PARAM *param);
	void DoSetDistance(PARAM *param);
	void DoGoAbsoluteStep(PARAM *param);
	void DoGoRelativeStep(PARAM *param);
	void DoSetEncoderPosition(PARAM *param);
	void DoSetPosition(PARAM *param);
	void DoDefineLimit(PARAM *param);
	void DoSeekHome(PARAM *param);

	void DoFindLimitSensor(PARAM *param);

	bool SetAccel(int accel);
	bool SetDecel(int decel);
	bool SetVelocity(int vel);
	bool SetDistance(int pos);
	bool GoRelativeStep();
	bool GoAbsoluteStep();
	bool SetEncoderPosition(int pos);
	bool SetPosition(int pos);
	bool DefineLimit(int limit);
	bool SeekHome();
	bool FindLimitSensor();

	bool MoveStepRelative(int pos);
	bool MoveStepAbsolute(int pos);

public:
	MoonsStepMotor(BaseChamber *chamber, PORTINFO portInfo);
	__fastcall ~MoonsStepMotor();

	bool IsAlarm() { return (FAlarmCode != 0); }
	bool AlarmClear();
	bool MotorEnable(bool enable=true);
	bool Stop();

	bool SearchHome();
	bool Open(int percent = 100);
	bool Close();
};
#endif
